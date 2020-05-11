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
 *     and register accessor functions for the pcie_al block
 *****************************************************************************/
#ifndef _PCIE_AL_IO_INLINE_H
#define _PCIE_AL_IO_INLINE_H

#include "pmc_handle.h"
#include "pcie_al_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PCIE_AL_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for pcie_al
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
    pmc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} pcie_al_buffer_t;
static INLINE void pcie_al_buffer_init( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void pcie_al_buffer_init( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "pcie_al_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void pcie_al_buffer_flush( pcie_al_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void pcie_al_buffer_flush( pcie_al_buffer_t *b_ptr )
{
    IOLOG( "pcie_al_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 pcie_al_reg_read( pcie_al_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_read( pcie_al_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
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
static INLINE void pcie_al_reg_write( pcie_al_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_write( pcie_al_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
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

static INLINE void pcie_al_field_set( pcie_al_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_set( pcie_al_buffer_t *b_ptr,
                                      pmc_handle_t *h_ptr,
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

static INLINE void pcie_al_action_on_write_field_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_action_on_write_field_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
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

static INLINE void pcie_al_burst_read( pcie_al_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void pcie_al_burst_read( pcie_al_buffer_t *b_ptr,
                                       pmc_handle_t *h_ptr,
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

static INLINE void pcie_al_burst_write( pcie_al_buffer_t *b_ptr,
                                        pmc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void pcie_al_burst_write( pcie_al_buffer_t *b_ptr,
                                        pmc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE pcie_al_poll( pcie_al_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pcie_al_poll( pcie_al_buffer_t *b_ptr,
                                                 pmc_handle_t *h_ptr,
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
 *  register access functions for pcie_al
 * ==================================================================================
 */

static INLINE void pcie_al_reg_LEGACY_INT_EN_REG_write( pcie_al_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_LEGACY_INT_EN_REG_write( pcie_al_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_LEGACY_INT_EN_REG_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PCIE_REG_LEGACY_INT_EN_REG,
                       value);
}

static INLINE void pcie_al_reg_LEGACY_INT_EN_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_LEGACY_INT_EN_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_LEGACY_INT_EN_REG_field_set", mask, ofs, value );
    pcie_al_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PCIE_REG_LEGACY_INT_EN_REG,
                       mask,
                       PMC_PCIE_REG_LEGACY_INT_EN_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pcie_al_reg_LEGACY_INT_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_LEGACY_INT_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_PCIE_REG_LEGACY_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_LEGACY_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void pcie_al_reg_PCIE_OB_DB_SET_REG_write( pcie_al_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_DB_SET_REG_write( pcie_al_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_PCIE_OB_DB_SET_REG_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_DB_SET_REG,
                       value);
}

static INLINE void pcie_al_reg_PCIE_OB_DB_SET_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_DB_SET_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_PCIE_OB_DB_SET_REG_field_set", mask, ofs, value );
    pcie_al_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_DB_SET_REG,
                       mask,
                       PMC_PCIE_REG_PCIE_OB_DB_SET_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_SET_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_SET_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_DB_SET_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_DB_SET_REG_read", reg_value);
    return reg_value;
}

static INLINE void pcie_al_reg_PCIE_OB_DB_CLR_REG_write( pcie_al_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_DB_CLR_REG_write( pcie_al_buffer_t *b_ptr,
                                                         pmc_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_PCIE_OB_DB_CLR_REG_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_DB_CLR_REG,
                       value);
}

static INLINE void pcie_al_reg_PCIE_OB_DB_CLR_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_DB_CLR_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_PCIE_OB_DB_CLR_REG_field_set", mask, ofs, value );
    pcie_al_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_DB_CLR_REG,
                       mask,
                       PMC_PCIE_REG_PCIE_OB_DB_CLR_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_CLR_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_CLR_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_DB_CLR_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_DB_CLR_REG_read", reg_value);
    return reg_value;
}

static INLINE void pcie_al_reg_PCIE_OB_IRQ_SET_REG_write( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_IRQ_SET_REG_write( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_PCIE_OB_IRQ_SET_REG_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG,
                       value);
}

static INLINE void pcie_al_reg_PCIE_OB_IRQ_SET_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_IRQ_SET_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_PCIE_OB_IRQ_SET_REG_field_set", mask, ofs, value );
    pcie_al_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG,
                       mask,
                       PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_SET_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_SET_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_IRQ_SET_REG_read", reg_value);
    return reg_value;
}

static INLINE void pcie_al_reg_PCIE_OB_IRQ_CLR_REG_write( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_IRQ_CLR_REG_write( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_PCIE_OB_IRQ_CLR_REG_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG,
                       value);
}

static INLINE void pcie_al_reg_PCIE_OB_IRQ_CLR_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_PCIE_OB_IRQ_CLR_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_PCIE_OB_IRQ_CLR_REG_field_set", mask, ofs, value );
    pcie_al_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG,
                       mask,
                       PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_CLR_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_CLR_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_IRQ_CLR_REG_read", reg_value);
    return reg_value;
}

static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_write( pcie_al_buffer_t *b_ptr,
                                                                             pmc_handle_t *h_ptr,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_write( pcie_al_buffer_t *b_ptr,
                                                                             pmc_handle_t *h_ptr,
                                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT,
                       value);
}

static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_action_on_write_field_set( pcie_al_buffer_t *b_ptr,
                                                                                                 pmc_handle_t *h_ptr,
                                                                                                 UINT32 mask,
                                                                                                 UINT32 ofs,
                                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_action_on_write_field_set( pcie_al_buffer_t *b_ptr,
                                                                                                 pmc_handle_t *h_ptr,
                                                                                                 UINT32 mask,
                                                                                                 UINT32 ofs,
                                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_action_on_write_field_set", mask, ofs, value );
    pcie_al_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_poll( pcie_al_buffer_t *b_ptr,
                                                                                            pmc_handle_t *h_ptr,
                                                                                            UINT32 mask,
                                                                                            UINT32 value,
                                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                                            UINT32 max_count,
                                                                                            UINT32 *num_failed_polls,
                                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_poll( pcie_al_buffer_t *b_ptr,
                                                                                            pmc_handle_t *h_ptr,
                                                                                            UINT32 mask,
                                                                                            UINT32 value,
                                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                                            UINT32 max_count,
                                                                                            UINT32 *num_failed_polls,
                                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return pcie_al_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_write( pcie_al_buffer_t *b_ptr,
                                                                                    pmc_handle_t *h_ptr,
                                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_write( pcie_al_buffer_t *b_ptr,
                                                                                    pmc_handle_t *h_ptr,
                                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT,
                       value);
}

static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_action_on_write_field_set( pcie_al_buffer_t *b_ptr,
                                                                                                        pmc_handle_t *h_ptr,
                                                                                                        UINT32 mask,
                                                                                                        UINT32 ofs,
                                                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_action_on_write_field_set( pcie_al_buffer_t *b_ptr,
                                                                                                        pmc_handle_t *h_ptr,
                                                                                                        UINT32 mask,
                                                                                                        UINT32 ofs,
                                                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_action_on_write_field_set", mask, ofs, value );
    pcie_al_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_poll( pcie_al_buffer_t *b_ptr,
                                                                                                   pmc_handle_t *h_ptr,
                                                                                                   UINT32 mask,
                                                                                                   UINT32 value,
                                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                                   UINT32 max_count,
                                                                                                   UINT32 *num_failed_polls,
                                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_poll( pcie_al_buffer_t *b_ptr,
                                                                                                   pmc_handle_t *h_ptr,
                                                                                                   UINT32 mask,
                                                                                                   UINT32 value,
                                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                                   UINT32 max_count,
                                                                                                   UINT32 *num_failed_polls,
                                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return pcie_al_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_write( pcie_al_buffer_t *b_ptr,
                                                                               pmc_handle_t *h_ptr,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_write( pcie_al_buffer_t *b_ptr,
                                                                               pmc_handle_t *h_ptr,
                                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG,
                       value);
}

static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                                                   pmc_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                                                   pmc_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_field_set", mask, ofs, value );
    pcie_al_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG,
                       mask,
                       PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_write( pcie_al_buffer_t *b_ptr,
                                                                                      pmc_handle_t *h_ptr,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_write( pcie_al_buffer_t *b_ptr,
                                                                                      pmc_handle_t *h_ptr,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_write", value );
    pcie_al_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG,
                       value);
}

static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                                                          pmc_handle_t *h_ptr,
                                                                                          UINT32 mask,
                                                                                          UINT32 ofs,
                                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_field_set( pcie_al_buffer_t *b_ptr,
                                                                                          pmc_handle_t *h_ptr,
                                                                                          UINT32 mask,
                                                                                          UINT32 ofs,
                                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_field_set", mask, ofs, value );
    pcie_al_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG,
                       mask,
                       PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 pcie_al_reg_MSI_X_PENDING_BIT_ARRAY_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_MSI_X_PENDING_BIT_ARRAY_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_MSI_X_PENDING_BIT_ARRAY);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_MSI_X_PENDING_BIT_ARRAY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_DB_EN_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_DB_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_STAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_STAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_DB_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_DB_STAT_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_RSTAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_DB_RSTAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_DB_RSTAT_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_DB_RSTAT_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_EN_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_IRQ_EN_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_IRQ_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_STAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_STAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_IRQ_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_IRQ_STAT_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_RSTAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_reg_PCIE_OB_IRQ_RSTAT_REG_read( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = pcie_al_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PCIE_REG_PCIE_OB_IRQ_RSTAT_REG);

    IOLOG( "%s -> 0x%08x;", "pcie_al_reg_PCIE_OB_IRQ_RSTAT_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void pcie_al_field_INTA_EN_set( pcie_al_buffer_t *b_ptr,
                                              pmc_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_INTA_EN_set( pcie_al_buffer_t *b_ptr,
                                              pmc_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "pcie_al_field_INTA_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "pcie_al_field_INTA_EN_set", value );

    /* (0x00009044 bits 8) field INTA_EN of register PMC_PCIE_REG_LEGACY_INT_EN_REG */
    pcie_al_reg_LEGACY_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             PCIE_REG_LEGACY_INT_EN_REG_BIT_INTA_EN_MSK,
                                             PCIE_REG_LEGACY_INT_EN_REG_BIT_INTA_EN_OFF,
                                             value);
}

static INLINE UINT32 pcie_al_field_INTA_EN_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_INTA_EN_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009044 bits 8) field INTA_EN of register PMC_PCIE_REG_LEGACY_INT_EN_REG */
    reg_value = pcie_al_reg_LEGACY_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PCIE_REG_LEGACY_INT_EN_REG_BIT_INTA_EN_MSK) >> PCIE_REG_LEGACY_INT_EN_REG_BIT_INTA_EN_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_INTA_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void pcie_al_field_PCIE_OB_DB_SET_set( pcie_al_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_PCIE_OB_DB_SET_set( pcie_al_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_PCIE_OB_DB_SET_set", value );

    /* (0x00009344 bits 31:0) bits 0:31 use field PCIE_OB_DB_SET of register PMC_PCIE_REG_PCIE_OB_DB_SET_REG */
    pcie_al_reg_PCIE_OB_DB_SET_REG_field_set( b_ptr,
                                              h_ptr,
                                              PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_MSK,
                                              PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_OFF,
                                              value);
}

static INLINE UINT32 pcie_al_field_PCIE_OB_DB_SET_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_SET_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009344 bits 31:0) bits 0:31 use field PCIE_OB_DB_SET of register PMC_PCIE_REG_PCIE_OB_DB_SET_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_SET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_MSK) >> PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_DB_SET_get", value );

    return value;
}
static INLINE void pcie_al_field_range_PCIE_OB_DB_SET_set( pcie_al_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_PCIE_OB_DB_SET_set( pcie_al_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_DB_SET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_DB_SET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_DB_SET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00009344 bits 31:0) bits 0:31 use field PCIE_OB_DB_SET of register PMC_PCIE_REG_PCIE_OB_DB_SET_REG */
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
        /* (0x00009344 bits 31:0) bits 0:31 use field PCIE_OB_DB_SET of register PMC_PCIE_REG_PCIE_OB_DB_SET_REG */
        pcie_al_reg_PCIE_OB_DB_SET_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_OFF + subfield_offset),
                                                  PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_SET_get( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_SET_get( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_DB_SET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_DB_SET_get", stop_bit, 31 );
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
    /* (0x00009344 bits 31:0) bits 0:31 use field PCIE_OB_DB_SET of register PMC_PCIE_REG_PCIE_OB_DB_SET_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_SET_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_MSK)
                  >> PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_MSK, PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_DB_SET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void pcie_al_field_PCIE_OB_DB_CLR_set( pcie_al_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_PCIE_OB_DB_CLR_set( pcie_al_buffer_t *b_ptr,
                                                     pmc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_PCIE_OB_DB_CLR_set", value );

    /* (0x00009348 bits 31:0) bits 0:31 use field PCIE_OB_DB_CLR of register PMC_PCIE_REG_PCIE_OB_DB_CLR_REG */
    pcie_al_reg_PCIE_OB_DB_CLR_REG_field_set( b_ptr,
                                              h_ptr,
                                              PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_MSK,
                                              PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_OFF,
                                              value);
}

static INLINE UINT32 pcie_al_field_PCIE_OB_DB_CLR_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_CLR_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009348 bits 31:0) bits 0:31 use field PCIE_OB_DB_CLR of register PMC_PCIE_REG_PCIE_OB_DB_CLR_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_CLR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_MSK) >> PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_DB_CLR_get", value );

    return value;
}
static INLINE void pcie_al_field_range_PCIE_OB_DB_CLR_set( pcie_al_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_PCIE_OB_DB_CLR_set( pcie_al_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_DB_CLR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_DB_CLR_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_DB_CLR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00009348 bits 31:0) bits 0:31 use field PCIE_OB_DB_CLR of register PMC_PCIE_REG_PCIE_OB_DB_CLR_REG */
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
        /* (0x00009348 bits 31:0) bits 0:31 use field PCIE_OB_DB_CLR of register PMC_PCIE_REG_PCIE_OB_DB_CLR_REG */
        pcie_al_reg_PCIE_OB_DB_CLR_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_OFF + subfield_offset),
                                                  PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_CLR_get( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_CLR_get( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_DB_CLR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_DB_CLR_get", stop_bit, 31 );
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
    /* (0x00009348 bits 31:0) bits 0:31 use field PCIE_OB_DB_CLR of register PMC_PCIE_REG_PCIE_OB_DB_CLR_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_CLR_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_MSK)
                  >> PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_MSK, PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_DB_CLR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void pcie_al_field_PCIE_OB_IRQ_SET_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_PCIE_OB_IRQ_SET_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_PCIE_OB_IRQ_SET_set", value );

    /* (0x00009358 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_SET of register PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG */
    pcie_al_reg_PCIE_OB_IRQ_SET_REG_field_set( b_ptr,
                                               h_ptr,
                                               PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_MSK,
                                               PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_OFF,
                                               value);
}

static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_SET_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_SET_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009358 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_SET of register PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_SET_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_MSK) >> PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_IRQ_SET_get", value );

    return value;
}
static INLINE void pcie_al_field_range_PCIE_OB_IRQ_SET_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_PCIE_OB_IRQ_SET_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_IRQ_SET_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_IRQ_SET_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_IRQ_SET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00009358 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_SET of register PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG */
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
        /* (0x00009358 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_SET of register PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG */
        pcie_al_reg_PCIE_OB_IRQ_SET_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_OFF + subfield_offset),
                                                   PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_SET_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_SET_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_IRQ_SET_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_IRQ_SET_get", stop_bit, 31 );
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
    /* (0x00009358 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_SET of register PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_SET_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_MSK)
                  >> PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_MSK, PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_IRQ_SET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void pcie_al_field_PCIE_OB_IRQ_CLR_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_PCIE_OB_IRQ_CLR_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_PCIE_OB_IRQ_CLR_set", value );

    /* (0x0000935c bits 31:0) bits 0:31 use field PCIE_OB_IRQ_CLR of register PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG */
    pcie_al_reg_PCIE_OB_IRQ_CLR_REG_field_set( b_ptr,
                                               h_ptr,
                                               PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_MSK,
                                               PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_OFF,
                                               value);
}

static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_CLR_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_CLR_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000935c bits 31:0) bits 0:31 use field PCIE_OB_IRQ_CLR of register PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_CLR_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_MSK) >> PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_IRQ_CLR_get", value );

    return value;
}
static INLINE void pcie_al_field_range_PCIE_OB_IRQ_CLR_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_PCIE_OB_IRQ_CLR_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_IRQ_CLR_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_IRQ_CLR_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_IRQ_CLR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000935c bits 31:0) bits 0:31 use field PCIE_OB_IRQ_CLR of register PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG */
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
        /* (0x0000935c bits 31:0) bits 0:31 use field PCIE_OB_IRQ_CLR of register PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG */
        pcie_al_reg_PCIE_OB_IRQ_CLR_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_OFF + subfield_offset),
                                                   PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_CLR_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_CLR_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_IRQ_CLR_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_IRQ_CLR_get", stop_bit, 31 );
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
    /* (0x0000935c bits 31:0) bits 0:31 use field PCIE_OB_IRQ_CLR of register PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_CLR_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_MSK)
                  >> PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_MSK, PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_IRQ_CLR_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void pcie_al_field_MU_IB_DB_I2_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_MU_IB_DB_I2_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                           pmc_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_MU_IB_DB_I2_set_to_clear", value );

    /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
    pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_action_on_write_field_set( b_ptr,
                                                                                  h_ptr,
                                                                                  PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_MSK,
                                                                                  PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF,
                                                                                  value);
}

static INLINE UINT32 pcie_al_field_MU_IB_DB_I2_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_MU_IB_DB_I2_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
    reg_value = pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_read( b_ptr,
                                                                         h_ptr);
    value = (reg_value & PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_MSK) >> PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_MU_IB_DB_I2_get", value );

    return value;
}
static INLINE void pcie_al_field_range_MU_IB_DB_I2_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_MU_IB_DB_I2_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                                 pmc_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_IB_DB_I2_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_IB_DB_I2_set_to_clear", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_IB_DB_I2_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
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
        /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
        pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_action_on_write_field_set( b_ptr,
                                                                                      h_ptr,
                                                                                      subfield_mask << (PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF + subfield_offset),
                                                                                      PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF + subfield_offset,
                                                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_MU_IB_DB_I2_get( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_MU_IB_DB_I2_get( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_IB_DB_I2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_IB_DB_I2_get", stop_bit, 31 );
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
    /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
    reg_value = pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_read( b_ptr,
                                                                         h_ptr);
    field_value = (reg_value & PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_MSK)
                  >> PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_MSK, PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_IB_DB_I2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_range_MU_IB_DB_I2_poll( pcie_al_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_range_MU_IB_DB_I2_poll( pcie_al_buffer_t *b_ptr,
                                                                         pmc_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_IB_DB_I2_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_IB_DB_I2_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_IB_DB_I2_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
        return pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_poll( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF + subfield_offset),
                                                                        value << (PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF + subfield_offset),
                                                                        cmp,
                                                                        max_count,
                                                                        num_failed_polls,
                                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_MU_IB_DB_I2_poll( pcie_al_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_MU_IB_DB_I2_poll( pcie_al_buffer_t *b_ptr,
                                                                   pmc_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "pcie_al_field_MU_IB_DB_I2_poll", value );

    /* (0x00009034 bits 31:0) bits 0:31 use field MU_IB_DB_I2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT */
    return pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_poll( b_ptr,
                                                                    h_ptr,
                                                                    PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_MSK,
                                                                    (value<<PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF),
                                                                    cmp,
                                                                    max_count,
                                                                    num_failed_polls,
                                                                    delay_between_polls_in_microseconds);
}

static INLINE void pcie_al_field_MU_OB_DB_CLR_I3_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_MU_OB_DB_CLR_I3_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_MU_OB_DB_CLR_I3_set_to_clear", value );

    /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
    pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                                         h_ptr,
                                                                                         PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_MSK,
                                                                                         PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF,
                                                                                         value);
}

static INLINE UINT32 pcie_al_field_MU_OB_DB_CLR_I3_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_MU_OB_DB_CLR_I3_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
    reg_value = pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_read( b_ptr,
                                                                                h_ptr);
    value = (reg_value & PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_MSK) >> PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_MU_OB_DB_CLR_I3_get", value );

    return value;
}
static INLINE void pcie_al_field_range_MU_OB_DB_CLR_I3_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_MU_OB_DB_CLR_I3_set_to_clear( pcie_al_buffer_t *b_ptr,
                                                                     pmc_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_OB_DB_CLR_I3_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_OB_DB_CLR_I3_set_to_clear", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_OB_DB_CLR_I3_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
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
        /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
        pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_action_on_write_field_set( b_ptr,
                                                                                             h_ptr,
                                                                                             subfield_mask << (PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF + subfield_offset),
                                                                                             PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF + subfield_offset,
                                                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_MU_OB_DB_CLR_I3_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_MU_OB_DB_CLR_I3_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_OB_DB_CLR_I3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_OB_DB_CLR_I3_get", stop_bit, 31 );
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
    /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
    reg_value = pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_read( b_ptr,
                                                                                h_ptr);
    field_value = (reg_value & PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_MSK)
                  >> PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_MSK, PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_OB_DB_CLR_I3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_range_MU_OB_DB_CLR_I3_poll( pcie_al_buffer_t *b_ptr,
                                                                             pmc_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_range_MU_OB_DB_CLR_I3_poll( pcie_al_buffer_t *b_ptr,
                                                                             pmc_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_OB_DB_CLR_I3_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_OB_DB_CLR_I3_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_OB_DB_CLR_I3_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
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
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
        return pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_poll( b_ptr,
                                                                               h_ptr,
                                                                               subfield_mask << (PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF + subfield_offset),
                                                                               value << (PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF + subfield_offset),
                                                                               cmp,
                                                                               max_count,
                                                                               num_failed_polls,
                                                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_MU_OB_DB_CLR_I3_poll( pcie_al_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE pcie_al_field_MU_OB_DB_CLR_I3_poll( pcie_al_buffer_t *b_ptr,
                                                                       pmc_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "pcie_al_field_MU_OB_DB_CLR_I3_poll", value );

    /* (0x0000903c bits 31:0) bits 0:31 use field MU_OB_DB_CLR_I3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT */
    return pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_poll( b_ptr,
                                                                           h_ptr,
                                                                           PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_MSK,
                                                                           (value<<PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF),
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
static INLINE void pcie_al_field_MU_IB_DB_E2_set( pcie_al_buffer_t *b_ptr,
                                                  pmc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_MU_IB_DB_E2_set( pcie_al_buffer_t *b_ptr,
                                                  pmc_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_MU_IB_DB_E2_set", value );

    /* (0x00009030 bits 31:0) field MU_IB_DB_E2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG */
    pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_field_set( b_ptr,
                                                                    h_ptr,
                                                                    PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_MSK,
                                                                    PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_OFF,
                                                                    value);
}

static INLINE UINT32 pcie_al_field_MU_IB_DB_E2_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_MU_IB_DB_E2_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009030 bits 31:0) field MU_IB_DB_E2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG */
    reg_value = pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_read( b_ptr,
                                                                           h_ptr);
    value = (reg_value & PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_MSK) >> PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_MU_IB_DB_E2_get", value );

    return value;
}
static INLINE void pcie_al_field_range_MU_IB_DB_E2_set( pcie_al_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_MU_IB_DB_E2_set( pcie_al_buffer_t *b_ptr,
                                                        pmc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_IB_DB_E2_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_IB_DB_E2_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_IB_DB_E2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00009030 bits 31:0) field MU_IB_DB_E2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG */
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
        /* (0x00009030 bits 31:0) field MU_IB_DB_E2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG */
        pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_OFF + subfield_offset),
                                                                        PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_MU_IB_DB_E2_get( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_MU_IB_DB_E2_get( pcie_al_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_IB_DB_E2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_IB_DB_E2_get", stop_bit, 31 );
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
    /* (0x00009030 bits 31:0) field MU_IB_DB_E2 of register PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG */
    reg_value = pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_read( b_ptr,
                                                                           h_ptr);
    field_value = (reg_value & PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_MSK)
                  >> PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_MSK, PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_IB_DB_E2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void pcie_al_field_MU_OB_DB_CLR_E3_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_MU_OB_DB_CLR_E3_set( pcie_al_buffer_t *b_ptr,
                                                      pmc_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "pcie_al_field_MU_OB_DB_CLR_E3_set", value );

    /* (0x00009038 bits 31:0) field MU_OB_DB_CLR_E3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG */
    pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_field_set( b_ptr,
                                                                           h_ptr,
                                                                           PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_MSK,
                                                                           PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_OFF,
                                                                           value);
}

static INLINE UINT32 pcie_al_field_MU_OB_DB_CLR_E3_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_MU_OB_DB_CLR_E3_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009038 bits 31:0) field MU_OB_DB_CLR_E3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG */
    reg_value = pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_read( b_ptr,
                                                                                  h_ptr);
    value = (reg_value & PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_MSK) >> PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_MU_OB_DB_CLR_E3_get", value );

    return value;
}
static INLINE void pcie_al_field_range_MU_OB_DB_CLR_E3_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void pcie_al_field_range_MU_OB_DB_CLR_E3_set( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_OB_DB_CLR_E3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_OB_DB_CLR_E3_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_OB_DB_CLR_E3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00009038 bits 31:0) field MU_OB_DB_CLR_E3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG */
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
        /* (0x00009038 bits 31:0) field MU_OB_DB_CLR_E3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG */
        pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_field_set( b_ptr,
                                                                               h_ptr,
                                                                               subfield_mask << (PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_OFF + subfield_offset),
                                                                               PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_OFF + subfield_offset,
                                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 pcie_al_field_range_MU_OB_DB_CLR_E3_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_MU_OB_DB_CLR_E3_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MU_OB_DB_CLR_E3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MU_OB_DB_CLR_E3_get", stop_bit, 31 );
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
    /* (0x00009038 bits 31:0) field MU_OB_DB_CLR_E3 of register PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG */
    reg_value = pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_read( b_ptr,
                                                                                  h_ptr);
    field_value = (reg_value & PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_MSK)
                  >> PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_MSK, PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MU_OB_DB_CLR_E3_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 pcie_al_field_MSIX_PBA_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_MSIX_PBA_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009340 bits 31:0) bits 0:31 use field MSIX_PBA of register PMC_PCIE_REG_MSI_X_PENDING_BIT_ARRAY */
    reg_value = pcie_al_reg_MSI_X_PENDING_BIT_ARRAY_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_MSK) >> PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_MSIX_PBA_get", value );

    return value;
}
static INLINE UINT32 pcie_al_field_range_MSIX_PBA_get( pcie_al_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_MSIX_PBA_get( pcie_al_buffer_t *b_ptr,
                                                       pmc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_MSIX_PBA_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_MSIX_PBA_get", stop_bit, 31 );
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
    /* (0x00009340 bits 31:0) bits 0:31 use field MSIX_PBA of register PMC_PCIE_REG_MSI_X_PENDING_BIT_ARRAY */
    reg_value = pcie_al_reg_MSI_X_PENDING_BIT_ARRAY_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_MSK)
                  >> PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_MSK, PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_MSIX_PBA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_EN_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_EN_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000934c bits 31:0) bits 0:31 use field PCIE_OB_DB_EN of register PMC_PCIE_REG_PCIE_OB_DB_EN_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_MSK) >> PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_DB_EN_get", value );

    return value;
}
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_EN_get( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_EN_get( pcie_al_buffer_t *b_ptr,
                                                            pmc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_DB_EN_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_DB_EN_get", stop_bit, 31 );
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
    /* (0x0000934c bits 31:0) bits 0:31 use field PCIE_OB_DB_EN of register PMC_PCIE_REG_PCIE_OB_DB_EN_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_EN_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_MSK)
                  >> PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_MSK, PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_DB_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_STAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_STAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009350 bits 31:0) bits 0:31 use field PCIE_OB_DB_STAT of register PMC_PCIE_REG_PCIE_OB_DB_STAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_STAT_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_MSK) >> PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_DB_STAT_get", value );

    return value;
}
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_STAT_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_STAT_get( pcie_al_buffer_t *b_ptr,
                                                              pmc_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_DB_STAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_DB_STAT_get", stop_bit, 31 );
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
    /* (0x00009350 bits 31:0) bits 0:31 use field PCIE_OB_DB_STAT of register PMC_PCIE_REG_PCIE_OB_DB_STAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_STAT_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_MSK)
                  >> PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_MSK, PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_DB_STAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_RSTAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_DB_RSTAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009354 bits 31:0) bits 0:31 use field PCIE_OB_DB_RSTAT of register PMC_PCIE_REG_PCIE_OB_DB_RSTAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_RSTAT_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_MSK) >> PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_DB_RSTAT_get", value );

    return value;
}
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_RSTAT_get( pcie_al_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_DB_RSTAT_get( pcie_al_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_DB_RSTAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_DB_RSTAT_get", stop_bit, 31 );
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
    /* (0x00009354 bits 31:0) bits 0:31 use field PCIE_OB_DB_RSTAT of register PMC_PCIE_REG_PCIE_OB_DB_RSTAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_DB_RSTAT_REG_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_MSK)
                  >> PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_MSK, PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_DB_RSTAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_EN_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_EN_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009360 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_EN of register PMC_PCIE_REG_PCIE_OB_IRQ_EN_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_MSK) >> PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_IRQ_EN_get", value );

    return value;
}
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_EN_get( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_EN_get( pcie_al_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_IRQ_EN_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_IRQ_EN_get", stop_bit, 31 );
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
    /* (0x00009360 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_EN of register PMC_PCIE_REG_PCIE_OB_IRQ_EN_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_EN_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_MSK)
                  >> PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_MSK, PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_IRQ_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_STAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_STAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009364 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_STAT of register PMC_PCIE_REG_PCIE_OB_IRQ_STAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_STAT_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_MSK) >> PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_IRQ_STAT_get", value );

    return value;
}
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_STAT_get( pcie_al_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_STAT_get( pcie_al_buffer_t *b_ptr,
                                                               pmc_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_IRQ_STAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_IRQ_STAT_get", stop_bit, 31 );
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
    /* (0x00009364 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_STAT of register PMC_PCIE_REG_PCIE_OB_IRQ_STAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_STAT_REG_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_MSK)
                  >> PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_MSK, PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_IRQ_STAT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_RSTAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_PCIE_OB_IRQ_RSTAT_get( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00009368 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_RSTAT of register PMC_PCIE_REG_PCIE_OB_IRQ_RSTAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_RSTAT_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_MSK) >> PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_OFF;
    IOLOG( "%s -> 0x%08x", "pcie_al_field_PCIE_OB_IRQ_RSTAT_get", value );

    return value;
}
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_RSTAT_get( pcie_al_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 pcie_al_field_range_PCIE_OB_IRQ_RSTAT_get( pcie_al_buffer_t *b_ptr,
                                                                pmc_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "pcie_al_field_range_PCIE_OB_IRQ_RSTAT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "pcie_al_field_range_PCIE_OB_IRQ_RSTAT_get", stop_bit, 31 );
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
    /* (0x00009368 bits 31:0) bits 0:31 use field PCIE_OB_IRQ_RSTAT of register PMC_PCIE_REG_PCIE_OB_IRQ_RSTAT_REG */
    reg_value = pcie_al_reg_PCIE_OB_IRQ_RSTAT_REG_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_MSK)
                  >> PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_MSK, PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "pcie_al_field_range_PCIE_OB_IRQ_RSTAT_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PCIE_AL_IO_INLINE_H */
