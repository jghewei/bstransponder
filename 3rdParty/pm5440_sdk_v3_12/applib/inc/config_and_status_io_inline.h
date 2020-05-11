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
 *     and register accessor functions for the config_and_status block
 *****************************************************************************/
#ifndef _CONFIG_AND_STATUS_IO_INLINE_H
#define _CONFIG_AND_STATUS_IO_INLINE_H

#include "cbrc_api.h"
#include "config_and_status_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CONFIG_AND_STATUS_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for config_and_status
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
 * Parameter Access Functions for Paramset GENERATOR_B1_E1_VAL
 * ==================================================================================
 */
/*----------------------------------.
 | parameterset GENERATOR_B1_E1_VAL |
 |                                  |
 | parameters are:                  |
 |     GEN_B1                       |
 +---------------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_GENERATOR_B1_E1_VAL_buffer_t;
static INLINE void config_and_status_GENERATOR_B1_E1_VAL_buffer_init( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_GENERATOR_B1_E1_VAL_buffer_init( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_B1_E1_VAL_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                           UINT32 A,
 *                                                                           UINT4 some_indexing_parameter,
 *                                                                           UINT4 some_other_indexing_parameter,
 *                                                                           config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_B1_E1_VAL_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                           UINT32 A,
 *                                                                           UINT4 some_indexing_parameter,
 *                                                                           UINT4 some_other_indexing_parameter,
 *                                                                           config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_B1_E1_VAL_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                        UINT32 A,
 *                                                                        UINT4 some_indexing_parameter,
 *                                                                        UINT4 some_other_indexing_parameter,
 *                                                                        config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_B1_E1_VAL_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                        UINT32 A,
 *                                                                        UINT4 some_indexing_parameter,
 *                                                                        UINT4 some_other_indexing_parameter,
 *                                                                        config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void config_and_status_ifield_GEN_B1_set( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_GEN_B1_set( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr, UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_GEN_B1_set", value, 255);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_GEN_B1_set", value );

    /* (0x0000000b bits 7:0) bits 0:7 use field GEN_B1 of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_OFF);
}

static INLINE UINT32 config_and_status_ifield_GEN_B1_get( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_GEN_B1_get( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000b bits 7:0) bits 0:7 use field GEN_B1 of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_GEN_B1_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset GENERATOR_PRBS_22_7_ACCUMULATOR
 * ==================================================================================
 */
/*----------------------------------------------.
 | parameterset GENERATOR_PRBS_22_7_ACCUMULATOR |
 |                                              |
 | parameters are:                              |
 |     GEN_PRBS                                 |
 +---------------------------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t;
static INLINE void config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_init( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_init( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                       UINT32 A,
 *                                                                                       UINT4 some_indexing_parameter,
 *                                                                                       UINT4 some_other_indexing_parameter,
 *                                                                                       config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                       UINT32 A,
 *                                                                                       UINT4 some_indexing_parameter,
 *                                                                                       UINT4 some_other_indexing_parameter,
 *                                                                                       config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                    UINT32 A,
 *                                                                                    UINT4 some_indexing_parameter,
 *                                                                                    UINT4 some_other_indexing_parameter,
 *                                                                                    config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void config_and_status_ifield_GEN_PRBS_set( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_GEN_PRBS_set( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr, UINT32 value )
{
    if (value > 8388607)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_GEN_PRBS_set", value, 8388607);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_GEN_PRBS_set", value );

    /* (0x00000009 bits 15:0) bits 7:22 use field GEN_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_OFF);

    /* (0x0000000a bits 6:0) bits 0:6 use field GEN_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_OFF);
}

static INLINE UINT32 config_and_status_ifield_GEN_PRBS_get( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_GEN_PRBS_get( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000009 bits 15:0) bits 7:22 use field GEN_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_OFF;
    value |= field_value;

    /* (0x0000000a bits 6:0) bits 0:6 use field GEN_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_GEN_PRBS_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset GENERATOR_PRBS_6_0_ACCUMULATOR
 * ==================================================================================
 */
/*---------------------------------------------.
 | parameterset GENERATOR_PRBS_6_0_ACCUMULATOR |
 |                                             |
 | parameters are:                             |
 +--------------------------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t;
static INLINE void config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_init( config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_init( config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                      UINT32 A,
 *                                                                                      UINT4 some_indexing_parameter,
 *                                                                                      UINT4 some_other_indexing_parameter,
 *                                                                                      config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                      UINT32 A,
 *                                                                                      UINT4 some_indexing_parameter,
 *                                                                                      UINT4 some_other_indexing_parameter,
 *                                                                                      config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                   UINT32 A,
 *                                                                                   UINT4 some_indexing_parameter,
 *                                                                                   UINT4 some_other_indexing_parameter,
 *                                                                                   config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                                   UINT32 A,
 *                                                                                   UINT4 some_indexing_parameter,
 *                                                                                   UINT4 some_other_indexing_parameter,
 *                                                                                   config_and_status_GENERATOR_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset GENERATOR_STS_1_PATH_CFG
 * ==================================================================================
 */
/*---------------------------------------.
 | parameterset GENERATOR_STS_1_PATH_CFG |
 |                                       |
 | parameters are:                       |
 |     LINKENA0                          |
 |     GEN_GPO                           |
 |     S                                 |
 |     GEN_B1E1_ENA                      |
 |     FORCE_ERR                         |
 |     GEN_INV_PRBS                      |
 |     GEN_AMODE                         |
 +--------------------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t;
static INLINE void config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_init( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_init( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                UINT32 A,
 *                                                                                UINT4 some_indexing_parameter,
 *                                                                                UINT4 some_other_indexing_parameter,
 *                                                                                config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                UINT32 A,
 *                                                                                UINT4 some_indexing_parameter,
 *                                                                                UINT4 some_other_indexing_parameter,
 *                                                                                config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                             UINT32 A,
 *                                                                             UINT4 some_indexing_parameter,
 *                                                                             UINT4 some_other_indexing_parameter,
 *                                                                             config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                             UINT32 A,
 *                                                                             UINT4 some_indexing_parameter,
 *                                                                             UINT4 some_other_indexing_parameter,
 *                                                                             config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void config_and_status_ifield_LINKENA0_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_LINKENA0_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_LINKENA0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_LINKENA0_set", value );

    /* (0x00000008 bits 12) field LINKENA0 of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_OFF);
}

static INLINE UINT32 config_and_status_ifield_LINKENA0_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_LINKENA0_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 12) field LINKENA0 of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_LINKENA0_get", value );

    return value;
}
static INLINE void config_and_status_ifield_GEN_GPO_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_GEN_GPO_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_GEN_GPO_set", value, 3);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_GEN_GPO_set", value );

    /* (0x00000008 bits 9:8) bits 0:1 use field GEN_GPO of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_OFF);
}

static INLINE UINT32 config_and_status_ifield_GEN_GPO_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_GEN_GPO_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 9:8) bits 0:1 use field GEN_GPO of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_GEN_GPO_get", value );

    return value;
}
static INLINE void config_and_status_ifield_S_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_S_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_S_set", value, 3);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_S_set", value );

    /* (0x00000008 bits 7:6) bits 0:1 use field S of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_OFF);
}

static INLINE UINT32 config_and_status_ifield_S_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_S_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 7:6) bits 0:1 use field S of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_S_get", value );

    return value;
}
static INLINE void config_and_status_ifield_GEN_B1E1_ENA_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_GEN_B1E1_ENA_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_GEN_B1E1_ENA_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_GEN_B1E1_ENA_set", value );

    /* (0x00000008 bits 4) field GEN_B1E1_ENA of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_OFF);
}

static INLINE UINT32 config_and_status_ifield_GEN_B1E1_ENA_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_GEN_B1E1_ENA_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 4) field GEN_B1E1_ENA of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_GEN_B1E1_ENA_get", value );

    return value;
}
static INLINE void config_and_status_ifield_FORCE_ERR_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_FORCE_ERR_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_FORCE_ERR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_FORCE_ERR_set", value );

    /* (0x00000008 bits 3) field FORCE_ERR of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_FORCE_ERR_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_FORCE_ERR_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_FORCE_ERR_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_FORCE_ERR_OFF);
}

static INLINE void config_and_status_ifield_GEN_INV_PRBS_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_GEN_INV_PRBS_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_GEN_INV_PRBS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_GEN_INV_PRBS_set", value );

    /* (0x00000008 bits 1) field GEN_INV_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_OFF);
}

static INLINE UINT32 config_and_status_ifield_GEN_INV_PRBS_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_GEN_INV_PRBS_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 1) field GEN_INV_PRBS of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_GEN_INV_PRBS_get", value );

    return value;
}
static INLINE void config_and_status_ifield_GEN_AMODE_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_GEN_AMODE_set( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_GEN_AMODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_GEN_AMODE_set", value );

    /* (0x00000008 bits 0) field GEN_AMODE of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_MSK & (value << CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_OFF);
}

static INLINE UINT32 config_and_status_ifield_GEN_AMODE_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_GEN_AMODE_get( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field GEN_AMODE of register PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_MSK) >> CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_GEN_AMODE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset MON_ERR_CNT
 * ==================================================================================
 */
/*--------------------------.
 | parameterset MON_ERR_CNT |
 |                          |
 | parameters are:          |
 |     ERR_CNT              |
 +-------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_MON_ERR_CNT_buffer_t;
static INLINE void config_and_status_MON_ERR_CNT_buffer_init( config_and_status_MON_ERR_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_MON_ERR_CNT_buffer_init( config_and_status_MON_ERR_CNT_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_MON_ERR_CNT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                   UINT32 A,
 *                                                                   UINT4 some_indexing_parameter,
 *                                                                   UINT4 some_other_indexing_parameter,
 *                                                                   config_and_status_MON_ERR_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_ERR_CNT_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                   UINT32 A,
 *                                                                   UINT4 some_indexing_parameter,
 *                                                                   UINT4 some_other_indexing_parameter,
 *                                                                   config_and_status_MON_ERR_CNT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_MON_ERR_CNT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                UINT32 A,
 *                                                                UINT4 some_indexing_parameter,
 *                                                                UINT4 some_other_indexing_parameter,
 *                                                                config_and_status_MON_ERR_CNT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_ERR_CNT_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                UINT32 A,
 *                                                                UINT4 some_indexing_parameter,
 *                                                                UINT4 some_other_indexing_parameter,
 *                                                                config_and_status_MON_ERR_CNT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 config_and_status_ifield_ERR_CNT_get( config_and_status_MON_ERR_CNT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_ERR_CNT_get( config_and_status_MON_ERR_CNT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 15:0) bits 0:15 use field ERR_CNT of register PMC_CONFIG_AND_STATUS_REG_MON_ERR_CNT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_MON_ERR_CNT_BIT_ERR_CNT_MSK) >> CONFIG_AND_STATUS_REG_MON_ERR_CNT_BIT_ERR_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_ERR_CNT_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset MON_PRBS_22_7_ACCUMULATOR
 * ==================================================================================
 */
/*----------------------------------------.
 | parameterset MON_PRBS_22_7_ACCUMULATOR |
 |                                        |
 | parameters are:                        |
 |     MON_PRBS                           |
 +---------------------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t;
static INLINE void config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_init( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_init( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                 UINT32 A,
 *                                                                                 UINT4 some_indexing_parameter,
 *                                                                                 UINT4 some_other_indexing_parameter,
 *                                                                                 config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                              UINT32 A,
 *                                                                              UINT4 some_indexing_parameter,
 *                                                                              UINT4 some_other_indexing_parameter,
 *                                                                              config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                              UINT32 A,
 *                                                                              UINT4 some_indexing_parameter,
 *                                                                              UINT4 some_other_indexing_parameter,
 *                                                                              config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void config_and_status_ifield_MON_PRBS_set( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_MON_PRBS_set( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr, UINT32 value )
{
    if (value > 8388607)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_MON_PRBS_set", value, 8388607);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_MON_PRBS_set", value );

    /* (0x00000001 bits 15:0) bits 7:22 use field MON_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_MSK & (value << CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_OFF);

    /* (0x00000002 bits 6:0) bits 0:6 use field MON_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_MSK & (value << CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_OFF);
}

static INLINE UINT32 config_and_status_ifield_MON_PRBS_get( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_MON_PRBS_get( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    /* (0x00000001 bits 15:0) bits 7:22 use field MON_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_MSK) >> CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_OFF;
    value |= field_value;

    /* (0x00000002 bits 6:0) bits 0:6 use field MON_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR */
    reg_value = b_ptr->base.data[0];
    field_value = (reg_value & CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_MSK) >> CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_OFF;
    value |= field_value;

    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_MON_PRBS_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset MON_PRBS_6_0_ACCUMULATOR
 * ==================================================================================
 */
/*---------------------------------------.
 | parameterset MON_PRBS_6_0_ACCUMULATOR |
 |                                       |
 | parameters are:                       |
 +--------------------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t;
static INLINE void config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_init( config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_init( config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                UINT32 A,
 *                                                                                UINT4 some_indexing_parameter,
 *                                                                                UINT4 some_other_indexing_parameter,
 *                                                                                config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                                UINT32 A,
 *                                                                                UINT4 some_indexing_parameter,
 *                                                                                UINT4 some_other_indexing_parameter,
 *                                                                                config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                             UINT32 A,
 *                                                                             UINT4 some_indexing_parameter,
 *                                                                             UINT4 some_other_indexing_parameter,
 *                                                                             config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                             UINT32 A,
 *                                                                             UINT4 some_indexing_parameter,
 *                                                                             UINT4 some_other_indexing_parameter,
 *                                                                             config_and_status_MON_PRBS_6_0_ACCUMULATOR_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset MON_STS_1_PATH_CFG
 * ==================================================================================
 */
/*---------------------------------.
 | parameterset MON_STS_1_PATH_CFG |
 |                                 |
 | parameters are:                 |
 |     RESYNC                      |
 |     MON_INV_PRBS                |
 |     MON_ENA                     |
 +--------------------------------*/
typedef struct {
    config_and_status_buffer_t base;
} config_and_status_MON_STS_1_PATH_CFG_buffer_t;
static INLINE void config_and_status_MON_STS_1_PATH_CFG_buffer_init( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void config_and_status_MON_STS_1_PATH_CFG_buffer_init( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    IOLOG( "%s", "config_and_status_buffer_init");
    config_and_status_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void config_and_status_MON_STS_1_PATH_CFG_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                          UINT32 A,
 *                                                                          UINT4 some_indexing_parameter,
 *                                                                          UINT4 some_other_indexing_parameter,
 *                                                                          config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_STS_1_PATH_CFG_buffer_retrieve( cbrc_handle_t *h_ptr,
 *                                                                          UINT32 A,
 *                                                                          UINT4 some_indexing_parameter,
 *                                                                          UINT4 some_other_indexing_parameter,
 *                                                                          config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_retrieve", A);
 *     config_and_status_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void config_and_status_MON_STS_1_PATH_CFG_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                       UINT32 A,
 *                                                                       UINT4 some_indexing_parameter,
 *                                                                       UINT4 some_other_indexing_parameter,
 *                                                                       config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void config_and_status_MON_STS_1_PATH_CFG_buffer_apply( cbrc_handle_t *h_ptr,
 *                                                                       UINT32 A,
 *                                                                       UINT4 some_indexing_parameter,
 *                                                                       UINT4 some_other_indexing_parameter,
 *                                                                       config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "config_and_status_buffer_apply", A);
 *     config_and_status_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void config_and_status_ifield_RESYNC_set( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_RESYNC_set( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_RESYNC_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_RESYNC_set", value );

    /* (0x00000000 bits 3) field RESYNC of register PMC_CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_RESYNC_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_RESYNC_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_RESYNC_MSK & (value << CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_RESYNC_OFF);
}

static INLINE void config_and_status_ifield_MON_INV_PRBS_set( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_MON_INV_PRBS_set( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_MON_INV_PRBS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_MON_INV_PRBS_set", value );

    /* (0x00000000 bits 2) field MON_INV_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_MSK & (value << CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_OFF);
}

static INLINE UINT32 config_and_status_ifield_MON_INV_PRBS_get( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_MON_INV_PRBS_get( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field MON_INV_PRBS of register PMC_CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_MSK) >> CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_MON_INV_PRBS_get", value );

    return value;
}
static INLINE void config_and_status_ifield_MON_ENA_set( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void config_and_status_ifield_MON_ENA_set( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "config_and_status_ifield_MON_ENA_set", value, 1);
    IOLOG( "%s <= 0x%08x", "config_and_status_ifield_MON_ENA_set", value );

    /* (0x00000000 bits 0) field MON_ENA of register PMC_CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG */
    b_ptr->base.mask[0] |= CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_MSK;
    b_ptr->base.data[0] &= ~CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_MSK;
    b_ptr->base.data[0] |= CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_MSK & (value << CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_OFF);
}

static INLINE UINT32 config_and_status_ifield_MON_ENA_get( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 config_and_status_ifield_MON_ENA_get( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field MON_ENA of register PMC_CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_MSK) >> CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_OFF;
    IOLOG( "%s -> 0x%08x", "config_and_status_ifield_MON_ENA_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CONFIG_AND_STATUS_IO_INLINE_H */
