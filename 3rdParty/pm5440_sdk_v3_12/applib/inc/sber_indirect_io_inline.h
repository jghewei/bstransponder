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
 *     and register accessor functions for the sber_indirect block
 *****************************************************************************/
#ifndef _SBER_INDIRECT_IO_INLINE_H
#define _SBER_INDIRECT_IO_INLINE_H

#include "sdh_pmg_api.h"
#include "sber_indirect_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SBER_INDIRECT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sber_indirect
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
 * Parameter Access Functions for Paramset SD_BERM_BER_TH
 * ==================================================================================
 */
/*-----------------------------.
 | parameterset SD_BERM_BER_TH |
 |                             |
 | parameters are:             |
 |     SDTR                    |
 +----------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_BERM_BER_TH_buffer_t;
static INLINE void sber_indirect_SD_BERM_BER_TH_buffer_init( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_BERM_BER_TH_buffer_init( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_BER_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_BER_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_BER_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_BER_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SDTR_set( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SDTR_set( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr, UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SDTR_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SDTR_set", value );

    /* (0x00000003 bits 3:0) bits 0:3 use field SDTR of register PMC_SBER_INDIRECT_REG_SD_BERM_BER_TH */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_MSK & (value << SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SDTR_get( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SDTR_get( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 3:0) bits 0:3 use field SDTR of register PMC_SBER_INDIRECT_REG_SD_BERM_BER_TH */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_MSK) >> SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SDTR_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_BERM_CLEAR_TH
 * ==================================================================================
 */
/*-------------------------------.
 | parameterset SD_BERM_CLEAR_TH |
 |                               |
 | parameters are:               |
 |     SDCT                      |
 +------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_BERM_CLEAR_TH_buffer_t;
static INLINE void sber_indirect_SD_BERM_CLEAR_TH_buffer_init( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_BERM_CLEAR_TH_buffer_init( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_CLEAR_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                    UINT32 A,
 *                                                                    UINT4 some_indexing_parameter,
 *                                                                    UINT4 some_other_indexing_parameter,
 *                                                                    sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_CLEAR_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                    UINT32 A,
 *                                                                    UINT4 some_indexing_parameter,
 *                                                                    UINT4 some_other_indexing_parameter,
 *                                                                    sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_CLEAR_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                 UINT32 A,
 *                                                                 UINT4 some_indexing_parameter,
 *                                                                 UINT4 some_other_indexing_parameter,
 *                                                                 sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_CLEAR_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                 UINT32 A,
 *                                                                 UINT4 some_indexing_parameter,
 *                                                                 UINT4 some_other_indexing_parameter,
 *                                                                 sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SDCT_set( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SDCT_set( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr, UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SDCT_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SDCT_set", value );

    /* (0x00000005 bits 11:0) bits 0:11 use field SDCT of register PMC_SBER_INDIRECT_REG_SD_BERM_CLEAR_TH */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_MSK & (value << SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SDCT_get( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SDCT_get( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000005 bits 11:0) bits 0:11 use field SDCT of register PMC_SBER_INDIRECT_REG_SD_BERM_CLEAR_TH */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_MSK) >> SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SDCT_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_BERM_SET_INTEGRATION_PERIOD_1
 * ==================================================================================
 */
/*-----------------------------------------------.
 | parameterset SD_BERM_SET_INTEGRATION_PERIOD_1 |
 |                                               |
 | parameters are:                               |
 +----------------------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t;
static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_init( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_init( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_BERM_SET_INTEGRATION_PERIOD_2
 * ==================================================================================
 */
/*-----------------------------------------------.
 | parameterset SD_BERM_SET_INTEGRATION_PERIOD_2 |
 |                                               |
 | parameters are:                               |
 |     SDIP                                      |
 +----------------------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t;
static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_init( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_init( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SDIP_set( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SDIP_set( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr, UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SDIP_set", value );

    /* (0x00000007 bits 15:0) bits 16:31 use field SDIP of register PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2 */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_MSK & (value << SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_OFF);

    /* (0x00000008 bits 15:0) bits 0:15 use field SDIP of register PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1 */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_MSK & (value << SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SDIP_get( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SDIP_get( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000007 bits 15:0) bits 16:31 use field SDIP of register PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2 */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_MSK) >> SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_OFF;
    value |= field_value;

    /* (0x00000008 bits 15:0) bits 0:15 use field SDIP of register PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1 */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_MSK) >> SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SDIP_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_BERM_SET_TH
 * ==================================================================================
 */
/*-----------------------------.
 | parameterset SD_BERM_SET_TH |
 |                             |
 | parameters are:             |
 |     SDST                    |
 +----------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_BERM_SET_TH_buffer_t;
static INLINE void sber_indirect_SD_BERM_SET_TH_buffer_init( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_BERM_SET_TH_buffer_init( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_SET_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_SET_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_BERM_SET_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_BERM_SET_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SDST_set( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SDST_set( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr, UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SDST_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SDST_set", value );

    /* (0x00000004 bits 11:0) bits 0:11 use field SDST of register PMC_SBER_INDIRECT_REG_SD_BERM_SET_TH */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_MSK & (value << SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SDST_get( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SDST_get( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 11:0) bits 0:11 use field SDST of register PMC_SBER_INDIRECT_REG_SD_BERM_SET_TH */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_MSK) >> SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SDST_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_PATH_MONING_EN
 * ==================================================================================
 */
/*--------------------------------.
 | parameterset SD_PATH_MONING_EN |
 |                                |
 | parameters are:                |
 |     SD_EN                      |
 +-------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_PATH_MONING_EN_buffer_t;
static INLINE void sber_indirect_SD_PATH_MONING_EN_buffer_init( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_PATH_MONING_EN_buffer_init( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_PATH_MONING_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                     UINT32 A,
 *                                                                     UINT4 some_indexing_parameter,
 *                                                                     UINT4 some_other_indexing_parameter,
 *                                                                     sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_PATH_MONING_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                     UINT32 A,
 *                                                                     UINT4 some_indexing_parameter,
 *                                                                     UINT4 some_other_indexing_parameter,
 *                                                                     sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_PATH_MONING_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_PATH_MONING_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SD_EN_set( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SD_EN_set( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SD_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SD_EN_set", value );

    /* (0x00000006 bits 0) field SD_EN of register PMC_SBER_INDIRECT_REG_SD_PATH_MONING_EN */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_MSK & (value << SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SD_EN_get( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SD_EN_get( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000006 bits 0) field SD_EN of register PMC_SBER_INDIRECT_REG_SD_PATH_MONING_EN */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_MSK) >> SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SD_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_SF_BERM_INT_EN
 * ==================================================================================
 */
/*--------------------------------.
 | parameterset SD_SF_BERM_INT_EN |
 |                                |
 | parameters are:                |
 |     SF_E                       |
 |     SD_E                       |
 +-------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_SF_BERM_INT_EN_buffer_t;
static INLINE void sber_indirect_SD_SF_BERM_INT_EN_buffer_init( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_SF_BERM_INT_EN_buffer_init( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_SF_BERM_INT_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                     UINT32 A,
 *                                                                     UINT4 some_indexing_parameter,
 *                                                                     UINT4 some_other_indexing_parameter,
 *                                                                     sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_SF_BERM_INT_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                     UINT32 A,
 *                                                                     UINT4 some_indexing_parameter,
 *                                                                     UINT4 some_other_indexing_parameter,
 *                                                                     sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_SF_BERM_INT_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_SF_BERM_INT_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SF_E_set( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SF_E_set( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SF_E_set", value );

    /* (0x00000001 bits 1) field SF_E of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_EN */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_MSK & (value << SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SF_E_get( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SF_E_get( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000001 bits 1) field SF_E of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_EN */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SF_E_get", value );

    return value;
}
static INLINE void sber_indirect_ifield_SD_E_set( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SD_E_set( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SD_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SD_E_set", value );

    /* (0x00000001 bits 0) field SD_E of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_EN */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_MSK & (value << SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SD_E_get( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SD_E_get( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000001 bits 0) field SD_E of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_EN */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SD_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_SF_BERM_INT_STAT
 * ==================================================================================
 */
/*----------------------------------.
 | parameterset SD_SF_BERM_INT_STAT |
 |                                  |
 | parameters are:                  |
 |     SF_I                         |
 |     SD_I                         |
 +---------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_SF_BERM_INT_STAT_buffer_t;
static INLINE void sber_indirect_SD_SF_BERM_INT_STAT_buffer_init( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_SF_BERM_INT_STAT_buffer_init( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_SF_BERM_INT_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                       UINT32 A,
 *                                                                       UINT4 some_indexing_parameter,
 *                                                                       UINT4 some_other_indexing_parameter,
 *                                                                       sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_SF_BERM_INT_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                       UINT32 A,
 *                                                                       UINT4 some_indexing_parameter,
 *                                                                       UINT4 some_other_indexing_parameter,
 *                                                                       sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_SF_BERM_INT_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                    UINT32 A,
 *                                                                    UINT4 some_indexing_parameter,
 *                                                                    UINT4 some_other_indexing_parameter,
 *                                                                    sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_SF_BERM_INT_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                    UINT32 A,
 *                                                                    UINT4 some_indexing_parameter,
 *                                                                    UINT4 some_other_indexing_parameter,
 *                                                                    sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SF_I_set_to_clear( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SF_I_set_to_clear( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SF_I_set_to_clear", value );

    /* (0x00000002 bits 1) field SF_I of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_MSK & (value << SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SF_I_get( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SF_I_get( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 1) field SF_I of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SF_I_get", value );

    return value;
}
static INLINE void sber_indirect_ifield_SD_I_set_to_clear( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SD_I_set_to_clear( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SD_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SD_I_set_to_clear", value );

    /* (0x00000002 bits 0) field SD_I of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_MSK & (value << SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SD_I_get( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SD_I_get( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 0) field SD_I of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SD_I_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SD_SF_BERM_STAT
 * ==================================================================================
 */
/*------------------------------.
 | parameterset SD_SF_BERM_STAT |
 |                              |
 | parameters are:              |
 |     SFPBS                    |
 |     SDPBS                    |
 |     SFCO                     |
 |     SDCO                     |
 |     SF_V                     |
 |     SD_V                     |
 +-----------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SD_SF_BERM_STAT_buffer_t;
static INLINE void sber_indirect_SD_SF_BERM_STAT_buffer_init( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SD_SF_BERM_STAT_buffer_init( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SD_SF_BERM_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                   UINT32 A,
 *                                                                   UINT4 some_indexing_parameter,
 *                                                                   UINT4 some_other_indexing_parameter,
 *                                                                   sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_SF_BERM_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                   UINT32 A,
 *                                                                   UINT4 some_indexing_parameter,
 *                                                                   UINT4 some_other_indexing_parameter,
 *                                                                   sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SD_SF_BERM_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                UINT32 A,
 *                                                                UINT4 some_indexing_parameter,
 *                                                                UINT4 some_other_indexing_parameter,
 *                                                                sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SD_SF_BERM_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                UINT32 A,
 *                                                                UINT4 some_indexing_parameter,
 *                                                                UINT4 some_other_indexing_parameter,
 *                                                                sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 sber_indirect_ifield_SFPBS_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SFPBS_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field SFPBS of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFPBS_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFPBS_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SFPBS_get", value );

    return value;
}
static INLINE UINT32 sber_indirect_ifield_SDPBS_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SDPBS_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field SDPBS of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDPBS_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDPBS_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SDPBS_get", value );

    return value;
}
static INLINE UINT32 sber_indirect_ifield_SFCO_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SFCO_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field SFCO of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFCO_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFCO_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SFCO_get", value );

    return value;
}
static INLINE UINT32 sber_indirect_ifield_SDCO_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SDCO_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field SDCO of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDCO_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDCO_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SDCO_get", value );

    return value;
}
static INLINE UINT32 sber_indirect_ifield_SF_V_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SF_V_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field SF_V of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SF_V_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SF_V_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SF_V_get", value );

    return value;
}
static INLINE UINT32 sber_indirect_ifield_SD_V_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SD_V_get( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SD_V of register PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SD_V_MSK) >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SD_V_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SD_V_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SF_BERM_BER_TH
 * ==================================================================================
 */
/*-----------------------------.
 | parameterset SF_BERM_BER_TH |
 |                             |
 | parameters are:             |
 |     SFTR                    |
 +----------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SF_BERM_BER_TH_buffer_t;
static INLINE void sber_indirect_SF_BERM_BER_TH_buffer_init( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SF_BERM_BER_TH_buffer_init( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_BER_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_BER_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_BER_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_BER_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SFTR_set( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SFTR_set( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr, UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SFTR_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SFTR_set", value );

    /* (0x00000013 bits 3:0) bits 0:3 use field SFTR of register PMC_SBER_INDIRECT_REG_SF_BERM_BER_TH */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_MSK & (value << SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SFTR_get( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SFTR_get( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000013 bits 3:0) bits 0:3 use field SFTR of register PMC_SBER_INDIRECT_REG_SF_BERM_BER_TH */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_MSK) >> SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SFTR_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SF_BERM_CLEAR_TH
 * ==================================================================================
 */
/*-------------------------------.
 | parameterset SF_BERM_CLEAR_TH |
 |                               |
 | parameters are:               |
 |     SFCT                      |
 +------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SF_BERM_CLEAR_TH_buffer_t;
static INLINE void sber_indirect_SF_BERM_CLEAR_TH_buffer_init( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SF_BERM_CLEAR_TH_buffer_init( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_CLEAR_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                    UINT32 A,
 *                                                                    UINT4 some_indexing_parameter,
 *                                                                    UINT4 some_other_indexing_parameter,
 *                                                                    sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_CLEAR_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                    UINT32 A,
 *                                                                    UINT4 some_indexing_parameter,
 *                                                                    UINT4 some_other_indexing_parameter,
 *                                                                    sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_CLEAR_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                 UINT32 A,
 *                                                                 UINT4 some_indexing_parameter,
 *                                                                 UINT4 some_other_indexing_parameter,
 *                                                                 sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_CLEAR_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                 UINT32 A,
 *                                                                 UINT4 some_indexing_parameter,
 *                                                                 UINT4 some_other_indexing_parameter,
 *                                                                 sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SFCT_set( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SFCT_set( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr, UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SFCT_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SFCT_set", value );

    /* (0x00000015 bits 11:0) bits 0:11 use field SFCT of register PMC_SBER_INDIRECT_REG_SF_BERM_CLEAR_TH */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_MSK & (value << SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SFCT_get( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SFCT_get( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000015 bits 11:0) bits 0:11 use field SFCT of register PMC_SBER_INDIRECT_REG_SF_BERM_CLEAR_TH */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_MSK) >> SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SFCT_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SF_BERM_SET_INTEGRATION_PERIOD_1
 * ==================================================================================
 */
/*-----------------------------------------------.
 | parameterset SF_BERM_SET_INTEGRATION_PERIOD_1 |
 |                                               |
 | parameters are:                               |
 +----------------------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t;
static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_init( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_init( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SF_BERM_SET_INTEGRATION_PERIOD_2
 * ==================================================================================
 */
/*-----------------------------------------------.
 | parameterset SF_BERM_SET_INTEGRATION_PERIOD_2 |
 |                                               |
 | parameters are:                               |
 |     SFIP                                      |
 +----------------------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t;
static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_init( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_init( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SFIP_set( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SFIP_set( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr, UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SFIP_set", value );

    /* (0x00000017 bits 15:0) bits 16:31 use field SFIP of register PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2 */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_MSK & (value << SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_OFF);

    /* (0x00000018 bits 15:0) bits 0:15 use field SFIP of register PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1 */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_MSK & (value << SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SFIP_get( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SFIP_get( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000017 bits 15:0) bits 16:31 use field SFIP of register PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2 */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_MSK) >> SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_OFF;
    value |= field_value;

    /* (0x00000018 bits 15:0) bits 0:15 use field SFIP of register PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1 */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_MSK) >> SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SFIP_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SF_BERM_SET_TH
 * ==================================================================================
 */
/*-----------------------------.
 | parameterset SF_BERM_SET_TH |
 |                             |
 | parameters are:             |
 |     SFST                    |
 +----------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SF_BERM_SET_TH_buffer_t;
static INLINE void sber_indirect_SF_BERM_SET_TH_buffer_init( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SF_BERM_SET_TH_buffer_init( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_SET_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_SET_TH_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SF_BERM_SET_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_BERM_SET_TH_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SFST_set( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SFST_set( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr, UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SFST_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SFST_set", value );

    /* (0x00000014 bits 11:0) bits 0:11 use field SFST of register PMC_SBER_INDIRECT_REG_SF_BERM_SET_TH */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_MSK & (value << SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SFST_get( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SFST_get( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 11:0) bits 0:11 use field SFST of register PMC_SBER_INDIRECT_REG_SF_BERM_SET_TH */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_MSK) >> SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SFST_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset SF_PATH_MONING_EN
 * ==================================================================================
 */
/*--------------------------------.
 | parameterset SF_PATH_MONING_EN |
 |                                |
 | parameters are:                |
 |     SF_EN                      |
 +-------------------------------*/
typedef struct {
    sber_indirect_buffer_t base;
} sber_indirect_SF_PATH_MONING_EN_buffer_t;
static INLINE void sber_indirect_SF_PATH_MONING_EN_buffer_init( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sber_indirect_SF_PATH_MONING_EN_buffer_init( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr )
{
    IOLOG( "%s", "sber_indirect_buffer_init");
    sber_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void sber_indirect_SF_PATH_MONING_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                     UINT32 A,
 *                                                                     UINT4 some_indexing_parameter,
 *                                                                     UINT4 some_other_indexing_parameter,
 *                                                                     sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_PATH_MONING_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                     UINT32 A,
 *                                                                     UINT4 some_indexing_parameter,
 *                                                                     UINT4 some_other_indexing_parameter,
 *                                                                     sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_retrieve", A);
 *     sber_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void sber_indirect_SF_PATH_MONING_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void sber_indirect_SF_PATH_MONING_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "sber_indirect_buffer_apply", A);
 *     sber_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void sber_indirect_ifield_SF_EN_set( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void sber_indirect_ifield_SF_EN_set( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sber_indirect_ifield_SF_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sber_indirect_ifield_SF_EN_set", value );

    /* (0x00000016 bits 0) field SF_EN of register PMC_SBER_INDIRECT_REG_SF_PATH_MONING_EN */
    b_ptr->base.mask[0] |= SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_MSK;
    b_ptr->base.data[0] &= ~SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_MSK;
    b_ptr->base.data[0] |= SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_MSK & (value << SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_OFF);
}

static INLINE UINT32 sber_indirect_ifield_SF_EN_get( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sber_indirect_ifield_SF_EN_get( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000016 bits 0) field SF_EN of register PMC_SBER_INDIRECT_REG_SF_PATH_MONING_EN */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_MSK) >> SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sber_indirect_ifield_SF_EN_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SBER_INDIRECT_IO_INLINE_H */
