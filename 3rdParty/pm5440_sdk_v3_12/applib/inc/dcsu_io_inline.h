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
 *     and register accessor functions for the dcsu block
 *****************************************************************************/
#ifndef _DCSU_IO_INLINE_H
#define _DCSU_IO_INLINE_H

#include "digi_api.h"
#include "dcsu_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DCSU_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for dcsu
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
    UINT32 dcsu_num;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} dcsu_buffer_t;
static INLINE void dcsu_buffer_init( dcsu_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
                                     UINT32 dcsu_num ) ALWAYS_INLINE;
static INLINE void dcsu_buffer_init( dcsu_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
                                     UINT32 dcsu_num )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->dcsu_num                 = dcsu_num;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s dcsu_num=%d", "dcsu_buffer_init", dcsu_num);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void dcsu_buffer_flush( dcsu_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void dcsu_buffer_flush( dcsu_buffer_t *b_ptr )
{
    IOLOG( "dcsu_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 dcsu_reg_read( dcsu_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
                                    UINT32 dcsu_num,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_read( dcsu_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
                                    UINT32 dcsu_num,
                                    UINT32 mem_type,
                                    UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->dcsu_num)*0x1000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg);
    return value;
}
static INLINE void dcsu_reg_write( dcsu_buffer_t *b_ptr,
                                   digi_handle_t *h_ptr,
                                   UINT32 dcsu_num,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_write( dcsu_buffer_t *b_ptr,
                                   digi_handle_t *h_ptr,
                                   UINT32 dcsu_num,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->dcsu_num)*0x1000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg,
                         value);
}

static INLINE void dcsu_field_set( dcsu_buffer_t *b_ptr,
                                   digi_handle_t *h_ptr,
                                   UINT32 dcsu_num,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_set( dcsu_buffer_t *b_ptr,
                                   digi_handle_t *h_ptr,
                                   UINT32 dcsu_num,
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
                          (b_ptr->base_address + (b_ptr->dcsu_num)*0x1000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void dcsu_action_on_write_field_set( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_action_on_write_field_set( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         (b_ptr->base_address + (b_ptr->dcsu_num)*0x1000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg,
                         value<<ofs);
}

static INLINE void dcsu_burst_read( dcsu_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
                                    UINT32 dcsu_num,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void dcsu_burst_read( dcsu_buffer_t *b_ptr,
                                    digi_handle_t *h_ptr,
                                    UINT32 dcsu_num,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->dcsu_num)*0x1000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg,
                          len,
                          value);
}

static INLINE void dcsu_burst_write( dcsu_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
                                     UINT32 dcsu_num,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void dcsu_burst_write( dcsu_buffer_t *b_ptr,
                                     digi_handle_t *h_ptr,
                                     UINT32 dcsu_num,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->dcsu_num)*0x1000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE dcsu_poll( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_poll( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
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
                           (b_ptr->base_address + (b_ptr->dcsu_num)*0x1000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (dcsu_num)*0x1000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for dcsu
 * ==================================================================================
 */

static INLINE void dcsu_reg_DCO_CTRL_REG_write( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_DCO_CTRL_REG_write( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_DCO_CTRL_REG_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_DCO_CTRL_REG,
                    value);
}

static INLINE void dcsu_reg_DCO_CTRL_REG_field_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_DCO_CTRL_REG_field_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_DCO_CTRL_REG_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_DCO_CTRL_REG,
                    mask,
                    PMC_DCSU_40N_REG_DCO_CTRL_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_DCO_CTRL_REG_read( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_DCO_CTRL_REG_read( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_DCO_CTRL_REG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_DCO_CTRL_REG_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_DCSU_CLK_CTRL_REG_write( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_DCSU_CLK_CTRL_REG_write( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_DCSU_CLK_CTRL_REG_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG,
                    value);
}

static INLINE void dcsu_reg_DCSU_CLK_CTRL_REG_field_set( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_DCSU_CLK_CTRL_REG_field_set( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_DCSU_CLK_CTRL_REG_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG,
                    mask,
                    PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_DCSU_CLK_CTRL_REG_read( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_DCSU_CLK_CTRL_REG_read( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_DCSU_CLK_CTRL_REG_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_PRE_DIVIDER_CFG_REG_write( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_PRE_DIVIDER_CFG_REG_write( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_PRE_DIVIDER_CFG_REG_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG,
                    value);
}

static INLINE void dcsu_reg_PRE_DIVIDER_CFG_REG_field_set( dcsu_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 dcsu_num,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_PRE_DIVIDER_CFG_REG_field_set( dcsu_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 dcsu_num,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_PRE_DIVIDER_CFG_REG_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG,
                    mask,
                    PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_PRE_DIVIDER_CFG_REG_read( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_PRE_DIVIDER_CFG_REG_read( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_PRE_DIVIDER_CFG_REG_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_POST_DIV_CTRL_write( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_POST_DIV_CTRL_write( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_POST_DIV_CTRL_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_POST_DIV_CTRL,
                    value);
}

static INLINE void dcsu_reg_POST_DIV_CTRL_field_set( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_POST_DIV_CTRL_field_set( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_POST_DIV_CTRL_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_POST_DIV_CTRL,
                    mask,
                    PMC_DCSU_40N_REG_POST_DIV_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_POST_DIV_CTRL_read( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_POST_DIV_CTRL_read( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_POST_DIV_CTRL);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_POST_DIV_CTRL_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_POST_DIVIDER_CFG_REG_write( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_POST_DIVIDER_CFG_REG_write( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_POST_DIVIDER_CFG_REG_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG,
                    value);
}

static INLINE void dcsu_reg_POST_DIVIDER_CFG_REG_field_set( dcsu_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 dcsu_num,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_POST_DIVIDER_CFG_REG_field_set( dcsu_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 dcsu_num,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_POST_DIVIDER_CFG_REG_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG,
                    mask,
                    PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_POST_DIVIDER_CFG_REG_read( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_POST_DIVIDER_CFG_REG_read( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_POST_DIVIDER_CFG_REG_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_FB_CONFIG_write( dcsu_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 dcsu_num,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_FB_CONFIG_write( dcsu_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 dcsu_num,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_FB_CONFIG_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_FB_CONFIG,
                    value);
}

static INLINE void dcsu_reg_FB_CONFIG_field_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_FB_CONFIG_field_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_FB_CONFIG_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_FB_CONFIG,
                    mask,
                    PMC_DCSU_40N_REG_FB_CONFIG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_FB_CONFIG_read( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_FB_CONFIG_read( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_FB_CONFIG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_FB_CONFIG_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_FB_NUMERATOR_write( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_FB_NUMERATOR_write( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_FB_NUMERATOR_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_FB_NUMERATOR,
                    value);
}

static INLINE void dcsu_reg_FB_NUMERATOR_field_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_FB_NUMERATOR_field_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_FB_NUMERATOR_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_FB_NUMERATOR,
                    mask,
                    PMC_DCSU_40N_REG_FB_NUMERATOR_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_FB_NUMERATOR_read( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_FB_NUMERATOR_read( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_FB_NUMERATOR);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_FB_NUMERATOR_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_FB_DENOMINATOR_write( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_FB_DENOMINATOR_write( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_FB_DENOMINATOR_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_FB_DENOMINATOR,
                    value);
}

static INLINE void dcsu_reg_FB_DENOMINATOR_field_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_FB_DENOMINATOR_field_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_FB_DENOMINATOR_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_FB_DENOMINATOR,
                    mask,
                    PMC_DCSU_40N_REG_FB_DENOMINATOR_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_FB_DENOMINATOR_read( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_FB_DENOMINATOR_read( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_FB_DENOMINATOR);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_FB_DENOMINATOR_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_MODULATION_CFG_write( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_MODULATION_CFG_write( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_MODULATION_CFG_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_MODULATION_CFG,
                    value);
}

static INLINE void dcsu_reg_MODULATION_CFG_field_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_MODULATION_CFG_field_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_MODULATION_CFG_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_MODULATION_CFG,
                    mask,
                    PMC_DCSU_40N_REG_MODULATION_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_MODULATION_CFG_read( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_MODULATION_CFG_read( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_MODULATION_CFG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_MODULATION_CFG_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_M_NUMERATOR_write( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_M_NUMERATOR_write( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_M_NUMERATOR_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_M_NUMERATOR,
                    value);
}

static INLINE void dcsu_reg_M_NUMERATOR_field_set( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_M_NUMERATOR_field_set( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_M_NUMERATOR_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_M_NUMERATOR,
                    mask,
                    PMC_DCSU_40N_REG_M_NUMERATOR_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_M_NUMERATOR_read( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_M_NUMERATOR_read( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_M_NUMERATOR);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_M_NUMERATOR_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_M_STEPSIZE_write( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_M_STEPSIZE_write( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_M_STEPSIZE_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_M_STEPSIZE,
                    value);
}

static INLINE void dcsu_reg_M_STEPSIZE_field_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_M_STEPSIZE_field_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_M_STEPSIZE_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_CONFIG,
                    PMC_DCSU_40N_REG_M_STEPSIZE,
                    mask,
                    PMC_DCSU_40N_REG_M_STEPSIZE_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_M_STEPSIZE_read( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_M_STEPSIZE_read( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_CONFIG,
                               PMC_DCSU_40N_REG_M_STEPSIZE);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_M_STEPSIZE_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE void dcsu_reg_DCSU_INTS_write( dcsu_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 dcsu_num,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_DCSU_INTS_write( dcsu_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 dcsu_num,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_DCSU_INTS_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_STATUS,
                    PMC_DCSU_40N_REG_DCSU_INTS,
                    value);
}

static INLINE void dcsu_reg_DCSU_INTS_action_on_write_field_set( dcsu_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 dcsu_num,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_DCSU_INTS_action_on_write_field_set( dcsu_buffer_t *b_ptr,
                                                                 digi_handle_t *h_ptr,
                                                                 UINT32 dcsu_num,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_DCSU_INTS_action_on_write_field_set", dcsu_num, mask, ofs, value );
    dcsu_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    dcsu_num,
                                    MEM_TYPE_STATUS,
                                    PMC_DCSU_40N_REG_DCSU_INTS,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 dcsu_reg_DCSU_INTS_read( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_DCSU_INTS_read( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_STATUS,
                               PMC_DCSU_40N_REG_DCSU_INTS);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_DCSU_INTS_read", reg_value, dcsu_num);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_reg_DCSU_INTS_poll( dcsu_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 dcsu_num,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_reg_DCSU_INTS_poll( dcsu_buffer_t *b_ptr,
                                                            digi_handle_t *h_ptr,
                                                            UINT32 dcsu_num,
                                                            UINT32 mask,
                                                            UINT32 value,
                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                            UINT32 max_count,
                                                            UINT32 *num_failed_polls,
                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "dcsu_reg_DCSU_INTS_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return dcsu_poll( b_ptr,
                      h_ptr,
                      dcsu_num,
                      MEM_TYPE_STATUS,
                      PMC_DCSU_40N_REG_DCSU_INTS,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void dcsu_reg_INT_EN_write( dcsu_buffer_t *b_ptr,
                                          digi_handle_t *h_ptr,
                                          UINT32 dcsu_num,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_INT_EN_write( dcsu_buffer_t *b_ptr,
                                          digi_handle_t *h_ptr,
                                          UINT32 dcsu_num,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_INT_EN_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_STATUS,
                    PMC_DCSU_40N_REG_INT_EN,
                    value);
}

static INLINE void dcsu_reg_INT_EN_field_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_INT_EN_field_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_INT_EN_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_STATUS,
                    PMC_DCSU_40N_REG_INT_EN,
                    mask,
                    PMC_DCSU_40N_REG_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_INT_EN_read( dcsu_buffer_t *b_ptr,
                                           digi_handle_t *h_ptr,
                                           UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_INT_EN_read( dcsu_buffer_t *b_ptr,
                                           digi_handle_t *h_ptr,
                                           UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_STATUS,
                               PMC_DCSU_40N_REG_INT_EN);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_INT_EN_read", reg_value, dcsu_num);
    return reg_value;
}

static INLINE UINT32 dcsu_reg_DCSU_INT_VAL_REG_read( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_DCSU_INT_VAL_REG_read( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_STATUS,
                               PMC_DCSU_40N_REG_DCSU_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_DCSU_INT_VAL_REG_read", reg_value, dcsu_num);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_reg_DCSU_INT_VAL_REG_poll( dcsu_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 dcsu_num,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_reg_DCSU_INT_VAL_REG_poll( dcsu_buffer_t *b_ptr,
                                                                   digi_handle_t *h_ptr,
                                                                   UINT32 dcsu_num,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "dcsu_reg_DCSU_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return dcsu_poll( b_ptr,
                      h_ptr,
                      dcsu_num,
                      MEM_TYPE_STATUS,
                      PMC_DCSU_40N_REG_DCSU_INT_VAL_REG,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void dcsu_reg_SW_RESET_REG_write( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_SW_RESET_REG_write( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcsu_reg_SW_RESET_REG_write", value );
    dcsu_reg_write( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_STATUS,
                    PMC_DCSU_40N_REG_SW_RESET_REG,
                    value);
}

static INLINE void dcsu_reg_SW_RESET_REG_field_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_reg_SW_RESET_REG_field_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- dcsu_num=%d mask=0x%08x ofs=%d value=0x%08x", "dcsu_reg_SW_RESET_REG_field_set", dcsu_num, mask, ofs, value );
    dcsu_field_set( b_ptr,
                    h_ptr,
                    dcsu_num,
                    MEM_TYPE_STATUS,
                    PMC_DCSU_40N_REG_SW_RESET_REG,
                    mask,
                    PMC_DCSU_40N_REG_SW_RESET_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 dcsu_reg_SW_RESET_REG_read( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_reg_SW_RESET_REG_read( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num )
{
    UINT32 reg_value;

    reg_value = dcsu_reg_read( b_ptr,
                               h_ptr,
                               dcsu_num,
                               MEM_TYPE_STATUS,
                               PMC_DCSU_40N_REG_SW_RESET_REG);

    IOLOG( "%s -> 0x%08x; dcsu_num=%d", "dcsu_reg_SW_RESET_REG_read", reg_value, dcsu_num);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void dcsu_field_DIV_RSTB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_DIV_RSTB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV_RSTB_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_DIV_RSTB_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_DIV_RSTB_set", dcsu_num, value );

    /* (0x000a0004 bits 2) field DIV_RSTB of register PMC_DCSU_40N_REG_DCO_CTRL_REG */
    dcsu_reg_DCO_CTRL_REG_field_set( b_ptr,
                                     h_ptr,
                                     dcsu_num,
                                     DCSU_40N_REG_DCO_CTRL_REG_BIT_DIV_RSTB_MSK,
                                     DCSU_40N_REG_DCO_CTRL_REG_BIT_DIV_RSTB_OFF,
                                     value);
}

static INLINE UINT32 dcsu_field_DIV_RSTB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_DIV_RSTB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV_RSTB_get", dcsu_num, 8);
    /* (0x000a0004 bits 2) field DIV_RSTB of register PMC_DCSU_40N_REG_DCO_CTRL_REG */
    reg_value = dcsu_reg_DCO_CTRL_REG_read( b_ptr,
                                            h_ptr,
                                            dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCO_CTRL_REG_BIT_DIV_RSTB_MSK) >> DCSU_40N_REG_DCO_CTRL_REG_BIT_DIV_RSTB_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_DIV_RSTB_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_ARSTB_set( dcsu_buffer_t *b_ptr,
                                         digi_handle_t *h_ptr,
                                         UINT32 dcsu_num,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_ARSTB_set( dcsu_buffer_t *b_ptr,
                                         digi_handle_t *h_ptr,
                                         UINT32 dcsu_num,
                                         UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ARSTB_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_ARSTB_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_ARSTB_set", dcsu_num, value );

    /* (0x000a0004 bits 1) field ARSTB of register PMC_DCSU_40N_REG_DCO_CTRL_REG */
    dcsu_reg_DCO_CTRL_REG_field_set( b_ptr,
                                     h_ptr,
                                     dcsu_num,
                                     DCSU_40N_REG_DCO_CTRL_REG_BIT_ARSTB_MSK,
                                     DCSU_40N_REG_DCO_CTRL_REG_BIT_ARSTB_OFF,
                                     value);
}

static INLINE UINT32 dcsu_field_ARSTB_get( dcsu_buffer_t *b_ptr,
                                           digi_handle_t *h_ptr,
                                           UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ARSTB_get( dcsu_buffer_t *b_ptr,
                                           digi_handle_t *h_ptr,
                                           UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ARSTB_get", dcsu_num, 8);
    /* (0x000a0004 bits 1) field ARSTB of register PMC_DCSU_40N_REG_DCO_CTRL_REG */
    reg_value = dcsu_reg_DCO_CTRL_REG_read( b_ptr,
                                            h_ptr,
                                            dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCO_CTRL_REG_BIT_ARSTB_MSK) >> DCSU_40N_REG_DCO_CTRL_REG_BIT_ARSTB_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ARSTB_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_ENB_set( dcsu_buffer_t *b_ptr,
                                       digi_handle_t *h_ptr,
                                       UINT32 dcsu_num,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_ENB_set( dcsu_buffer_t *b_ptr,
                                       digi_handle_t *h_ptr,
                                       UINT32 dcsu_num,
                                       UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ENB_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_ENB_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_ENB_set", dcsu_num, value );

    /* (0x000a0004 bits 0) field ENB of register PMC_DCSU_40N_REG_DCO_CTRL_REG */
    dcsu_reg_DCO_CTRL_REG_field_set( b_ptr,
                                     h_ptr,
                                     dcsu_num,
                                     DCSU_40N_REG_DCO_CTRL_REG_BIT_ENB_MSK,
                                     DCSU_40N_REG_DCO_CTRL_REG_BIT_ENB_OFF,
                                     value);
}

static INLINE UINT32 dcsu_field_ENB_get( dcsu_buffer_t *b_ptr,
                                         digi_handle_t *h_ptr,
                                         UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ENB_get( dcsu_buffer_t *b_ptr,
                                         digi_handle_t *h_ptr,
                                         UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ENB_get", dcsu_num, 8);
    /* (0x000a0004 bits 0) field ENB of register PMC_DCSU_40N_REG_DCO_CTRL_REG */
    reg_value = dcsu_reg_DCO_CTRL_REG_read( b_ptr,
                                            h_ptr,
                                            dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCO_CTRL_REG_BIT_ENB_MSK) >> DCSU_40N_REG_DCO_CTRL_REG_BIT_ENB_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ENB_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_DIVIDER_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_DIVIDER_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIVIDER_CLKSEL_set", dcsu_num, 8);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_DIVIDER_CLKSEL_set", value, 3);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_DIVIDER_CLKSEL_set", dcsu_num, value );

    /* (0x000a0008 bits 7:6) field DIVIDER_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_MSK,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_OFF,
                                          value);
}

static INLINE UINT32 dcsu_field_DIVIDER_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_DIVIDER_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIVIDER_CLKSEL_get", dcsu_num, 8);
    /* (0x000a0008 bits 7:6) field DIVIDER_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_MSK) >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_DIVIDER_CLKSEL_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_DIVIDER_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_DIVIDER_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_DIVIDER_CLKSEL_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_DIVIDER_CLKSEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_DIVIDER_CLKSEL_set", stop_bit, 1 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_DIVIDER_CLKSEL_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0008 bits 7:6) field DIVIDER_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
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
        /* (0x000a0008 bits 7:6) field DIVIDER_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
        dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                              h_ptr,
                                              dcsu_num,
                                              subfield_mask << (DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_OFF + subfield_offset),
                                              DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_DIVIDER_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 dcsu_num,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_DIVIDER_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 dcsu_num,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_DIVIDER_CLKSEL_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_DIVIDER_CLKSEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_DIVIDER_CLKSEL_get", stop_bit, 1 );
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
    /* (0x000a0008 bits 7:6) field DIVIDER_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_MSK)
                  >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_MSK, DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_DIVIDER_CLKSEL_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_DCO_CTL_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_DCO_CTL_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DCO_CTL_CLKSEL_set", dcsu_num, 8);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_DCO_CTL_CLKSEL_set", value, 3);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_DCO_CTL_CLKSEL_set", dcsu_num, value );

    /* (0x000a0008 bits 5:4) field DCO_CTL_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_MSK,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_OFF,
                                          value);
}

static INLINE UINT32 dcsu_field_DCO_CTL_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_DCO_CTL_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DCO_CTL_CLKSEL_get", dcsu_num, 8);
    /* (0x000a0008 bits 5:4) field DCO_CTL_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_MSK) >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_DCO_CTL_CLKSEL_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_DCO_CTL_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_DCO_CTL_CLKSEL_set( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_DCO_CTL_CLKSEL_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_DCO_CTL_CLKSEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_DCO_CTL_CLKSEL_set", stop_bit, 1 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_DCO_CTL_CLKSEL_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0008 bits 5:4) field DCO_CTL_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
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
        /* (0x000a0008 bits 5:4) field DCO_CTL_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
        dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                              h_ptr,
                                              dcsu_num,
                                              subfield_mask << (DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_OFF + subfield_offset),
                                              DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_DCO_CTL_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 dcsu_num,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_DCO_CTL_CLKSEL_get( dcsu_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 dcsu_num,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_DCO_CTL_CLKSEL_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_DCO_CTL_CLKSEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_DCO_CTL_CLKSEL_get", stop_bit, 1 );
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
    /* (0x000a0008 bits 5:4) field DCO_CTL_CLKSEL of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_MSK)
                  >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_MSK, DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_DCO_CTL_CLKSEL_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_DIV4_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_DIV4_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV4_ENB_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_DIV4_ENB_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_DIV4_ENB_set", dcsu_num, value );

    /* (0x000a0008 bits 3) field DIV4_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV4_ENB_MSK,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV4_ENB_OFF,
                                          value);
}

static INLINE UINT32 dcsu_field_DIV4_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_DIV4_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV4_ENB_get", dcsu_num, 8);
    /* (0x000a0008 bits 3) field DIV4_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV4_ENB_MSK) >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV4_ENB_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_DIV4_ENB_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_DIV3_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_DIV3_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV3_ENB_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_DIV3_ENB_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_DIV3_ENB_set", dcsu_num, value );

    /* (0x000a0008 bits 2) field DIV3_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV3_ENB_MSK,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV3_ENB_OFF,
                                          value);
}

static INLINE UINT32 dcsu_field_DIV3_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_DIV3_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV3_ENB_get", dcsu_num, 8);
    /* (0x000a0008 bits 2) field DIV3_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV3_ENB_MSK) >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV3_ENB_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_DIV3_ENB_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_DIV2_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_DIV2_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV2_ENB_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_DIV2_ENB_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_DIV2_ENB_set", dcsu_num, value );

    /* (0x000a0008 bits 1) field DIV2_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV2_ENB_MSK,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV2_ENB_OFF,
                                          value);
}

static INLINE UINT32 dcsu_field_DIV2_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_DIV2_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV2_ENB_get", dcsu_num, 8);
    /* (0x000a0008 bits 1) field DIV2_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV2_ENB_MSK) >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV2_ENB_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_DIV2_ENB_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_DIV1_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_DIV1_ENB_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV1_ENB_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_DIV1_ENB_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_DIV1_ENB_set", dcsu_num, value );

    /* (0x000a0008 bits 0) field DIV1_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    dcsu_reg_DCSU_CLK_CTRL_REG_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV1_ENB_MSK,
                                          DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV1_ENB_OFF,
                                          value);
}

static INLINE UINT32 dcsu_field_DIV1_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_DIV1_ENB_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_DIV1_ENB_get", dcsu_num, 8);
    /* (0x000a0008 bits 0) field DIV1_ENB of register PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG */
    reg_value = dcsu_reg_DCSU_CLK_CTRL_REG_read( b_ptr,
                                                 h_ptr,
                                                 dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV1_ENB_MSK) >> DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV1_ENB_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_DIV1_ENB_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_PRE_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_PRE_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_PRE_DIV_BY_set", dcsu_num, 8);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_PRE_DIV_BY_set", value, 31);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_PRE_DIV_BY_set", dcsu_num, value );

    /* (0x000a000c bits 4:0) bits 0:4 use field PRE_DIV_BY of register PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG */
    dcsu_reg_PRE_DIVIDER_CFG_REG_field_set( b_ptr,
                                            h_ptr,
                                            dcsu_num,
                                            DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_MSK,
                                            DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_OFF,
                                            value);
}

static INLINE UINT32 dcsu_field_PRE_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_PRE_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_PRE_DIV_BY_get", dcsu_num, 8);
    /* (0x000a000c bits 4:0) bits 0:4 use field PRE_DIV_BY of register PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_PRE_DIVIDER_CFG_REG_read( b_ptr,
                                                   h_ptr,
                                                   dcsu_num);
    value = (reg_value & DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_MSK) >> DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_PRE_DIV_BY_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_PRE_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_PRE_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_PRE_DIV_BY_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_PRE_DIV_BY_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_PRE_DIV_BY_set", stop_bit, 4 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_PRE_DIV_BY_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a000c bits 4:0) bits 0:4 use field PRE_DIV_BY of register PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG */
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
        /* (0x000a000c bits 4:0) bits 0:4 use field PRE_DIV_BY of register PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG */
        dcsu_reg_PRE_DIVIDER_CFG_REG_field_set( b_ptr,
                                                h_ptr,
                                                dcsu_num,
                                                subfield_mask << (DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_OFF + subfield_offset),
                                                DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_PRE_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_PRE_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_PRE_DIV_BY_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_PRE_DIV_BY_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_PRE_DIV_BY_get", stop_bit, 4 );
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
    /* (0x000a000c bits 4:0) bits 0:4 use field PRE_DIV_BY of register PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_PRE_DIVIDER_CFG_REG_read( b_ptr,
                                                   h_ptr,
                                                   dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_MSK)
                  >> DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_MSK, DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_PRE_DIV_BY_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_POST_UPDATE_EN_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_POST_UPDATE_EN_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_UPDATE_EN_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_POST_UPDATE_EN_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_POST_UPDATE_EN_set", dcsu_num, value );

    /* (0x000a0010 bits 31) field POST_UPDATE_EN of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                      h_ptr,
                                      dcsu_num,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_POST_UPDATE_EN_MSK,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_POST_UPDATE_EN_OFF,
                                      value);
}

static INLINE UINT32 dcsu_field_POST_UPDATE_EN_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_POST_UPDATE_EN_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_UPDATE_EN_get", dcsu_num, 8);
    /* (0x000a0010 bits 31) field POST_UPDATE_EN of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_POST_UPDATE_EN_MSK) >> DCSU_40N_REG_POST_DIV_CTRL_BIT_POST_UPDATE_EN_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_POST_UPDATE_EN_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_SHIFT_CLK_SEL_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_SHIFT_CLK_SEL_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_SHIFT_CLK_SEL_set", dcsu_num, 8);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_SHIFT_CLK_SEL_set", value, 15);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_SHIFT_CLK_SEL_set", dcsu_num, value );

    /* (0x000a0010 bits 15:12) bits 0:3 use field SHIFT_CLK_SEL of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                      h_ptr,
                                      dcsu_num,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_MSK,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_OFF,
                                      value);
}

static INLINE UINT32 dcsu_field_SHIFT_CLK_SEL_get( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_SHIFT_CLK_SEL_get( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_SHIFT_CLK_SEL_get", dcsu_num, 8);
    /* (0x000a0010 bits 15:12) bits 0:3 use field SHIFT_CLK_SEL of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_MSK) >> DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_SHIFT_CLK_SEL_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_SHIFT_CLK_SEL_set( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_SHIFT_CLK_SEL_set( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_SHIFT_CLK_SEL_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_SHIFT_CLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_SHIFT_CLK_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_SHIFT_CLK_SEL_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0010 bits 15:12) bits 0:3 use field SHIFT_CLK_SEL of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
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
        /* (0x000a0010 bits 15:12) bits 0:3 use field SHIFT_CLK_SEL of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
        dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          subfield_mask << (DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_OFF + subfield_offset),
                                          DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_SHIFT_CLK_SEL_get( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_SHIFT_CLK_SEL_get( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_SHIFT_CLK_SEL_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_SHIFT_CLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_SHIFT_CLK_SEL_get", stop_bit, 3 );
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
    /* (0x000a0010 bits 15:12) bits 0:3 use field SHIFT_CLK_SEL of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_MSK)
                  >> DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_MSK, DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_SHIFT_CLK_SEL_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_AC_PULSE_REQ_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_AC_PULSE_REQ_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_AC_PULSE_REQ_set", dcsu_num, 8);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_AC_PULSE_REQ_set", value, 15);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_AC_PULSE_REQ_set", dcsu_num, value );

    /* (0x000a0010 bits 11:8) bits 0:3 use field AC_PULSE_REQ of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                      h_ptr,
                                      dcsu_num,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_MSK,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_OFF,
                                      value);
}

static INLINE UINT32 dcsu_field_AC_PULSE_REQ_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_AC_PULSE_REQ_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_AC_PULSE_REQ_get", dcsu_num, 8);
    /* (0x000a0010 bits 11:8) bits 0:3 use field AC_PULSE_REQ of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_MSK) >> DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_AC_PULSE_REQ_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_AC_PULSE_REQ_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_AC_PULSE_REQ_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_AC_PULSE_REQ_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_AC_PULSE_REQ_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_AC_PULSE_REQ_set", stop_bit, 3 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_AC_PULSE_REQ_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0010 bits 11:8) bits 0:3 use field AC_PULSE_REQ of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
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
        /* (0x000a0010 bits 11:8) bits 0:3 use field AC_PULSE_REQ of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
        dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          subfield_mask << (DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_OFF + subfield_offset),
                                          DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_AC_PULSE_REQ_get( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_AC_PULSE_REQ_get( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_AC_PULSE_REQ_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_AC_PULSE_REQ_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_AC_PULSE_REQ_get", stop_bit, 3 );
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
    /* (0x000a0010 bits 11:8) bits 0:3 use field AC_PULSE_REQ of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_MSK)
                  >> DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_MSK, DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_AC_PULSE_REQ_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_CLK_STOP_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_CLK_STOP_set( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num,
                                            UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_CLK_STOP_set", dcsu_num, 8);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_CLK_STOP_set", value, 15);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_CLK_STOP_set", dcsu_num, value );

    /* (0x000a0010 bits 7:4) bits 0:3 use field CLK_STOP of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                      h_ptr,
                                      dcsu_num,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_MSK,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_OFF,
                                      value);
}

static INLINE UINT32 dcsu_field_CLK_STOP_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_CLK_STOP_get( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_CLK_STOP_get", dcsu_num, 8);
    /* (0x000a0010 bits 7:4) bits 0:3 use field CLK_STOP of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_MSK) >> DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_CLK_STOP_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_CLK_STOP_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_CLK_STOP_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_CLK_STOP_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_CLK_STOP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_CLK_STOP_set", stop_bit, 3 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_CLK_STOP_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0010 bits 7:4) bits 0:3 use field CLK_STOP of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
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
        /* (0x000a0010 bits 7:4) bits 0:3 use field CLK_STOP of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
        dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          subfield_mask << (DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_OFF + subfield_offset),
                                          DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_CLK_STOP_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_CLK_STOP_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_CLK_STOP_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_CLK_STOP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_CLK_STOP_get", stop_bit, 3 );
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
    /* (0x000a0010 bits 7:4) bits 0:3 use field CLK_STOP of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_MSK)
                  >> DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_MSK, DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_CLK_STOP_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_CLK_BYPASS_EN_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_CLK_BYPASS_EN_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_CLK_BYPASS_EN_set", dcsu_num, 8);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_CLK_BYPASS_EN_set", value, 15);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_CLK_BYPASS_EN_set", dcsu_num, value );

    /* (0x000a0010 bits 3:0) bits 0:3 use field CLK_BYPASS_EN of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                      h_ptr,
                                      dcsu_num,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_MSK,
                                      DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_OFF,
                                      value);
}

static INLINE UINT32 dcsu_field_CLK_BYPASS_EN_get( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_CLK_BYPASS_EN_get( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_CLK_BYPASS_EN_get", dcsu_num, 8);
    /* (0x000a0010 bits 3:0) bits 0:3 use field CLK_BYPASS_EN of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_MSK) >> DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_CLK_BYPASS_EN_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_CLK_BYPASS_EN_set( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_CLK_BYPASS_EN_set( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_CLK_BYPASS_EN_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_CLK_BYPASS_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_CLK_BYPASS_EN_set", stop_bit, 3 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_CLK_BYPASS_EN_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0010 bits 3:0) bits 0:3 use field CLK_BYPASS_EN of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
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
        /* (0x000a0010 bits 3:0) bits 0:3 use field CLK_BYPASS_EN of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
        dcsu_reg_POST_DIV_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          dcsu_num,
                                          subfield_mask << (DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_OFF + subfield_offset),
                                          DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_CLK_BYPASS_EN_get( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_CLK_BYPASS_EN_get( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_CLK_BYPASS_EN_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_CLK_BYPASS_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_CLK_BYPASS_EN_get", stop_bit, 3 );
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
    /* (0x000a0010 bits 3:0) bits 0:3 use field CLK_BYPASS_EN of register PMC_DCSU_40N_REG_POST_DIV_CTRL */
    reg_value = dcsu_reg_POST_DIV_CTRL_read( b_ptr,
                                             h_ptr,
                                             dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_MSK)
                  >> DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_MSK, DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_CLK_BYPASS_EN_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_POST_SYNC_EN_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_POST_SYNC_EN_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_SYNC_EN_set", dcsu_num, 8);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_POST_SYNC_EN_set", value, 15);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_POST_SYNC_EN_set", dcsu_num, value );

    /* (0x000a0014 bits 31:28) bits 0:3 use field POST_SYNC_EN of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    dcsu_reg_POST_DIVIDER_CFG_REG_field_set( b_ptr,
                                             h_ptr,
                                             dcsu_num,
                                             DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_MSK,
                                             DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_OFF,
                                             value);
}

static INLINE UINT32 dcsu_field_POST_SYNC_EN_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_POST_SYNC_EN_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_SYNC_EN_get", dcsu_num, 8);
    /* (0x000a0014 bits 31:28) bits 0:3 use field POST_SYNC_EN of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_POST_DIVIDER_CFG_REG_read( b_ptr,
                                                    h_ptr,
                                                    dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_MSK) >> DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_POST_SYNC_EN_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_POST_SYNC_EN_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_POST_SYNC_EN_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_POST_SYNC_EN_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_POST_SYNC_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_POST_SYNC_EN_set", stop_bit, 3 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_POST_SYNC_EN_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0014 bits 31:28) bits 0:3 use field POST_SYNC_EN of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
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
        /* (0x000a0014 bits 31:28) bits 0:3 use field POST_SYNC_EN of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
        dcsu_reg_POST_DIVIDER_CFG_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 dcsu_num,
                                                 subfield_mask << (DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_OFF + subfield_offset),
                                                 DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_POST_SYNC_EN_get( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_POST_SYNC_EN_get( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_POST_SYNC_EN_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_POST_SYNC_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_POST_SYNC_EN_get", stop_bit, 3 );
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
    /* (0x000a0014 bits 31:28) bits 0:3 use field POST_SYNC_EN of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_POST_DIVIDER_CFG_REG_read( b_ptr,
                                                    h_ptr,
                                                    dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_MSK)
                  >> DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_MSK, DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_POST_SYNC_EN_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_POST_SYNC_CNT_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_POST_SYNC_CNT_set( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num,
                                                 UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_SYNC_CNT_set", dcsu_num, 8);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_POST_SYNC_CNT_set", value, 31);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_POST_SYNC_CNT_set", dcsu_num, value );

    /* (0x000a0014 bits 24:20) bits 0:4 use field POST_SYNC_CNT of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    dcsu_reg_POST_DIVIDER_CFG_REG_field_set( b_ptr,
                                             h_ptr,
                                             dcsu_num,
                                             DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_MSK,
                                             DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_OFF,
                                             value);
}

static INLINE UINT32 dcsu_field_POST_SYNC_CNT_get( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_POST_SYNC_CNT_get( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_SYNC_CNT_get", dcsu_num, 8);
    /* (0x000a0014 bits 24:20) bits 0:4 use field POST_SYNC_CNT of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_POST_DIVIDER_CFG_REG_read( b_ptr,
                                                    h_ptr,
                                                    dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_MSK) >> DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_POST_SYNC_CNT_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_POST_SYNC_CNT_set( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_POST_SYNC_CNT_set( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_POST_SYNC_CNT_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_POST_SYNC_CNT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_POST_SYNC_CNT_set", stop_bit, 4 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_POST_SYNC_CNT_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0014 bits 24:20) bits 0:4 use field POST_SYNC_CNT of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
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
        /* (0x000a0014 bits 24:20) bits 0:4 use field POST_SYNC_CNT of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
        dcsu_reg_POST_DIVIDER_CFG_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 dcsu_num,
                                                 subfield_mask << (DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_OFF + subfield_offset),
                                                 DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_POST_SYNC_CNT_get( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_POST_SYNC_CNT_get( dcsu_buffer_t *b_ptr,
                                                         digi_handle_t *h_ptr,
                                                         UINT32 dcsu_num,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_POST_SYNC_CNT_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_POST_SYNC_CNT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_POST_SYNC_CNT_get", stop_bit, 4 );
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
    /* (0x000a0014 bits 24:20) bits 0:4 use field POST_SYNC_CNT of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_POST_DIVIDER_CFG_REG_read( b_ptr,
                                                    h_ptr,
                                                    dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_MSK)
                  >> DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_MSK, DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_POST_SYNC_CNT_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_POST_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_POST_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num,
                                               UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_DIV_BY_set", dcsu_num, 8);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_POST_DIV_BY_set", value, 1048575);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_POST_DIV_BY_set", dcsu_num, value );

    /* (0x000a0014 bits 19:0) bits 0:19 use field POST_DIV_BY of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    dcsu_reg_POST_DIVIDER_CFG_REG_field_set( b_ptr,
                                             h_ptr,
                                             dcsu_num,
                                             DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_MSK,
                                             DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_OFF,
                                             value);
}

static INLINE UINT32 dcsu_field_POST_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_POST_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_POST_DIV_BY_get", dcsu_num, 8);
    /* (0x000a0014 bits 19:0) bits 0:19 use field POST_DIV_BY of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_POST_DIVIDER_CFG_REG_read( b_ptr,
                                                    h_ptr,
                                                    dcsu_num);
    value = (reg_value & DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_MSK) >> DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_POST_DIV_BY_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_POST_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_POST_DIV_BY_set( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_POST_DIV_BY_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_POST_DIV_BY_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_POST_DIV_BY_set", stop_bit, 19 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_POST_DIV_BY_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0014 bits 19:0) bits 0:19 use field POST_DIV_BY of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
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
        /* (0x000a0014 bits 19:0) bits 0:19 use field POST_DIV_BY of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
        dcsu_reg_POST_DIVIDER_CFG_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 dcsu_num,
                                                 subfield_mask << (DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_OFF + subfield_offset),
                                                 DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_POST_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_POST_DIV_BY_get( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_POST_DIV_BY_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_POST_DIV_BY_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_POST_DIV_BY_get", stop_bit, 19 );
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
    /* (0x000a0014 bits 19:0) bits 0:19 use field POST_DIV_BY of register PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG */
    reg_value = dcsu_reg_POST_DIVIDER_CFG_REG_read( b_ptr,
                                                    h_ptr,
                                                    dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_MSK)
                  >> DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_MSK, DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_POST_DIV_BY_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_FB_UPDATE_EN_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_FB_UPDATE_EN_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_UPDATE_EN_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_FB_UPDATE_EN_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_FB_UPDATE_EN_set", dcsu_num, value );

    /* (0x000a0018 bits 31) field FB_UPDATE_EN of register PMC_DCSU_40N_REG_FB_CONFIG */
    dcsu_reg_FB_CONFIG_field_set( b_ptr,
                                  h_ptr,
                                  dcsu_num,
                                  DCSU_40N_REG_FB_CONFIG_BIT_FB_UPDATE_EN_MSK,
                                  DCSU_40N_REG_FB_CONFIG_BIT_FB_UPDATE_EN_OFF,
                                  value);
}

static INLINE UINT32 dcsu_field_FB_UPDATE_EN_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_FB_UPDATE_EN_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_UPDATE_EN_get", dcsu_num, 8);
    /* (0x000a0018 bits 31) field FB_UPDATE_EN of register PMC_DCSU_40N_REG_FB_CONFIG */
    reg_value = dcsu_reg_FB_CONFIG_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_FB_CONFIG_BIT_FB_UPDATE_EN_MSK) >> DCSU_40N_REG_FB_CONFIG_BIT_FB_UPDATE_EN_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_FB_UPDATE_EN_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_FB_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_FB_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_INT_DIV_set", dcsu_num, 8);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_FB_INT_DIV_set", value, 255);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_FB_INT_DIV_set", dcsu_num, value );

    /* (0x000a0018 bits 7:0) bits 0:7 use field FB_INT_DIV of register PMC_DCSU_40N_REG_FB_CONFIG */
    dcsu_reg_FB_CONFIG_field_set( b_ptr,
                                  h_ptr,
                                  dcsu_num,
                                  DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_MSK,
                                  DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_OFF,
                                  value);
}

static INLINE UINT32 dcsu_field_FB_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_FB_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_INT_DIV_get", dcsu_num, 8);
    /* (0x000a0018 bits 7:0) bits 0:7 use field FB_INT_DIV of register PMC_DCSU_40N_REG_FB_CONFIG */
    reg_value = dcsu_reg_FB_CONFIG_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_MSK) >> DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_FB_INT_DIV_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_FB_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_FB_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_FB_INT_DIV_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_FB_INT_DIV_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_FB_INT_DIV_set", stop_bit, 7 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_FB_INT_DIV_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0018 bits 7:0) bits 0:7 use field FB_INT_DIV of register PMC_DCSU_40N_REG_FB_CONFIG */
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
        /* (0x000a0018 bits 7:0) bits 0:7 use field FB_INT_DIV of register PMC_DCSU_40N_REG_FB_CONFIG */
        dcsu_reg_FB_CONFIG_field_set( b_ptr,
                                      h_ptr,
                                      dcsu_num,
                                      subfield_mask << (DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_OFF + subfield_offset),
                                      DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_FB_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_FB_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_FB_INT_DIV_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_FB_INT_DIV_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_FB_INT_DIV_get", stop_bit, 7 );
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
    /* (0x000a0018 bits 7:0) bits 0:7 use field FB_INT_DIV of register PMC_DCSU_40N_REG_FB_CONFIG */
    reg_value = dcsu_reg_FB_CONFIG_read(  b_ptr, h_ptr, dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_MSK)
                  >> DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_MSK, DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_FB_INT_DIV_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_FB_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_FB_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num,
                                                UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_NUMERATOR_set", dcsu_num, 8);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_FB_NUMERATOR_set", dcsu_num, value );

    /* (0x000a001c bits 31:0) bits 0:31 use field FB_NUMERATOR of register PMC_DCSU_40N_REG_FB_NUMERATOR */
    dcsu_reg_FB_NUMERATOR_field_set( b_ptr,
                                     h_ptr,
                                     dcsu_num,
                                     DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_MSK,
                                     DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_OFF,
                                     value);
}

static INLINE UINT32 dcsu_field_FB_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_FB_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_NUMERATOR_get", dcsu_num, 8);
    /* (0x000a001c bits 31:0) bits 0:31 use field FB_NUMERATOR of register PMC_DCSU_40N_REG_FB_NUMERATOR */
    reg_value = dcsu_reg_FB_NUMERATOR_read( b_ptr,
                                            h_ptr,
                                            dcsu_num);
    value = (reg_value & DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_MSK) >> DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_FB_NUMERATOR_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_FB_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_FB_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_FB_NUMERATOR_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_FB_NUMERATOR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_FB_NUMERATOR_set", stop_bit, 31 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_FB_NUMERATOR_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a001c bits 31:0) bits 0:31 use field FB_NUMERATOR of register PMC_DCSU_40N_REG_FB_NUMERATOR */
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
        /* (0x000a001c bits 31:0) bits 0:31 use field FB_NUMERATOR of register PMC_DCSU_40N_REG_FB_NUMERATOR */
        dcsu_reg_FB_NUMERATOR_field_set( b_ptr,
                                         h_ptr,
                                         dcsu_num,
                                         subfield_mask << (DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_OFF + subfield_offset),
                                         DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_FB_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_FB_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_FB_NUMERATOR_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_FB_NUMERATOR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_FB_NUMERATOR_get", stop_bit, 31 );
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
    /* (0x000a001c bits 31:0) bits 0:31 use field FB_NUMERATOR of register PMC_DCSU_40N_REG_FB_NUMERATOR */
    reg_value = dcsu_reg_FB_NUMERATOR_read( b_ptr,
                                            h_ptr,
                                            dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_MSK)
                  >> DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_MSK, DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_FB_NUMERATOR_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_FB_DENOMINATOR_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_FB_DENOMINATOR_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_DENOMINATOR_set", dcsu_num, 8);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_FB_DENOMINATOR_set", dcsu_num, value );

    /* (0x000a0020 bits 31:0) bits 0:31 use field FB_DENOMINATOR of register PMC_DCSU_40N_REG_FB_DENOMINATOR */
    dcsu_reg_FB_DENOMINATOR_field_set( b_ptr,
                                       h_ptr,
                                       dcsu_num,
                                       DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_MSK,
                                       DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_OFF,
                                       value);
}

static INLINE UINT32 dcsu_field_FB_DENOMINATOR_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_FB_DENOMINATOR_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_FB_DENOMINATOR_get", dcsu_num, 8);
    /* (0x000a0020 bits 31:0) bits 0:31 use field FB_DENOMINATOR of register PMC_DCSU_40N_REG_FB_DENOMINATOR */
    reg_value = dcsu_reg_FB_DENOMINATOR_read( b_ptr,
                                              h_ptr,
                                              dcsu_num);
    value = (reg_value & DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_MSK) >> DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_FB_DENOMINATOR_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_FB_DENOMINATOR_set( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_FB_DENOMINATOR_set( dcsu_buffer_t *b_ptr,
                                                        digi_handle_t *h_ptr,
                                                        UINT32 dcsu_num,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_FB_DENOMINATOR_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_FB_DENOMINATOR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_FB_DENOMINATOR_set", stop_bit, 31 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_FB_DENOMINATOR_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0020 bits 31:0) bits 0:31 use field FB_DENOMINATOR of register PMC_DCSU_40N_REG_FB_DENOMINATOR */
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
        /* (0x000a0020 bits 31:0) bits 0:31 use field FB_DENOMINATOR of register PMC_DCSU_40N_REG_FB_DENOMINATOR */
        dcsu_reg_FB_DENOMINATOR_field_set( b_ptr,
                                           h_ptr,
                                           dcsu_num,
                                           subfield_mask << (DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_OFF + subfield_offset),
                                           DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_FB_DENOMINATOR_get( dcsu_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 dcsu_num,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_FB_DENOMINATOR_get( dcsu_buffer_t *b_ptr,
                                                          digi_handle_t *h_ptr,
                                                          UINT32 dcsu_num,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_FB_DENOMINATOR_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_FB_DENOMINATOR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_FB_DENOMINATOR_get", stop_bit, 31 );
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
    /* (0x000a0020 bits 31:0) bits 0:31 use field FB_DENOMINATOR of register PMC_DCSU_40N_REG_FB_DENOMINATOR */
    reg_value = dcsu_reg_FB_DENOMINATOR_read( b_ptr,
                                              h_ptr,
                                              dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_MSK)
                  >> DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_MSK, DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_FB_DENOMINATOR_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_M_EN_set( dcsu_buffer_t *b_ptr,
                                        digi_handle_t *h_ptr,
                                        UINT32 dcsu_num,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_M_EN_set( dcsu_buffer_t *b_ptr,
                                        digi_handle_t *h_ptr,
                                        UINT32 dcsu_num,
                                        UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_EN_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_M_EN_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_M_EN_set", dcsu_num, value );

    /* (0x000a0024 bits 31) field M_EN of register PMC_DCSU_40N_REG_MODULATION_CFG */
    dcsu_reg_MODULATION_CFG_field_set( b_ptr,
                                       h_ptr,
                                       dcsu_num,
                                       DCSU_40N_REG_MODULATION_CFG_BIT_M_EN_MSK,
                                       DCSU_40N_REG_MODULATION_CFG_BIT_M_EN_OFF,
                                       value);
}

static INLINE UINT32 dcsu_field_M_EN_get( dcsu_buffer_t *b_ptr,
                                          digi_handle_t *h_ptr,
                                          UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_M_EN_get( dcsu_buffer_t *b_ptr,
                                          digi_handle_t *h_ptr,
                                          UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_EN_get", dcsu_num, 8);
    /* (0x000a0024 bits 31) field M_EN of register PMC_DCSU_40N_REG_MODULATION_CFG */
    reg_value = dcsu_reg_MODULATION_CFG_read( b_ptr,
                                              h_ptr,
                                              dcsu_num);
    value = (reg_value & DCSU_40N_REG_MODULATION_CFG_BIT_M_EN_MSK) >> DCSU_40N_REG_MODULATION_CFG_BIT_M_EN_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_M_EN_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_M_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 dcsu_num,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_M_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                             digi_handle_t *h_ptr,
                                             UINT32 dcsu_num,
                                             UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_INT_DIV_set", dcsu_num, 8);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_M_INT_DIV_set", value, 255);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_M_INT_DIV_set", dcsu_num, value );

    /* (0x000a0024 bits 7:0) bits 0:7 use field M_INT_DIV of register PMC_DCSU_40N_REG_MODULATION_CFG */
    dcsu_reg_MODULATION_CFG_field_set( b_ptr,
                                       h_ptr,
                                       dcsu_num,
                                       DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_MSK,
                                       DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_OFF,
                                       value);
}

static INLINE UINT32 dcsu_field_M_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_M_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_INT_DIV_get", dcsu_num, 8);
    /* (0x000a0024 bits 7:0) bits 0:7 use field M_INT_DIV of register PMC_DCSU_40N_REG_MODULATION_CFG */
    reg_value = dcsu_reg_MODULATION_CFG_read( b_ptr,
                                              h_ptr,
                                              dcsu_num);
    value = (reg_value & DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_MSK) >> DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_M_INT_DIV_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_M_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_M_INT_DIV_set( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_M_INT_DIV_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_M_INT_DIV_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_M_INT_DIV_set", stop_bit, 7 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_M_INT_DIV_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0024 bits 7:0) bits 0:7 use field M_INT_DIV of register PMC_DCSU_40N_REG_MODULATION_CFG */
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
        /* (0x000a0024 bits 7:0) bits 0:7 use field M_INT_DIV of register PMC_DCSU_40N_REG_MODULATION_CFG */
        dcsu_reg_MODULATION_CFG_field_set( b_ptr,
                                           h_ptr,
                                           dcsu_num,
                                           subfield_mask << (DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_OFF + subfield_offset),
                                           DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_M_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_M_INT_DIV_get( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_M_INT_DIV_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_M_INT_DIV_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_M_INT_DIV_get", stop_bit, 7 );
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
    /* (0x000a0024 bits 7:0) bits 0:7 use field M_INT_DIV of register PMC_DCSU_40N_REG_MODULATION_CFG */
    reg_value = dcsu_reg_MODULATION_CFG_read( b_ptr,
                                              h_ptr,
                                              dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_MSK)
                  >> DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_MSK, DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_M_INT_DIV_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_M_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_M_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num,
                                               UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_NUMERATOR_set", dcsu_num, 8);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_M_NUMERATOR_set", dcsu_num, value );

    /* (0x000a0028 bits 31:0) bits 0:31 use field M_NUMERATOR of register PMC_DCSU_40N_REG_M_NUMERATOR */
    dcsu_reg_M_NUMERATOR_field_set( b_ptr,
                                    h_ptr,
                                    dcsu_num,
                                    DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_MSK,
                                    DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_OFF,
                                    value);
}

static INLINE UINT32 dcsu_field_M_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_M_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                 digi_handle_t *h_ptr,
                                                 UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_NUMERATOR_get", dcsu_num, 8);
    /* (0x000a0028 bits 31:0) bits 0:31 use field M_NUMERATOR of register PMC_DCSU_40N_REG_M_NUMERATOR */
    reg_value = dcsu_reg_M_NUMERATOR_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_MSK) >> DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_M_NUMERATOR_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_M_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_M_NUMERATOR_set( dcsu_buffer_t *b_ptr,
                                                     digi_handle_t *h_ptr,
                                                     UINT32 dcsu_num,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_M_NUMERATOR_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_M_NUMERATOR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_M_NUMERATOR_set", stop_bit, 31 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_M_NUMERATOR_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a0028 bits 31:0) bits 0:31 use field M_NUMERATOR of register PMC_DCSU_40N_REG_M_NUMERATOR */
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
        /* (0x000a0028 bits 31:0) bits 0:31 use field M_NUMERATOR of register PMC_DCSU_40N_REG_M_NUMERATOR */
        dcsu_reg_M_NUMERATOR_field_set( b_ptr,
                                        h_ptr,
                                        dcsu_num,
                                        subfield_mask << (DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_OFF + subfield_offset),
                                        DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_M_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_M_NUMERATOR_get( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_M_NUMERATOR_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_M_NUMERATOR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_M_NUMERATOR_get", stop_bit, 31 );
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
    /* (0x000a0028 bits 31:0) bits 0:31 use field M_NUMERATOR of register PMC_DCSU_40N_REG_M_NUMERATOR */
    reg_value = dcsu_reg_M_NUMERATOR_read(  b_ptr, h_ptr, dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_MSK)
                  >> DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_MSK, DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_M_NUMERATOR_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcsu_field_M_STEPSIZE_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_M_STEPSIZE_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_STEPSIZE_set", dcsu_num, 8);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_M_STEPSIZE_set", dcsu_num, value );

    /* (0x000a002c bits 31:0) bits 0:31 use field M_STEPSIZE of register PMC_DCSU_40N_REG_M_STEPSIZE */
    dcsu_reg_M_STEPSIZE_field_set( b_ptr,
                                   h_ptr,
                                   dcsu_num,
                                   DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_MSK,
                                   DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_OFF,
                                   value);
}

static INLINE UINT32 dcsu_field_M_STEPSIZE_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_M_STEPSIZE_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_M_STEPSIZE_get", dcsu_num, 8);
    /* (0x000a002c bits 31:0) bits 0:31 use field M_STEPSIZE of register PMC_DCSU_40N_REG_M_STEPSIZE */
    reg_value = dcsu_reg_M_STEPSIZE_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_MSK) >> DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_M_STEPSIZE_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_range_M_STEPSIZE_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_range_M_STEPSIZE_set( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_M_STEPSIZE_set", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_M_STEPSIZE_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_M_STEPSIZE_set", stop_bit, 31 );
    IOLOG( "%s <= dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_M_STEPSIZE_set", dcsu_num, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000a002c bits 31:0) bits 0:31 use field M_STEPSIZE of register PMC_DCSU_40N_REG_M_STEPSIZE */
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
        /* (0x000a002c bits 31:0) bits 0:31 use field M_STEPSIZE of register PMC_DCSU_40N_REG_M_STEPSIZE */
        dcsu_reg_M_STEPSIZE_field_set( b_ptr,
                                       h_ptr,
                                       dcsu_num,
                                       subfield_mask << (DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_OFF + subfield_offset),
                                       DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 dcsu_field_range_M_STEPSIZE_get( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_range_M_STEPSIZE_get( dcsu_buffer_t *b_ptr,
                                                      digi_handle_t *h_ptr,
                                                      UINT32 dcsu_num,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_range_M_STEPSIZE_get", dcsu_num, 8);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcsu_field_range_M_STEPSIZE_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcsu_field_range_M_STEPSIZE_get", stop_bit, 31 );
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
    /* (0x000a002c bits 31:0) bits 0:31 use field M_STEPSIZE of register PMC_DCSU_40N_REG_M_STEPSIZE */
    reg_value = dcsu_reg_M_STEPSIZE_read(  b_ptr, h_ptr, dcsu_num);
    field_value = (reg_value & DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_MSK)
                  >> DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_MSK, DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> dcsu_num=%d start_bit=%d stop_bit=%d 0x%08x", "dcsu_field_range_M_STEPSIZE_get", dcsu_num, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void dcsu_field_ERR_FAST_I_set_to_clear( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_ERR_FAST_I_set_to_clear( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_FAST_I_set_to_clear", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_ERR_FAST_I_set_to_clear", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_ERR_FAST_I_set_to_clear", dcsu_num, value );

    /* (0x000a0034 bits 2) field ERR_FAST_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    dcsu_reg_DCSU_INTS_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  dcsu_num,
                                                  DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_MSK,
                                                  DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_OFF,
                                                  value);
}

static INLINE UINT32 dcsu_field_ERR_FAST_I_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ERR_FAST_I_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_FAST_I_get", dcsu_num, 8);
    /* (0x000a0034 bits 2) field ERR_FAST_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    reg_value = dcsu_reg_DCSU_INTS_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_MSK) >> DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ERR_FAST_I_get", dcsu_num, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_FAST_I_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_FAST_I_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  dcsu_num=%d 0x%08x", "dcsu_field_ERR_FAST_I_poll", dcsu_num, value );

    /* (0x000a0034 bits 2) field ERR_FAST_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    return dcsu_reg_DCSU_INTS_poll( b_ptr,
                                    h_ptr,
                                    dcsu_num,
                                    DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_MSK,
                                    (value<<DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void dcsu_field_ERR_SLOW_I_set_to_clear( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_ERR_SLOW_I_set_to_clear( dcsu_buffer_t *b_ptr,
                                                       digi_handle_t *h_ptr,
                                                       UINT32 dcsu_num,
                                                       UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_SLOW_I_set_to_clear", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_ERR_SLOW_I_set_to_clear", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_ERR_SLOW_I_set_to_clear", dcsu_num, value );

    /* (0x000a0034 bits 1) field ERR_SLOW_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    dcsu_reg_DCSU_INTS_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  dcsu_num,
                                                  DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_MSK,
                                                  DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_OFF,
                                                  value);
}

static INLINE UINT32 dcsu_field_ERR_SLOW_I_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ERR_SLOW_I_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_SLOW_I_get", dcsu_num, 8);
    /* (0x000a0034 bits 1) field ERR_SLOW_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    reg_value = dcsu_reg_DCSU_INTS_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_MSK) >> DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ERR_SLOW_I_get", dcsu_num, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_SLOW_I_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_SLOW_I_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  dcsu_num=%d 0x%08x", "dcsu_field_ERR_SLOW_I_poll", dcsu_num, value );

    /* (0x000a0034 bits 1) field ERR_SLOW_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    return dcsu_reg_DCSU_INTS_poll( b_ptr,
                                    h_ptr,
                                    dcsu_num,
                                    DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_MSK,
                                    (value<<DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_OFF),
                                    cmp,
                                    max_count,
                                    num_failed_polls,
                                    delay_between_polls_in_microseconds);
}

static INLINE void dcsu_field_LOCK_I_set_to_clear( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_LOCK_I_set_to_clear( dcsu_buffer_t *b_ptr,
                                                   digi_handle_t *h_ptr,
                                                   UINT32 dcsu_num,
                                                   UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_LOCK_I_set_to_clear", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_LOCK_I_set_to_clear", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_LOCK_I_set_to_clear", dcsu_num, value );

    /* (0x000a0034 bits 0) field LOCK_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    dcsu_reg_DCSU_INTS_action_on_write_field_set( b_ptr,
                                                  h_ptr,
                                                  dcsu_num,
                                                  DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_MSK,
                                                  DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_OFF,
                                                  value);
}

static INLINE UINT32 dcsu_field_LOCK_I_get( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_LOCK_I_get( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_LOCK_I_get", dcsu_num, 8);
    /* (0x000a0034 bits 0) field LOCK_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    reg_value = dcsu_reg_DCSU_INTS_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_MSK) >> DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_LOCK_I_get", dcsu_num, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_LOCK_I_poll( dcsu_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 dcsu_num,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_LOCK_I_poll( dcsu_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 dcsu_num,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  dcsu_num=%d 0x%08x", "dcsu_field_LOCK_I_poll", dcsu_num, value );

    /* (0x000a0034 bits 0) field LOCK_I of register PMC_DCSU_40N_REG_DCSU_INTS */
    return dcsu_reg_DCSU_INTS_poll( b_ptr,
                                    h_ptr,
                                    dcsu_num,
                                    DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_MSK,
                                    (value<<DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_OFF),
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
static INLINE void dcsu_field_ERR_FAST_E_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_ERR_FAST_E_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_FAST_E_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_ERR_FAST_E_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_ERR_FAST_E_set", dcsu_num, value );

    /* (0x000a0038 bits 2) field ERR_FAST_E of register PMC_DCSU_40N_REG_INT_EN */
    dcsu_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               dcsu_num,
                               DCSU_40N_REG_INT_EN_BIT_ERR_FAST_E_MSK,
                               DCSU_40N_REG_INT_EN_BIT_ERR_FAST_E_OFF,
                               value);
}

static INLINE UINT32 dcsu_field_ERR_FAST_E_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ERR_FAST_E_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_FAST_E_get", dcsu_num, 8);
    /* (0x000a0038 bits 2) field ERR_FAST_E of register PMC_DCSU_40N_REG_INT_EN */
    reg_value = dcsu_reg_INT_EN_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_INT_EN_BIT_ERR_FAST_E_MSK) >> DCSU_40N_REG_INT_EN_BIT_ERR_FAST_E_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ERR_FAST_E_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_ERR_SLOW_E_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_ERR_SLOW_E_set( dcsu_buffer_t *b_ptr,
                                              digi_handle_t *h_ptr,
                                              UINT32 dcsu_num,
                                              UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_SLOW_E_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_ERR_SLOW_E_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_ERR_SLOW_E_set", dcsu_num, value );

    /* (0x000a0038 bits 1) field ERR_SLOW_E of register PMC_DCSU_40N_REG_INT_EN */
    dcsu_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               dcsu_num,
                               DCSU_40N_REG_INT_EN_BIT_ERR_SLOW_E_MSK,
                               DCSU_40N_REG_INT_EN_BIT_ERR_SLOW_E_OFF,
                               value);
}

static INLINE UINT32 dcsu_field_ERR_SLOW_E_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ERR_SLOW_E_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_SLOW_E_get", dcsu_num, 8);
    /* (0x000a0038 bits 1) field ERR_SLOW_E of register PMC_DCSU_40N_REG_INT_EN */
    reg_value = dcsu_reg_INT_EN_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_INT_EN_BIT_ERR_SLOW_E_MSK) >> DCSU_40N_REG_INT_EN_BIT_ERR_SLOW_E_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ERR_SLOW_E_get", dcsu_num, value );

    return value;
}
static INLINE void dcsu_field_LOCK_E_set( dcsu_buffer_t *b_ptr,
                                          digi_handle_t *h_ptr,
                                          UINT32 dcsu_num,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_LOCK_E_set( dcsu_buffer_t *b_ptr,
                                          digi_handle_t *h_ptr,
                                          UINT32 dcsu_num,
                                          UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_LOCK_E_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_LOCK_E_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_LOCK_E_set", dcsu_num, value );

    /* (0x000a0038 bits 0) field LOCK_E of register PMC_DCSU_40N_REG_INT_EN */
    dcsu_reg_INT_EN_field_set( b_ptr,
                               h_ptr,
                               dcsu_num,
                               DCSU_40N_REG_INT_EN_BIT_LOCK_E_MSK,
                               DCSU_40N_REG_INT_EN_BIT_LOCK_E_OFF,
                               value);
}

static INLINE UINT32 dcsu_field_LOCK_E_get( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_LOCK_E_get( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_LOCK_E_get", dcsu_num, 8);
    /* (0x000a0038 bits 0) field LOCK_E of register PMC_DCSU_40N_REG_INT_EN */
    reg_value = dcsu_reg_INT_EN_read(  b_ptr, h_ptr, dcsu_num);
    value = (reg_value & DCSU_40N_REG_INT_EN_BIT_LOCK_E_MSK) >> DCSU_40N_REG_INT_EN_BIT_LOCK_E_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_LOCK_E_get", dcsu_num, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 dcsu_field_ERR_FAST_V_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ERR_FAST_V_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_FAST_V_get", dcsu_num, 8);
    /* (0x000a0030 bits 2) field ERR_FAST_V of register PMC_DCSU_40N_REG_DCSU_INT_VAL_REG */
    reg_value = dcsu_reg_DCSU_INT_VAL_REG_read( b_ptr,
                                                h_ptr,
                                                dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_FAST_V_MSK) >> DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_FAST_V_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ERR_FAST_V_get", dcsu_num, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_FAST_V_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_FAST_V_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  dcsu_num=%d 0x%08x", "dcsu_field_ERR_FAST_V_poll", dcsu_num, value );

    /* (0x000a0030 bits 2) field ERR_FAST_V of register PMC_DCSU_40N_REG_DCSU_INT_VAL_REG */
    return dcsu_reg_DCSU_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           dcsu_num,
                                           DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_FAST_V_MSK,
                                           (value<<DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_FAST_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 dcsu_field_ERR_SLOW_V_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_ERR_SLOW_V_get( dcsu_buffer_t *b_ptr,
                                                digi_handle_t *h_ptr,
                                                UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_ERR_SLOW_V_get", dcsu_num, 8);
    /* (0x000a0030 bits 1) field ERR_SLOW_V of register PMC_DCSU_40N_REG_DCSU_INT_VAL_REG */
    reg_value = dcsu_reg_DCSU_INT_VAL_REG_read( b_ptr,
                                                h_ptr,
                                                dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_SLOW_V_MSK) >> DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_SLOW_V_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_ERR_SLOW_V_get", dcsu_num, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_SLOW_V_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_ERR_SLOW_V_poll( dcsu_buffer_t *b_ptr,
                                                               digi_handle_t *h_ptr,
                                                               UINT32 dcsu_num,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  dcsu_num=%d 0x%08x", "dcsu_field_ERR_SLOW_V_poll", dcsu_num, value );

    /* (0x000a0030 bits 1) field ERR_SLOW_V of register PMC_DCSU_40N_REG_DCSU_INT_VAL_REG */
    return dcsu_reg_DCSU_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           dcsu_num,
                                           DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_SLOW_V_MSK,
                                           (value<<DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_SLOW_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 dcsu_field_LOCK_V_get( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_LOCK_V_get( dcsu_buffer_t *b_ptr,
                                            digi_handle_t *h_ptr,
                                            UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_LOCK_V_get", dcsu_num, 8);
    /* (0x000a0030 bits 0) field LOCK_V of register PMC_DCSU_40N_REG_DCSU_INT_VAL_REG */
    reg_value = dcsu_reg_DCSU_INT_VAL_REG_read( b_ptr,
                                                h_ptr,
                                                dcsu_num);
    value = (reg_value & DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_LOCK_V_MSK) >> DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_LOCK_V_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_LOCK_V_get", dcsu_num, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_LOCK_V_poll( dcsu_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 dcsu_num,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcsu_field_LOCK_V_poll( dcsu_buffer_t *b_ptr,
                                                           digi_handle_t *h_ptr,
                                                           UINT32 dcsu_num,
                                                           UINT32 value,
                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                           UINT32 max_count,
                                                           UINT32 *num_failed_polls,
                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  dcsu_num=%d 0x%08x", "dcsu_field_LOCK_V_poll", dcsu_num, value );

    /* (0x000a0030 bits 0) field LOCK_V of register PMC_DCSU_40N_REG_DCSU_INT_VAL_REG */
    return dcsu_reg_DCSU_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           dcsu_num,
                                           DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_LOCK_V_MSK,
                                           (value<<DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_LOCK_V_OFF),
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
static INLINE UINT32 dcsu_field_BOOT_MODE_get( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_BOOT_MODE_get( dcsu_buffer_t *b_ptr,
                                               digi_handle_t *h_ptr,
                                               UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_BOOT_MODE_get", dcsu_num, 8);
    /* (0x000a0000 bits 16) field BOOT_MODE of register PMC_DCSU_40N_REG_SW_RESET_REG */
    reg_value = dcsu_reg_SW_RESET_REG_read( b_ptr,
                                            h_ptr,
                                            dcsu_num);
    value = (reg_value & DCSU_40N_REG_SW_RESET_REG_BIT_BOOT_MODE_MSK) >> DCSU_40N_REG_SW_RESET_REG_BIT_BOOT_MODE_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_BOOT_MODE_get", dcsu_num, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void dcsu_field_SOFTWARE_RESET_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcsu_field_SOFTWARE_RESET_set( dcsu_buffer_t *b_ptr,
                                                  digi_handle_t *h_ptr,
                                                  UINT32 dcsu_num,
                                                  UINT32 value )
{
    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_SOFTWARE_RESET_set", dcsu_num, 8);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcsu_field_SOFTWARE_RESET_set", value, 1);
    IOLOG( "%s <= dcsu_num=%d 0x%08x", "dcsu_field_SOFTWARE_RESET_set", dcsu_num, value );

    /* (0x000a0000 bits 0) field SOFTWARE_RESET of register PMC_DCSU_40N_REG_SW_RESET_REG */
    dcsu_reg_SW_RESET_REG_field_set( b_ptr,
                                     h_ptr,
                                     dcsu_num,
                                     DCSU_40N_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_MSK,
                                     DCSU_40N_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_OFF,
                                     value);
}

static INLINE UINT32 dcsu_field_SOFTWARE_RESET_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcsu_field_SOFTWARE_RESET_get( dcsu_buffer_t *b_ptr,
                                                    digi_handle_t *h_ptr,
                                                    UINT32 dcsu_num )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (dcsu_num > 8)
        IO_RANGE_CHECK("%s dcsu_num is %d but max is %d", "dcsu_field_SOFTWARE_RESET_get", dcsu_num, 8);
    /* (0x000a0000 bits 0) field SOFTWARE_RESET of register PMC_DCSU_40N_REG_SW_RESET_REG */
    reg_value = dcsu_reg_SW_RESET_REG_read( b_ptr,
                                            h_ptr,
                                            dcsu_num);
    value = (reg_value & DCSU_40N_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_MSK) >> DCSU_40N_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_OFF;
    IOLOG( "%s -> dcsu_num=%d 0x%08x", "dcsu_field_SOFTWARE_RESET_get", dcsu_num, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DCSU_IO_INLINE_H */
