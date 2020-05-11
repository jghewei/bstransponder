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
 *     pcie_al block
 *****************************************************************************/
#ifndef _PCIE_AL_REGS_H
#define _PCIE_AL_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG        0x00009030
#define PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT          0x00009034
#define PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG 0x00009038
#define PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT   0x0000903c
#define PMC_PCIE_REG_LEGACY_INT_EN_REG                               0x00009044
#define PMC_PCIE_REG_MSI_X_PENDING_BIT_ARRAY                         0x00009340
#define PMC_PCIE_REG_PCIE_OB_DB_SET_REG                              0x00009344
#define PMC_PCIE_REG_PCIE_OB_DB_CLR_REG                              0x00009348
#define PMC_PCIE_REG_PCIE_OB_DB_EN_REG                               0x0000934c
#define PMC_PCIE_REG_PCIE_OB_DB_STAT_REG                             0x00009350
#define PMC_PCIE_REG_PCIE_OB_DB_RSTAT_REG                            0x00009354
#define PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG                             0x00009358
#define PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG                             0x0000935c
#define PMC_PCIE_REG_PCIE_OB_IRQ_EN_REG                              0x00009360
#define PMC_PCIE_REG_PCIE_OB_IRQ_STAT_REG                            0x00009364
#define PMC_PCIE_REG_PCIE_OB_IRQ_RSTAT_REG                           0x00009368

/*--------------------------------------------------------------.
 | Register 0x00009030 MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  MU_IB_DB_E2                                   |
 +-------------------------------------------------------------*/
#define PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_UNUSED_MASK     0x00000000
#define PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_MSK 0xffffffff
#define PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_BIT_MU_IB_DB_E2_OFF 0

/*------------------------------------------------------------.
 | Register 0x00009034 MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT |
 +------------------------------------------------------------+
 | bit  31:0 R/W  MU_IB_DB_I2                                 |
 +-----------------------------------------------------------*/
#define PMC_PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_UNUSED_MASK     0x00000000
#define PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_MSK 0xffffffff
#define PCIE_REG_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_BIT_MU_IB_DB_I2_OFF 0

/*---------------------------------------------------------------------.
 | Register 0x00009038 MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG |
 +---------------------------------------------------------------------+
 | bit  31:0 R/W  MU_OB_DB_CLR_E3                                      |
 +--------------------------------------------------------------------*/
#define PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_UNUSED_MASK         0x00000000
#define PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_MSK 0xffffffff
#define PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_BIT_MU_OB_DB_CLR_E3_OFF 0

/*-------------------------------------------------------------------.
 | Register 0x0000903c MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT |
 +-------------------------------------------------------------------+
 | bit  31:0 R/W  MU_OB_DB_CLR_I3                                    |
 +------------------------------------------------------------------*/
#define PMC_PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_UNUSED_MASK         0x00000000
#define PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_MSK 0xffffffff
#define PCIE_REG_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_BIT_MU_OB_DB_CLR_I3_OFF 0

/*---------------------------------------.
 | Register 0x00009044 LEGACY_INT_EN_REG |
 +---------------------------------------+
 | bit  8 R/W  INTA_EN                   |
 +--------------------------------------*/
#define PMC_PCIE_REG_LEGACY_INT_EN_REG_UNUSED_MASK 0xfffff0ff
#define PCIE_REG_LEGACY_INT_EN_REG_BIT_INTA_EN_MSK 0x00000100
#define PCIE_REG_LEGACY_INT_EN_REG_BIT_INTA_EN_OFF 8

/*---------------------------------------------.
 | Register 0x00009340 MSI_X_PENDING_BIT_ARRAY |
 +---------------------------------------------+
 | bit  31:0 R  MSIX_PBA                       |
 +--------------------------------------------*/
#define PMC_PCIE_REG_MSI_X_PENDING_BIT_ARRAY_UNUSED_MASK  0x00000000
#define PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_MSK 0xffffffff
#define PCIE_REG_MSI_X_PENDING_BIT_ARRAY_BIT_MSIX_PBA_OFF 0

/*----------------------------------------.
 | Register 0x00009344 PCIE_OB_DB_SET_REG |
 +----------------------------------------+
 | bit  31:0 R/W  PCIE_OB_DB_SET          |
 +---------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_DB_SET_REG_UNUSED_MASK        0x00000000
#define PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_DB_SET_REG_BIT_PCIE_OB_DB_SET_OFF 0

/*----------------------------------------.
 | Register 0x00009348 PCIE_OB_DB_CLR_REG |
 +----------------------------------------+
 | bit  31:0 R/W  PCIE_OB_DB_CLR          |
 +---------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_DB_CLR_REG_UNUSED_MASK        0x00000000
#define PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_DB_CLR_REG_BIT_PCIE_OB_DB_CLR_OFF 0

/*---------------------------------------.
 | Register 0x0000934c PCIE_OB_DB_EN_REG |
 +---------------------------------------+
 | bit  31:0 R  PCIE_OB_DB_EN            |
 +--------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_DB_EN_REG_UNUSED_MASK       0x00000000
#define PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_DB_EN_REG_BIT_PCIE_OB_DB_EN_OFF 0

/*-----------------------------------------.
 | Register 0x00009350 PCIE_OB_DB_STAT_REG |
 +-----------------------------------------+
 | bit  31:0 R  PCIE_OB_DB_STAT            |
 +----------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_DB_STAT_REG_UNUSED_MASK         0x00000000
#define PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_DB_STAT_REG_BIT_PCIE_OB_DB_STAT_OFF 0

/*------------------------------------------.
 | Register 0x00009354 PCIE_OB_DB_RSTAT_REG |
 +------------------------------------------+
 | bit  31:0 R  PCIE_OB_DB_RSTAT            |
 +-----------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_DB_RSTAT_REG_UNUSED_MASK          0x00000000
#define PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_DB_RSTAT_REG_BIT_PCIE_OB_DB_RSTAT_OFF 0

/*-----------------------------------------.
 | Register 0x00009358 PCIE_OB_IRQ_SET_REG |
 +-----------------------------------------+
 | bit  31:0 R/W  PCIE_OB_IRQ_SET          |
 +----------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_IRQ_SET_REG_UNUSED_MASK         0x00000000
#define PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_IRQ_SET_REG_BIT_PCIE_OB_IRQ_SET_OFF 0

/*-----------------------------------------.
 | Register 0x0000935c PCIE_OB_IRQ_CLR_REG |
 +-----------------------------------------+
 | bit  31:0 R/W  PCIE_OB_IRQ_CLR          |
 +----------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_IRQ_CLR_REG_UNUSED_MASK         0x00000000
#define PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_IRQ_CLR_REG_BIT_PCIE_OB_IRQ_CLR_OFF 0

/*----------------------------------------.
 | Register 0x00009360 PCIE_OB_IRQ_EN_REG |
 +----------------------------------------+
 | bit  31:0 R  PCIE_OB_IRQ_EN            |
 +---------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_IRQ_EN_REG_UNUSED_MASK        0x00000000
#define PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_IRQ_EN_REG_BIT_PCIE_OB_IRQ_EN_OFF 0

/*------------------------------------------.
 | Register 0x00009364 PCIE_OB_IRQ_STAT_REG |
 +------------------------------------------+
 | bit  31:0 R  PCIE_OB_IRQ_STAT            |
 +-----------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_IRQ_STAT_REG_UNUSED_MASK          0x00000000
#define PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_IRQ_STAT_REG_BIT_PCIE_OB_IRQ_STAT_OFF 0

/*-------------------------------------------.
 | Register 0x00009368 PCIE_OB_IRQ_RSTAT_REG |
 +-------------------------------------------+
 | bit  31:0 R  PCIE_OB_IRQ_RSTAT            |
 +------------------------------------------*/
#define PMC_PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_UNUSED_MASK           0x00000000
#define PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_MSK 0xffffffff
#define PCIE_REG_PCIE_OB_IRQ_RSTAT_REG_BIT_PCIE_OB_IRQ_RSTAT_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PCIE_AL_REGS_H */
