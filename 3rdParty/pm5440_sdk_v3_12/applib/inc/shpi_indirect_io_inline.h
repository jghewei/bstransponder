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
 *     and register accessor functions for the shpi_indirect block
 *****************************************************************************/
#ifndef _SHPI_INDIRECT_IO_INLINE_H
#define _SHPI_INDIRECT_IO_INLINE_H

#include "cbrc_api.h"
#include "shpi_indirect_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SHPI_INDIRECT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for shpi_indirect
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

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset CFG
 * ==================================================================================
 */
/*------------------.
 | parameterset CFG |
 |                  |
 | parameters are:  |
 |     NDFCNT       |
 |     SSEN         |
 +-----------------*/
typedef struct {
    shpi_indirect_buffer_t base;
} shpi_indirect_CFG_buffer_t;
static INLINE void shpi_indirect_CFG_buffer_init( shpi_indirect_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void shpi_indirect_CFG_buffer_init( shpi_indirect_CFG_buffer_t *b_ptr )
{
    IOLOG( "%s", "shpi_indirect_buffer_init");
    shpi_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void shpi_indirect_CFG_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                       UINT32 A,
 *                                                       UINT4 some_indexing_parameter,
 *                                                       UINT4 some_other_indexing_parameter,
 *                                                       shpi_indirect_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_CFG_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                       UINT32 A,
 *                                                       UINT4 some_indexing_parameter,
 *                                                       UINT4 some_other_indexing_parameter,
 *                                                       shpi_indirect_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_retrieve", A);
 *     shpi_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void shpi_indirect_CFG_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                    UINT32 A,
 *                                                    UINT4 some_indexing_parameter,
 *                                                    UINT4 some_other_indexing_parameter,
 *                                                    shpi_indirect_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_CFG_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                    UINT32 A,
 *                                                    UINT4 some_indexing_parameter,
 *                                                    UINT4 some_other_indexing_parameter,
 *                                                    shpi_indirect_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_apply", A);
 *     shpi_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void shpi_indirect_ifield_NDFCNT_set( shpi_indirect_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void shpi_indirect_ifield_NDFCNT_set( shpi_indirect_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shpi_indirect_ifield_NDFCNT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "shpi_indirect_ifield_NDFCNT_set", value );

    /* (0x00000003 bits 4) field NDFCNT of register PMC_SHPI_INDIRECT_REG_CFG */
    b_ptr->base.mask[0] |= SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_MSK;
    b_ptr->base.data[0] &= ~SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_MSK;
    b_ptr->base.data[0] |= SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_MSK & (value << SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_OFF);
}

static INLINE UINT32 shpi_indirect_ifield_NDFCNT_get( shpi_indirect_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_NDFCNT_get( shpi_indirect_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 4) field NDFCNT of register PMC_SHPI_INDIRECT_REG_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_MSK) >> SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_NDFCNT_get", value );

    return value;
}
static INLINE void shpi_indirect_ifield_SSEN_set( shpi_indirect_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void shpi_indirect_ifield_SSEN_set( shpi_indirect_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shpi_indirect_ifield_SSEN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "shpi_indirect_ifield_SSEN_set", value );

    /* (0x00000003 bits 0) field SSEN of register PMC_SHPI_INDIRECT_REG_CFG */
    b_ptr->base.mask[0] |= SHPI_INDIRECT_REG_CFG_BIT_SSEN_MSK;
    b_ptr->base.data[0] &= ~SHPI_INDIRECT_REG_CFG_BIT_SSEN_MSK;
    b_ptr->base.data[0] |= SHPI_INDIRECT_REG_CFG_BIT_SSEN_MSK & (value << SHPI_INDIRECT_REG_CFG_BIT_SSEN_OFF);
}

static INLINE UINT32 shpi_indirect_ifield_SSEN_get( shpi_indirect_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_SSEN_get( shpi_indirect_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 0) field SSEN of register PMC_SHPI_INDIRECT_REG_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_CFG_BIT_SSEN_MSK) >> SHPI_INDIRECT_REG_CFG_BIT_SSEN_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_SSEN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset IND_INT_STAT
 * ==================================================================================
 */
/*---------------------------.
 | parameterset IND_INT_STAT |
 |                           |
 | parameters are:           |
 |     PAISCI                |
 |     PLOPCI                |
 |     PAISI                 |
 |     PLOPI                 |
 +--------------------------*/
typedef struct {
    shpi_indirect_buffer_t base;
} shpi_indirect_IND_INT_STAT_buffer_t;
static INLINE void shpi_indirect_IND_INT_STAT_buffer_init( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void shpi_indirect_IND_INT_STAT_buffer_init( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
{
    IOLOG( "%s", "shpi_indirect_buffer_init");
    shpi_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void shpi_indirect_IND_INT_STAT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                UINT32 A,
 *                                                                UINT4 some_indexing_parameter,
 *                                                                UINT4 some_other_indexing_parameter,
 *                                                                shpi_indirect_IND_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_IND_INT_STAT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                UINT32 A,
 *                                                                UINT4 some_indexing_parameter,
 *                                                                UINT4 some_other_indexing_parameter,
 *                                                                shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_retrieve", A);
 *     shpi_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void shpi_indirect_IND_INT_STAT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                             UINT32 A,
 *                                                             UINT4 some_indexing_parameter,
 *                                                             UINT4 some_other_indexing_parameter,
 *                                                             shpi_indirect_IND_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_IND_INT_STAT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                             UINT32 A,
 *                                                             UINT4 some_indexing_parameter,
 *                                                             UINT4 some_other_indexing_parameter,
 *                                                             shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_apply", A);
 *     shpi_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void shpi_indirect_ifield_PAISCI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void shpi_indirect_ifield_PAISCI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shpi_indirect_ifield_PAISCI_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "shpi_indirect_ifield_PAISCI_set_to_clear", value );

    /* (0x00000002 bits 5) field PAISCI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    b_ptr->base.mask[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_MSK;
    b_ptr->base.data[0] &= ~SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_MSK;
    b_ptr->base.data[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_MSK & (value << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_OFF);
}

static INLINE UINT32 shpi_indirect_ifield_PAISCI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PAISCI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 5) field PAISCI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_MSK) >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PAISCI_get", value );

    return value;
}
static INLINE void shpi_indirect_ifield_PLOPCI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void shpi_indirect_ifield_PLOPCI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shpi_indirect_ifield_PLOPCI_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "shpi_indirect_ifield_PLOPCI_set_to_clear", value );

    /* (0x00000002 bits 4) field PLOPCI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    b_ptr->base.mask[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_MSK;
    b_ptr->base.data[0] &= ~SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_MSK;
    b_ptr->base.data[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_MSK & (value << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_OFF);
}

static INLINE UINT32 shpi_indirect_ifield_PLOPCI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PLOPCI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 4) field PLOPCI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_MSK) >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PLOPCI_get", value );

    return value;
}
static INLINE void shpi_indirect_ifield_PAISI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void shpi_indirect_ifield_PAISI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shpi_indirect_ifield_PAISI_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "shpi_indirect_ifield_PAISI_set_to_clear", value );

    /* (0x00000002 bits 3) field PAISI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    b_ptr->base.mask[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_MSK;
    b_ptr->base.data[0] &= ~SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_MSK;
    b_ptr->base.data[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_MSK & (value << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_OFF);
}

static INLINE UINT32 shpi_indirect_ifield_PAISI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PAISI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 3) field PAISI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_MSK) >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PAISI_get", value );

    return value;
}
static INLINE void shpi_indirect_ifield_PLOPI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void shpi_indirect_ifield_PLOPI_set_to_clear( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "shpi_indirect_ifield_PLOPI_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "shpi_indirect_ifield_PLOPI_set_to_clear", value );

    /* (0x00000002 bits 2) field PLOPI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    b_ptr->base.mask[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_MSK;
    b_ptr->base.data[0] &= ~SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_MSK;
    b_ptr->base.data[0] |= SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_MSK & (value << SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_OFF);
}

static INLINE UINT32 shpi_indirect_ifield_PLOPI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PLOPI_get( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 2) field PLOPI of register PMC_SHPI_INDIRECT_REG_IND_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_MSK) >> SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PLOPI_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset IND_INT_VAL
 * ==================================================================================
 */
/*--------------------------.
 | parameterset IND_INT_VAL |
 |                          |
 | parameters are:          |
 |     PAISCV               |
 |     PLOPCV               |
 |     PAISV                |
 |     PLOPV                |
 +-------------------------*/
typedef struct {
    shpi_indirect_buffer_t base;
} shpi_indirect_IND_INT_VAL_buffer_t;
static INLINE void shpi_indirect_IND_INT_VAL_buffer_init( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void shpi_indirect_IND_INT_VAL_buffer_init( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
{
    IOLOG( "%s", "shpi_indirect_buffer_init");
    shpi_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void shpi_indirect_IND_INT_VAL_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               shpi_indirect_IND_INT_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_IND_INT_VAL_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_retrieve", A);
 *     shpi_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void shpi_indirect_IND_INT_VAL_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                            UINT32 A,
 *                                                            UINT4 some_indexing_parameter,
 *                                                            UINT4 some_other_indexing_parameter,
 *                                                            shpi_indirect_IND_INT_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_IND_INT_VAL_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                            UINT32 A,
 *                                                            UINT4 some_indexing_parameter,
 *                                                            UINT4 some_other_indexing_parameter,
 *                                                            shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_apply", A);
 *     shpi_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 shpi_indirect_ifield_PAISCV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PAISCV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field PAISCV of register PMC_SHPI_INDIRECT_REG_IND_INT_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISCV_MSK) >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISCV_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PAISCV_get", value );

    return value;
}
static INLINE UINT32 shpi_indirect_ifield_PLOPCV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PLOPCV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field PLOPCV of register PMC_SHPI_INDIRECT_REG_IND_INT_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPCV_MSK) >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPCV_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PLOPCV_get", value );

    return value;
}
static INLINE UINT32 shpi_indirect_ifield_PAISV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PAISV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field PAISV of register PMC_SHPI_INDIRECT_REG_IND_INT_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISV_MSK) >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISV_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PAISV_get", value );

    return value;
}
static INLINE UINT32 shpi_indirect_ifield_PLOPV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PLOPV_get( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field PLOPV of register PMC_SHPI_INDIRECT_REG_IND_INT_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPV_MSK) >> SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPV_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PLOPV_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT
 * ==================================================================================
 */
/*----------------------------------------------------.
 | parameterset PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT |
 |                                                    |
 | parameters are:                                    |
 |     PNJE                                           |
 +---------------------------------------------------*/
typedef struct {
    shpi_indirect_buffer_t base;
} shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t;
static INLINE void shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_init( shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_init( shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
{
    IOLOG( "%s", "shpi_indirect_buffer_init");
    shpi_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                         UINT32 A,
 *                                                                                         UINT4 some_indexing_parameter,
 *                                                                                         UINT4 some_other_indexing_parameter,
 *                                                                                         shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                         UINT32 A,
 *                                                                                         UINT4 some_indexing_parameter,
 *                                                                                         UINT4 some_other_indexing_parameter,
 *                                                                                         shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_retrieve", A);
 *     shpi_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                      UINT32 A,
 *                                                                                      UINT4 some_indexing_parameter,
 *                                                                                      UINT4 some_other_indexing_parameter,
 *                                                                                      shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                      UINT32 A,
 *                                                                                      UINT4 some_indexing_parameter,
 *                                                                                      UINT4 some_other_indexing_parameter,
 *                                                                                      shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_apply", A);
 *     shpi_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 shpi_indirect_ifield_PNJE_get( shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PNJE_get( shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 12:0) bits 0:12 use field PNJE of register PMC_SHPI_INDIRECT_REG_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_BIT_PNJE_MSK) >> SHPI_INDIRECT_REG_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_BIT_PNJE_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PNJE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset PATH_POSITIVE_JUSTIFICATION_EVENT_CNT
 * ==================================================================================
 */
/*----------------------------------------------------.
 | parameterset PATH_POSITIVE_JUSTIFICATION_EVENT_CNT |
 |                                                    |
 | parameters are:                                    |
 |     PPJE                                           |
 +---------------------------------------------------*/
typedef struct {
    shpi_indirect_buffer_t base;
} shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t;
static INLINE void shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_init( shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_init( shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
{
    IOLOG( "%s", "shpi_indirect_buffer_init");
    shpi_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                         UINT32 A,
 *                                                                                         UINT4 some_indexing_parameter,
 *                                                                                         UINT4 some_other_indexing_parameter,
 *                                                                                         shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                         UINT32 A,
 *                                                                                         UINT4 some_indexing_parameter,
 *                                                                                         UINT4 some_other_indexing_parameter,
 *                                                                                         shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_retrieve", A);
 *     shpi_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                      UINT32 A,
 *                                                                                      UINT4 some_indexing_parameter,
 *                                                                                      UINT4 some_other_indexing_parameter,
 *                                                                                      shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                      UINT32 A,
 *                                                                                      UINT4 some_indexing_parameter,
 *                                                                                      UINT4 some_other_indexing_parameter,
 *                                                                                      shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_apply", A);
 *     shpi_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 shpi_indirect_ifield_PPJE_get( shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PPJE_get( shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000005 bits 12:0) bits 0:12 use field PPJE of register PMC_SHPI_INDIRECT_REG_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_BIT_PPJE_MSK) >> SHPI_INDIRECT_REG_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_BIT_PPJE_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PPJE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset POINTER_VAL
 * ==================================================================================
 */
/*--------------------------.
 | parameterset POINTER_VAL |
 |                          |
 | parameters are:          |
 |     NDF                  |
 |     SSV                  |
 |     PTRV                 |
 +-------------------------*/
typedef struct {
    shpi_indirect_buffer_t base;
} shpi_indirect_POINTER_VAL_buffer_t;
static INLINE void shpi_indirect_POINTER_VAL_buffer_init( shpi_indirect_POINTER_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void shpi_indirect_POINTER_VAL_buffer_init( shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
{
    IOLOG( "%s", "shpi_indirect_buffer_init");
    shpi_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void shpi_indirect_POINTER_VAL_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               shpi_indirect_POINTER_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_POINTER_VAL_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_retrieve", A);
 *     shpi_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void shpi_indirect_POINTER_VAL_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                            UINT32 A,
 *                                                            UINT4 some_indexing_parameter,
 *                                                            UINT4 some_other_indexing_parameter,
 *                                                            shpi_indirect_POINTER_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void shpi_indirect_POINTER_VAL_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                            UINT32 A,
 *                                                            UINT4 some_indexing_parameter,
 *                                                            UINT4 some_other_indexing_parameter,
 *                                                            shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "shpi_indirect_buffer_apply", A);
 *     shpi_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 shpi_indirect_ifield_NDF_get( shpi_indirect_POINTER_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_NDF_get( shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000006 bits 15:12) bits 0:3 use field NDF of register PMC_SHPI_INDIRECT_REG_POINTER_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_POINTER_VAL_BIT_NDF_MSK) >> SHPI_INDIRECT_REG_POINTER_VAL_BIT_NDF_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_NDF_get", value );

    return value;
}
static INLINE UINT32 shpi_indirect_ifield_SSV_get( shpi_indirect_POINTER_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_SSV_get( shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000006 bits 11:10) bits 0:1 use field SSV of register PMC_SHPI_INDIRECT_REG_POINTER_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_POINTER_VAL_BIT_SSV_MSK) >> SHPI_INDIRECT_REG_POINTER_VAL_BIT_SSV_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_SSV_get", value );

    return value;
}
static INLINE UINT32 shpi_indirect_ifield_PTRV_get( shpi_indirect_POINTER_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 shpi_indirect_ifield_PTRV_get( shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000006 bits 9:0) bits 0:9 use field PTRV of register PMC_SHPI_INDIRECT_REG_POINTER_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SHPI_INDIRECT_REG_POINTER_VAL_BIT_PTRV_MSK) >> SHPI_INDIRECT_REG_POINTER_VAL_BIT_PTRV_OFF;
    IOLOG( "%s -> 0x%08x", "shpi_indirect_ifield_PTRV_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SHPI_INDIRECT_IO_INLINE_H */
