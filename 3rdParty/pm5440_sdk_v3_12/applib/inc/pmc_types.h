/********************************************************************************
*  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
* --------------------------------------------------------------------------
*  DESCRIPTION:
*     Common types for PMC devices
*
*  NOTES:
*     None.
*
********************************************************************************/

#ifndef PMC_TYPES_H
#define PMC_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef unsigned long long UINT64;  /*!< 64 bit unsigned type */
typedef unsigned int       UINT32;  /*!< 32 bit unsigned type */
typedef unsigned short     UINT16;  /*!< 16 bit unsigned type */
typedef unsigned char      UINT8;   /*!< 8 bit unsigned type */
typedef unsigned long      PMC_UINTPTR; /*!< correct bit width for 32 or 64 bit processors to store a pointer as an unsigned */
typedef signed long int    PMC_PTRDIFF; /*!< type to store difference between pointers */

typedef signed long long INT64; /*!< 64 bit signed type */
typedef signed int       INT32; /*!< 32 bit signed type */
typedef signed short     INT16; /*!< 16 bit signed type */
typedef signed char      INT8;  /*!< 8 bit signed type */ 

typedef float      FLOAT;  /*!< 32 bit floating point type */
typedef double     DOUBLE; /*!< 64 bit floating point type */

#define MAX_UINT32 (UINT32)((((UINT64)(1)) << (sizeof(UINT32) * 8))-1 ) /* the maximum value of UINT32*/

/* The following can be used to compare DOUBLE values where the representation may not be exact
   so it is appropriate to compare against range of [comp-precision,comp+precision].  For example
   PMC_COMPARE_DOUBLE(my_var, 1.5, 0.1) would result in:
   (1.4 < my_var) && (my_var < 1.6) */
#define PMC_COMPARE_DOUBLE(value, comp, precision)   ((((DOUBLE)comp - (DOUBLE)precision) < (DOUBLE)value) && ((DOUBLE)value < ((DOUBLE)comp + (DOUBLE)precision)))
#define PMC_COMPARE_FLOAT(value, comp, precision)   ((((FLOAT)comp - (FLOAT)precision) < (FLOAT)value) && ((FLOAT)value < ((FLOAT)comp + (FLOAT)precision)))

typedef unsigned char BOOL8; /*!< short boolean type to save memory */

#if !defined(BOOL)
#if defined(__cplusplus)
typedef bool             BOOL;       /*!< Boolean quantity */
#else
typedef int              BOOL;       /*!< Boolean quantity */
#endif /* !__cplusplus */
#endif /* !BOOL */

#if !defined(PMC_POLL_RETURN_TYPE)
/*! return type of pmc_sys_poll() can be changed to suit the software environment. */
typedef UINT32 PMC_POLL_RETURN_TYPE;
#endif /* !PMC_POLL_RETURN_TYPE */

/*******************************************************************************
* ENUM: PMC_POLL_COMPARISON_TYPE
* ______________________________________________________________________________
*
* DESCRIPTION:
*   When polling for comparison this enum specifies when the poll is to complete.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum PMC_POLL_COMPARISON_TYPE 
{
  PMC_POLL_EQUAL,
  PMC_POLL_NOT_EQUAL,
  PMC_POLL_GREATER,
  PMC_POLL_LESS,
  PMC_POLL_GREATER_OR_EQUAL,
  PMC_POLL_LESS_OR_EQUAL,
} PMC_POLL_COMPARISON_TYPE;

#if !defined(TRUE)
#if defined(__cplusplus)
#define TRUE        true
#else
#define TRUE        (1 == 1)
#endif /* !__cplusplus */
#endif /* !TRUE */

#if !defined(FALSE)
#if defined(__cplusplus)
#define FALSE       false
#else
#define FALSE       (1 == 0)
#endif /* !__cplusplus */
#endif /* !FALSE */

#if !defined(NULL)
#define NULL        0L
#endif /* !NULL */

#if !defined(PUBLIC)
#define PUBLIC
#endif /* !PUBLIC */

#if !defined(PRIVATE)
#define PRIVATE static
#endif /* !PRIVATE */

#if !defined(INLINE)
#define INLINE __inline
#endif /* !INLINE */

#if !defined(PMC_VAR_CONST)
# ifdef __cplusplus
#  define PMC_VAR_CONST 
# else
#  ifdef INSIDE_SWIG_COMPILE
#   define PMC_VAR_CONST 
#  else
#   define PMC_VAR_CONST const
#  endif /* INSIDE_SWIG_COMPILE */
# endif /* __cplusplus */
#endif /* !PMC_VAR_CONST */

/*******************************************************************************
* STRUCTURE: pmc_mem_pool_handle
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque type
*
* ELEMENTS:
*   Hidden
*******************************************************************************/
typedef void *pmc_mem_pool_handle;

/*******************************************************************************
* STRUCTURE: pmc_mem_align_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque type
*
* ELEMENTS:
*   Hidden
*******************************************************************************/
typedef void *pmc_mem_align_enum;

/*******************************************************************************
* ENUM: pmc_energy_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type to track the current energy state of a block.
*
* ELEMENTS:
*   PMC_ENERGY_STATE_RESET    - Block is held in reset and low power
*   PMC_ENERGY_STATE_RUNNING  - Block is in an incoherent state
*   PMC_ENERGY_STATE_INCOHERENT - Block reset/lowpwr bits not RUNNING, but also
*                                 not full RESET.
*******************************************************************************/
typedef enum {
    PMC_ENERGY_STATE_RESET   = 0,
    PMC_ENERGY_STATE_RUNNING = 1,
    PMC_ENERGY_STATE_INCOHERENT = 2
} pmc_energy_state_t; 

/*******************************************************************************
* ENUM: pmc_energy_state_request_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type to track the requested energy state of a block.
*
* ELEMENTS:
*   PMC_ENERGY_STATE_REQUEST_RESET    - Block is to be forced to reset
*   PMC_ENERGY_STATE_REQUEST_RUNNING  - Block is to be put in running state or 
*                                       maintain running state
*   PMC_ENERGY_STATE_REQUEST_OPTIMAL  - Block to be put in optimal (typically 
*                                       lowest) energy state for current 
*                                       configured operations 
*******************************************************************************/
typedef enum {
    PMC_ENERGY_STATE_REQUEST_RESET   = PMC_ENERGY_STATE_RESET,
    PMC_ENERGY_STATE_REQUEST_RUNNING = PMC_ENERGY_STATE_RUNNING,
    PMC_ENERGY_STATE_REQUEST_OPTIMAL
} pmc_energy_state_request_t;

/*******************************************************************************
* ENUM: pmc_block_energy_state_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type to track the energy state of a block.
*
* ELEMENTS:
*   PMC_BLOCK_LOWPWR_ON_ALTERABLE  - Block is out of reset and in power saving mode
*   PMC_BLOCK_LOWPWR_ON_DEFAULT    - Block is held in reset
*   PMC_BLOCK_LOWPWR_OFF_ALTERABLE - Block is out of reset and ready for operation
*   PMC_BLOCK_LOWPWR_OFF_DEFAULT   - Block is in useless state; not configurable
*******************************************************************************/
typedef enum {
    PMC_BLOCK_LOWPWR_ON_ALTERABLE   = 0,
    PMC_BLOCK_LOWPWR_ON_DEFAULT     = 1,
    PMC_BLOCK_LOWPWR_OFF_ALTERABLE  = 2,
    PMC_BLOCK_LOWPWR_OFF_DEFAULT    = 3,
} pmc_block_energy_state_enum; 

/*******************************************************************************
* ENUM: pmc_block_reg_access_state_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type to track the energy state of a block which has only a reset
*   bit.
*
* ELEMENTS:
*   PMC_BLOCK_ALTERABLE - Block is out of reset and ready for configuration
*   PMC_BLOCK_DEFAULT   - Block is held in reset and registers are non-modifyable
*******************************************************************************/
typedef enum {
    PMC_BLOCK_ALTERABLE   = 0,    /* Block is out of reset and ready for configuration */
    PMC_BLOCK_DEFAULT     = 1,    /* Block is held in reset and registers are non-modifyable */
} pmc_block_reg_access_state_enum;

/*******************************************************************************
* STRUCTURE: pmc_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque type definition for the system access handle. Customer creates
*   their own structure with whatever fields are required. For example a
*   mmmap() handle.
*******************************************************************************/
typedef void *pmc_sys_handle_t;

/***************************************************************************
* MACRO: PMC_ENDIAN_SWAP_*
* __________________________________________________________________________
*
* DESCRIPTION:
*   Endian-swap for 16, 32, or 64 bytes.
*
* INPUTS:
*   x - value to be swapped.
*
* RETURNS:
*   Bytes in x are swapped endian-wise.
*
****************************************************************************/
#define PMC_ENDIAN_SWAP_16(x) \
            (((((UINT16) x) & 0x00FF) << 8) | \
             ((((UINT16) x) & 0xFF00) >> 8))

#define PMC_ENDIAN_SWAP_32(x) \
            (((((UINT32) x) & 0x000000FF) << 24) | \
             ((((UINT32) x) & 0x0000FF00) <<  8) | \
             ((((UINT32) x) & 0x00FF0000) >>  8) | \
             ((((UINT32) x) & 0xFF000000) >> 24))

#define PMC_ENDIAN_SWAP_64(x) \
            (((((UINT64) x) & 0x00000000000000FFULL) << 56) | \
             ((((UINT64) x) & 0x000000000000FF00ULL) << 40) | \
             ((((UINT64) x) & 0x0000000000FF0000ULL) << 24) | \
             ((((UINT64) x) & 0x00000000FF000000ULL) <<  8) | \
             ((((UINT64) x) & 0x000000FF00000000ULL) >>  8) | \
             ((((UINT64) x) & 0x0000FF0000000000ULL) >> 24) | \
             ((((UINT64) x) & 0x00FF000000000000ULL) >> 40) | \
             ((((UINT64) x) & 0xFF00000000000000ULL) >> 56))


/***************************************************************************
* MACRO: PMC_CPU_TO_LE_*
*        PMC_LE_TO_CPU_*
*        PMC_CPU_TO_BE_*
*        PMC_BE_TO_CPU_*
* __________________________________________________________________________
*
* DESCRIPTION:
*   Coverting host CPU to/from little/big endian in 16, 32, or 64 bytes.
*
* INPUTS:
*   x - value to be converted.
*
* RETURNS:
*   x in little/big endian.
*
****************************************************************************/
#if defined(PMC_BIG_ENDIAN) 
/* BIG ENDIAN definitions */
#define PMC_CPU_TO_LE_16(x) PMC_ENDIAN_SWAP_16(x)   /* Writing 16bit value from BE CPU to LE target */
#define PMC_CPU_TO_LE_32(x) PMC_ENDIAN_SWAP_32(x)   /* Writing 32bit value from BE CPU to LE target */
#define PMC_CPU_TO_LE_64(x) PMC_ENDIAN_SWAP_64(x)   /* Writing 64bit value from BE CPU to LE target */

#define PMC_LE_TO_CPU_16(x) PMC_ENDIAN_SWAP_16(x)   /* Reading 16bit value from LE target to BE CPU */
#define PMC_LE_TO_CPU_32(x) PMC_ENDIAN_SWAP_32(x)   /* Reading 32bit value from LE target to BE CPU */
#define PMC_LE_TO_CPU_64(x) PMC_ENDIAN_SWAP_64(x)   /* Reading 64bit value from LE target to BE CPU */

#define PMC_CPU_TO_BE_16(x) (x)   /* Writing 16bit value from BE CPU to BE target */
#define PMC_CPU_TO_BE_32(x) (x)   /* Writing 32bit value from BE CPU to BE target */
#define PMC_CPU_TO_BE_64(x) (x)   /* Writing 64bit value from BE CPU to BE target */
                                                                                    
#define PMC_BE_TO_CPU_16(x) (x)   /* Reading 16bit value from BE target to BE CPU*/ 
#define PMC_BE_TO_CPU_32(x) (x)   /* Reading 32bit value from BE target to BE CPU */
#define PMC_BE_TO_CPU_64(x) (x)   /* Reading 64bit value from BE target to BE CPU */

#else 
/* LITTLE ENDIAN definitions */

#define PMC_CPU_TO_LE_16(x) (x)   /* Writing 16bit value from LE CPU to LE target */
#define PMC_CPU_TO_LE_32(x) (x)   /* Writing 32bit value from LE CPU to LE target */
#define PMC_CPU_TO_LE_64(x) (x)   /* Writing 64bit value from LE CPU to LE target */
                                                                                    
#define PMC_LE_TO_CPU_16(x) (x)   /* Reading 16bit value from LE target to LE CPU */
#define PMC_LE_TO_CPU_32(x) (x)   /* Reading 32bit value from LE target to LE CPU */
#define PMC_LE_TO_CPU_64(x) (x)   /* Reading 64bit value from LE target to LE CPU */

#define PMC_CPU_TO_BE_16(x) PMC_ENDIAN_SWAP_16(x)   /* Writing 16bit value from LE CPU to BE target */
#define PMC_CPU_TO_BE_32(x) PMC_ENDIAN_SWAP_32(x)   /* Writing 32bit value from LE CPU to BE target */
#define PMC_CPU_TO_BE_64(x) PMC_ENDIAN_SWAP_64(x)   /* Writing 64bit value from LE CPU to BE target */
                                                                                                     
#define PMC_BE_TO_CPU_16(x) PMC_ENDIAN_SWAP_16(x)   /* Reading 16bit value from LE target to BE CPU*/ 
#define PMC_BE_TO_CPU_32(x) PMC_ENDIAN_SWAP_32(x)   /* Reading 32bit value from LE target to BE CPU */
#define PMC_BE_TO_CPU_64(x) PMC_ENDIAN_SWAP_64(x)   /* Reading 64bit value from LE target to BE CPU */

#endif /* PMC_BIG_ENDIAN */

/* The following definitions are use to define the bit order
   of bitfields - either MSB first of LSB first.  The default
   of the code is LSB first. To change to MSB first the following
   should be defined on the build line to the compiler:
      -DPMC_BITFIELD_MSB_FIRST
 
   Initial releases supported DIGI_PMON_BITFIELD_MSB_FIRST and
   it is still supported and when defined the following will
   also ensure that PMC_BITFIELD_MSB_FIRST is defined */

/* Provide backwards compatibility - first if DIGI_PMON_BITFIELD_MSB_FIRST
   is defined then define PMC_BITFIELD_MSB_FIRST if needed */
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
#ifndef PMC_BITFIELD_MSB_FIRST
#define PMC_BITFIELD_MSB_FIRST 
#endif
#endif
/* If PMC_BITFIELD_MSB_FIRST is defined then define
   the previous DIGI_PMON_BITFIELD_MSB_FIRST for compatibility */
#ifdef PMC_BITFIELD_MSB_FIRST
#ifndef DIGI_PMON_BITFIELD_MSB_FIRST
#define DIGI_PMON_BITFIELD_MSB_FIRST 
#endif
#endif

/*-------------- compiler attributes ----------------------*/
#ifdef __GNUC__
#define __gnuattribute__(x) __attribute__(x)    
#else
#define __gnuattribute__(x)
#endif

/*
 * ATTRIBUTE_NORETURN 
 *  
 * function does not return (calls exit()) 
 */
#define ATTRIBUTE_NORETURN __gnuattribute__((__noreturn__))

/*
 * ALWAYS_INLINE 
 *  
 * force the function to be inline'd 
 */
#ifndef ALWAYS_INLINE
  #ifndef LOGGING_DISABLED
    /* if logging is enabled then inlining IO function might result in code-bloat. */
    #define ALWAYS_INLINE
  #else
    #define ALWAYS_INLINE __attribute__((__always_inline__))
  #endif
#endif

/*
 * ATTRIBUTE_COLD
 *
 * function should be optimized for size rather than speed
 */
/* Test for GCC >= 4.3 */
#if (__GNUC__ > 4) || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 6))
#define ATTRIBUTE_COLD __gnuattribute__((__cold__))
#else
#define ATTRIBUTE_COLD 
#endif

/*
 * ATTRIBUTE_HOT
 *
 *   function should be aggressively optimized for speed
 */
/* Test for GCC >= 4.3 */
#if (__GNUC__ > 4) || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 6))
#define ATTRIBUTE_HOT  __gnuattribute__((__hot__))
#else
#define ATTRIBUTE_HOT 
#endif

/* don't generate a warning message if the function is not used.
 * for example static inline functions in header files that are similar to macros. */
#define ATTRIBUTE_UNUSED  __gnuattribute__((__unused__))

/*
 * ATTRIBUTE_WARN_UNUSED_RESULT
 *
 *   generate a warning message if the result of a function is not used.
 *   for example, malloc() should only be called if the result is used.
 *
 * example:
 *   void malloc( int size ) ATTRIBUTE_WARN_UNUSED_RESULT;
 *
 * if called like:
 *   malloc(10);
 *
 * generates the warning:
 *   file.c:336:4: warning: ignoring return value of 'malloc', declared with attribute warn_unused_result
 */
#ifdef __linux__
#define ATTRIBUTE_WARN_UNUSED_RESULT  __gnuattribute__((__warn_unused_result__))
#else
#define ATTRIBUTE_WARN_UNUSED_RESULT
#endif

/*
 * ATTRIBUTE_PRINTF()
 *   indicates that printf() style aruguments are being used:
 *   argument m is the printf() format string and
 *   argument n is the start of the var args list
 *
 *   this causes the gcc compiler to generate warnings if incompatible arguments
 *   are passed in the var args list.
 *
 * example:
 *   void vaLog( const char *fmt, ... ) ATTRIBUTE_PRINTF(1,2); -- attribute_printf() allowed in porting function
 *
 * if called with incompatible arguments as in the following:
 *   vaLog( "hello %s", 10 );   
 *
 * generates the following error message:
 *   file.c: In function 'some_function':
 *   file.c:48:5: warning: format '%s' expects type 'char *', but argument 2 has type 'int'
 */
#define ATTRIBUTE_PRINTF(m,n)  __gnuattribute__((__format__(printf,m,n))) /* attribute_printf() allowed in porting function */



/*-------------- compiler warning overrides ----------------------*/
#ifdef WIN32
#ifndef __GNUC__
#pragma warning (disable : 4127) /* conditional expression is constant */ /* (otherwise every do {} while(0) generates a warning) */
#endif
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PMC_TYPES_H */
