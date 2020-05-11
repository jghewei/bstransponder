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
 *     Contains all register offset and register bit definitions for the
 *     int_agr block
 *****************************************************************************/
#ifndef _INT_AGR_REGS_H
#define _INT_AGR_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET      0x000010c8
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR    0x000010cc
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN       0x000010d0
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT     0x000010d4
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT 0x000010d8
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET        0x000010dc
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR      0x000010e0
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_EN         0x000010e4
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_STAT       0x000010e8
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT   0x000010ec
#define PMC_INT_AGR_REG_INTER_PROCESSOR_INTS( N )      (0x00001230 + (N) * 4)
#define PMC_INT_AGR_REG_IPI2PCIE_0_INT                 0x00001330
#define PMC_INT_AGR_REG_IPI2PCIE_1_INT                 0x00001334
#define PMC_INT_AGR_REG_IPI2PCIE_2_INT                 0x00001338
#define PMC_INT_AGR_REG_IPI2PCIE_3_INT                 0x0000133c
#define PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN              0x00001340
#define PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN              0x00001344
#define PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN              0x00001348
#define PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN              0x0000134c
#define PMC_INT_AGR_REG_IPI2MIPS_0_INT                 0x00001350
#define PMC_INT_AGR_REG_IPI2MIPS_1_INT                 0x00001354
#define PMC_INT_AGR_REG_IPI2MIPS_2_INT                 0x00001358
#define PMC_INT_AGR_REG_IPI2MIPS_3_INT                 0x0000135c
#define PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN              0x00001360
#define PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN              0x00001364
#define PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN              0x00001368
#define PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN              0x0000136c

/*-----------------------------------------------.
 | Register 0x000010c8 INT_AGGREGATOR_DEVICE_SET |
 +-----------------------------------------------+
 | bit  31:0 R/W  DEVICE_SET                     |
 +----------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_UNUSED_MASK    0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_SET_BIT_DEVICE_SET_OFF 0

/*-------------------------------------------------.
 | Register 0x000010cc INT_AGGREGATOR_DEVICE_CLEAR |
 +-------------------------------------------------+
 | bit  31:0 R/W  DEVICE_CLR                       |
 +------------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_UNUSED_MASK    0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_CLEAR_BIT_DEVICE_CLR_OFF 0

/*----------------------------------------------.
 | Register 0x000010d0 INT_AGGREGATOR_DEVICE_EN |
 +----------------------------------------------+
 | bit  31:0 R  DEVICE_EN                       |
 +---------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_UNUSED_MASK   0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_EN_BIT_DEVICE_EN_OFF 0

/*------------------------------------------------.
 | Register 0x000010d4 INT_AGGREGATOR_DEVICE_STAT |
 +------------------------------------------------+
 | bit  31:0 R  DEVICE_STAT                       |
 +-----------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_UNUSED_MASK     0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_STAT_BIT_DEVICE_STAT_OFF 0

/*----------------------------------------------------.
 | Register 0x000010d8 INT_AGGREGATOR_DEVICE_RAW_STAT |
 +----------------------------------------------------+
 | bit  31:0 R  DEVICE_RSTAT                          |
 +---------------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_UNUSED_MASK      0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_DEVICE_RAW_STAT_BIT_DEVICE_RSTAT_OFF 0

/*---------------------------------------------.
 | Register 0x000010dc INT_AGGREGATOR_MISC_SET |
 +---------------------------------------------+
 | bit  31:0 R/W  MISC_SET                     |
 +--------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_SET_UNUSED_MASK  0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_MISC_SET_BIT_MISC_SET_OFF 0

/*-----------------------------------------------.
 | Register 0x000010e0 INT_AGGREGATOR_MISC_CLEAR |
 +-----------------------------------------------+
 | bit  31:0 R/W  MISC_CLR                       |
 +----------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_UNUSED_MASK  0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_MISC_CLEAR_BIT_MISC_CLR_OFF 0

/*--------------------------------------------.
 | Register 0x000010e4 INT_AGGREGATOR_MISC_EN |
 +--------------------------------------------+
 | bit  31:0 R  MISC_EN                       |
 +-------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_EN_UNUSED_MASK 0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_MISC_EN_BIT_MISC_EN_OFF 0

/*----------------------------------------------.
 | Register 0x000010e8 INT_AGGREGATOR_MISC_STAT |
 +----------------------------------------------+
 | bit  31:0 R  MISC_STAT                       |
 +---------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_UNUSED_MASK   0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_MISC_STAT_BIT_MISC_STAT_OFF 0

/*--------------------------------------------------.
 | Register 0x000010ec INT_AGGREGATOR_MISC_RAW_STAT |
 +--------------------------------------------------+
 | bit  31:0 R  MISC_RSTAT                          |
 +-------------------------------------------------*/
#define PMC_INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_UNUSED_MASK    0x00000000
#define INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_MSK 0xffffffff
#define INT_AGR_REG_INT_AGGREGATOR_MISC_RAW_STAT_BIT_MISC_RSTAT_OFF 0

/* index definitions for PMC_INT_AGR_REG_INTER_PROCESSOR_INTS */
#define PMC_INT_AGR_REG_INTER_PROCESSOR_INTS_INDEX_N_MIN    0
#define PMC_INT_AGR_REG_INTER_PROCESSOR_INTS_INDEX_N_MAX    63
#define PMC_INT_AGR_REG_INTER_PROCESSOR_INTS_INDEX_N_SIZE   64
#define PMC_INT_AGR_REG_INTER_PROCESSOR_INTS_INDEX_N_OFFSET 4

/*------------------------------------------------------.
 | Register (0x00001230 + (N) * 4) INTER_PROCESSOR_INTS |
 +------------------------------------------------------+
 | bit  0 R/W  IPI_BITS                                 |
 +-----------------------------------------------------*/
#define PMC_INT_AGR_REG_INTER_PROCESSOR_INTS_UNUSED_MASK    0xfffffffe
#define INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_MSK   0x00000001
#define INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_OFF   0

/*------------------------------------.
 | Register 0x00001330 IPI2PCIE_0_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_0_I0       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_0_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_0_INT_BIT_IPI2PCIE_0_I0_OFF 0

/*------------------------------------.
 | Register 0x00001334 IPI2PCIE_1_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_1_I1       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_1_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_1_INT_BIT_IPI2PCIE_1_I1_OFF 0

/*------------------------------------.
 | Register 0x00001338 IPI2PCIE_2_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_2_I2       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_2_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_2_INT_BIT_IPI2PCIE_2_I2_OFF 0

/*------------------------------------.
 | Register 0x0000133c IPI2PCIE_3_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_3_I3       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_3_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_3_INT_BIT_IPI2PCIE_3_I3_OFF 0

/*---------------------------------------.
 | Register 0x00001340 IPI2PCIE_0_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_0_E0          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_0_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_0_INT_EN_BIT_IPI2PCIE_0_E0_OFF 0

/*---------------------------------------.
 | Register 0x00001344 IPI2PCIE_1_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_1_E1          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_1_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_1_INT_EN_BIT_IPI2PCIE_1_E1_OFF 0

/*---------------------------------------.
 | Register 0x00001348 IPI2PCIE_2_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_2_E2          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_2_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_2_INT_EN_BIT_IPI2PCIE_2_E2_OFF 0

/*---------------------------------------.
 | Register 0x0000134c IPI2PCIE_3_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2PCIE_3_E3          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2PCIE_3_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_MSK 0x0000ffff
#define INT_AGR_REG_IPI2PCIE_3_INT_EN_BIT_IPI2PCIE_3_E3_OFF 0

/*------------------------------------.
 | Register 0x00001350 IPI2MIPS_0_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_0_I4       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_0_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_0_INT_BIT_IPI2MIPS_0_I4_OFF 0

/*------------------------------------.
 | Register 0x00001354 IPI2MIPS_1_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_1_I5       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_1_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_1_INT_BIT_IPI2MIPS_1_I5_OFF 0

/*------------------------------------.
 | Register 0x00001358 IPI2MIPS_2_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_2_I6       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_2_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_2_INT_BIT_IPI2MIPS_2_I6_OFF 0

/*------------------------------------.
 | Register 0x0000135c IPI2MIPS_3_INT |
 +------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_3_I7       |
 +-----------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_3_INT_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_3_INT_BIT_IPI2MIPS_3_I7_OFF 0

/*---------------------------------------.
 | Register 0x00001360 IPI2MIPS_0_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_0_E4          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_0_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_0_INT_EN_BIT_IPI2MIPS_0_E4_OFF 0

/*---------------------------------------.
 | Register 0x00001364 IPI2MIPS_1_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_1_E5          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_1_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_1_INT_EN_BIT_IPI2MIPS_1_E5_OFF 0

/*---------------------------------------.
 | Register 0x00001368 IPI2MIPS_2_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_2_E6          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_2_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_2_INT_EN_BIT_IPI2MIPS_2_E6_OFF 0

/*---------------------------------------.
 | Register 0x0000136c IPI2MIPS_3_INT_EN |
 +---------------------------------------+
 | bit  15:0 R/W  IPI2MIPS_3_E7          |
 +--------------------------------------*/
#define PMC_INT_AGR_REG_IPI2MIPS_3_INT_EN_UNUSED_MASK       0xffff0000
#define INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_MSK 0x0000ffff
#define INT_AGR_REG_IPI2MIPS_3_INT_EN_BIT_IPI2MIPS_3_E7_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INT_AGR_REGS_H */
