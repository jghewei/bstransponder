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
 *     rrmp block
 *****************************************************************************/
#ifndef _RRMP_REGS_H
#define _RRMP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_RRMP_REG_CFG                  0x00000100
#define PMC_RRMP_REG_CFG_2                0x00000104
#define PMC_RRMP_REG_STAT                 0x00000108
#define PMC_RRMP_REG_INT_EN               0x0000010c
#define PMC_RRMP_REG_INT_STAT             0x00000110
#define PMC_RRMP_REG_RX_APS               0x00000114
#define PMC_RRMP_REG_RXD_SSM              0x00000118
#define PMC_RRMP_REG_SECTION_BIP_ERR_CNT  0x00000120
#define PMC_RRMP_REG_LINE_BIP_ERR_CNT_LSB 0x00000124
#define PMC_RRMP_REG_LINE_BIP_ERR_CNT_MSB 0x00000128
#define PMC_RRMP_REG_LINE_REI_ERR_CNT_LSB 0x0000012c
#define PMC_RRMP_REG_LINE_REI_ERR_CNT_MSB 0x00000130
#define PMC_RRMP_REG_RXD_J0_BYTE          0x00000134

/*---------------------------.
 | Register 0x00000100 CFG   |
 +---------------------------+
 | bit  15  R    TIP         |
 | bit  14  R/W  EXTAPSDIS   |
 | bit  12  R/W  LREIACCBLK  |
 | bit  11  R/W  LBIPEREIBLK |
 | bit  9   R/W  LBIPEACCBLK |
 | bit  7   R/W  SBIPEACCBLK |
 | bit  6   R/W  LBIPEREISAT |
 | bit  5:4 R/W  APSUNST     |
 | bit  3   R/W  LRDI3       |
 | bit  2   R/W  LAIS3       |
 | bit  1   R/W  ALGO2       |
 | bit  0   R/W  FOOF        |
 +--------------------------*/
#define PMC_RRMP_REG_CFG_UNUSED_MASK     0xffff0000
#define RRMP_REG_CFG_BIT_TIP_MSK         0x00008000
#define RRMP_REG_CFG_BIT_TIP_OFF         15
#define RRMP_REG_CFG_BIT_EXTAPSDIS_MSK   0x00004000
#define RRMP_REG_CFG_BIT_EXTAPSDIS_OFF   14
#define RRMP_REG_CFG_BIT_LREIACCBLK_MSK  0x00001000
#define RRMP_REG_CFG_BIT_LREIACCBLK_OFF  12
#define RRMP_REG_CFG_BIT_LBIPEREIBLK_MSK 0x00000800
#define RRMP_REG_CFG_BIT_LBIPEREIBLK_OFF 11
#define RRMP_REG_CFG_BIT_LBIPEACCBLK_MSK 0x00000200
#define RRMP_REG_CFG_BIT_LBIPEACCBLK_OFF 9
#define RRMP_REG_CFG_BIT_SBIPEACCBLK_MSK 0x00000080
#define RRMP_REG_CFG_BIT_SBIPEACCBLK_OFF 7
#define RRMP_REG_CFG_BIT_LBIPEREISAT_MSK 0x00000040
#define RRMP_REG_CFG_BIT_LBIPEREISAT_OFF 6
#define RRMP_REG_CFG_BIT_APSUNST_MSK     0x00000030
#define RRMP_REG_CFG_BIT_APSUNST_OFF     4
#define RRMP_REG_CFG_BIT_LRDI3_MSK       0x00000008
#define RRMP_REG_CFG_BIT_LRDI3_OFF       3
#define RRMP_REG_CFG_BIT_LAIS3_MSK       0x00000004
#define RRMP_REG_CFG_BIT_LAIS3_OFF       2
#define RRMP_REG_CFG_BIT_ALGO2_MSK       0x00000002
#define RRMP_REG_CFG_BIT_ALGO2_OFF       1
#define RRMP_REG_CFG_BIT_FOOF_MSK        0x00000001
#define RRMP_REG_CFG_BIT_FOOF_OFF        0

/*-----------------------------.
 | Register 0x00000104 CFG_2   |
 +-----------------------------+
 | bit  15 R/W  DESCRAMA1A2SEL |
 | bit  14 R/W  DESCRAMJ0SEL   |
 | bit  13 R/W  FRM768SEL      |
 | bit  12 R/W  THREE_FRMSSM   |
 | bit  5  R/W  LOSSPEN        |
 | bit  3  R/W  B2MASKEN       |
 | bit  1  R/W  B1MASKEN       |
 | bit  0  R/W  M0EN           |
 +----------------------------*/
#define PMC_RRMP_REG_CFG_2_UNUSED_MASK        0xffff03c0
#define RRMP_REG_CFG_2_BIT_DESCRAMA1A2SEL_MSK 0x00008000
#define RRMP_REG_CFG_2_BIT_DESCRAMA1A2SEL_OFF 15
#define RRMP_REG_CFG_2_BIT_DESCRAMJ0SEL_MSK   0x00004000
#define RRMP_REG_CFG_2_BIT_DESCRAMJ0SEL_OFF   14
#define RRMP_REG_CFG_2_BIT_FRM768SEL_MSK      0x00002000
#define RRMP_REG_CFG_2_BIT_FRM768SEL_OFF      13
#define RRMP_REG_CFG_2_BIT_THREE_FRMSSM_MSK   0x00001000
#define RRMP_REG_CFG_2_BIT_THREE_FRMSSM_OFF   12
#define RRMP_REG_CFG_2_BIT_LOSSPEN_MSK        0x00000020
#define RRMP_REG_CFG_2_BIT_LOSSPEN_OFF        5
#define RRMP_REG_CFG_2_BIT_B2MASKEN_MSK       0x00000008
#define RRMP_REG_CFG_2_BIT_B2MASKEN_OFF       3
#define RRMP_REG_CFG_2_BIT_B1MASKEN_MSK       0x00000002
#define RRMP_REG_CFG_2_BIT_B1MASKEN_OFF       1
#define RRMP_REG_CFG_2_BIT_M0EN_MSK           0x00000001
#define RRMP_REG_CFG_2_BIT_M0EN_OFF           0

/*--------------------------.
 | Register 0x00000108 STAT |
 +--------------------------+
 | bit  5 R  APSBF_V        |
 | bit  4 R  LRDI_V         |
 | bit  3 R  LAIS_V         |
 | bit  2 R  LOS_V          |
 | bit  1 R  LOF_V          |
 | bit  0 R  OOF_V          |
 +-------------------------*/
#define PMC_RRMP_REG_STAT_UNUSED_MASK 0xffffffc0
#define RRMP_REG_STAT_BIT_APSBF_V_MSK 0x00000020
#define RRMP_REG_STAT_BIT_APSBF_V_OFF 5
#define RRMP_REG_STAT_BIT_LRDI_V_MSK  0x00000010
#define RRMP_REG_STAT_BIT_LRDI_V_OFF  4
#define RRMP_REG_STAT_BIT_LAIS_V_MSK  0x00000008
#define RRMP_REG_STAT_BIT_LAIS_V_OFF  3
#define RRMP_REG_STAT_BIT_LOS_V_MSK   0x00000004
#define RRMP_REG_STAT_BIT_LOS_V_OFF   2
#define RRMP_REG_STAT_BIT_LOF_V_MSK   0x00000002
#define RRMP_REG_STAT_BIT_LOF_V_OFF   1
#define RRMP_REG_STAT_BIT_OOF_V_MSK   0x00000001
#define RRMP_REG_STAT_BIT_OOF_V_OFF   0

/*----------------------------.
 | Register 0x0000010c INT_EN |
 +----------------------------+
 | bit  11 R/W  COK2_E        |
 | bit  10 R/W  LREIE_E       |
 | bit  9  R/W  LBIPE_E       |
 | bit  8  R/W  SBIPE_E       |
 | bit  7  R/W  COSSM_E       |
 | bit  6  R/W  COAPS_E       |
 | bit  5  R/W  APSBF_E       |
 | bit  4  R/W  LRDI_E        |
 | bit  3  R/W  LAIS_E        |
 | bit  2  R/W  LOS_E         |
 | bit  1  R/W  LOF_E         |
 | bit  0  R/W  OOF_E         |
 +---------------------------*/
#define PMC_RRMP_REG_INT_EN_UNUSED_MASK 0xfffff000
#define RRMP_REG_INT_EN_BIT_COK2_E_MSK  0x00000800
#define RRMP_REG_INT_EN_BIT_COK2_E_OFF  11
#define RRMP_REG_INT_EN_BIT_LREIE_E_MSK 0x00000400
#define RRMP_REG_INT_EN_BIT_LREIE_E_OFF 10
#define RRMP_REG_INT_EN_BIT_LBIPE_E_MSK 0x00000200
#define RRMP_REG_INT_EN_BIT_LBIPE_E_OFF 9
#define RRMP_REG_INT_EN_BIT_SBIPE_E_MSK 0x00000100
#define RRMP_REG_INT_EN_BIT_SBIPE_E_OFF 8
#define RRMP_REG_INT_EN_BIT_COSSM_E_MSK 0x00000080
#define RRMP_REG_INT_EN_BIT_COSSM_E_OFF 7
#define RRMP_REG_INT_EN_BIT_COAPS_E_MSK 0x00000040
#define RRMP_REG_INT_EN_BIT_COAPS_E_OFF 6
#define RRMP_REG_INT_EN_BIT_APSBF_E_MSK 0x00000020
#define RRMP_REG_INT_EN_BIT_APSBF_E_OFF 5
#define RRMP_REG_INT_EN_BIT_LRDI_E_MSK  0x00000010
#define RRMP_REG_INT_EN_BIT_LRDI_E_OFF  4
#define RRMP_REG_INT_EN_BIT_LAIS_E_MSK  0x00000008
#define RRMP_REG_INT_EN_BIT_LAIS_E_OFF  3
#define RRMP_REG_INT_EN_BIT_LOS_E_MSK   0x00000004
#define RRMP_REG_INT_EN_BIT_LOS_E_OFF   2
#define RRMP_REG_INT_EN_BIT_LOF_E_MSK   0x00000002
#define RRMP_REG_INT_EN_BIT_LOF_E_OFF   1
#define RRMP_REG_INT_EN_BIT_OOF_E_MSK   0x00000001
#define RRMP_REG_INT_EN_BIT_OOF_E_OFF   0

/*------------------------------.
 | Register 0x00000110 INT_STAT |
 +------------------------------+
 | bit  11 R/W  COK2_I          |
 | bit  10 R/W  LREIE_I         |
 | bit  9  R/W  LBIPE_I         |
 | bit  8  R/W  SBIPE_I         |
 | bit  7  R/W  COSSM_I         |
 | bit  6  R/W  COAPS_I         |
 | bit  5  R/W  APSBF_I         |
 | bit  4  R/W  LRDI_I          |
 | bit  3  R/W  LAIS_I          |
 | bit  2  R/W  LOS_I           |
 | bit  1  R/W  LOF_I           |
 | bit  0  R/W  OOF_I           |
 +-----------------------------*/
#define PMC_RRMP_REG_INT_STAT_UNUSED_MASK 0xfffff000
#define RRMP_REG_INT_STAT_BIT_COK2_I_MSK  0x00000800
#define RRMP_REG_INT_STAT_BIT_COK2_I_OFF  11
#define RRMP_REG_INT_STAT_BIT_LREIE_I_MSK 0x00000400
#define RRMP_REG_INT_STAT_BIT_LREIE_I_OFF 10
#define RRMP_REG_INT_STAT_BIT_LBIPE_I_MSK 0x00000200
#define RRMP_REG_INT_STAT_BIT_LBIPE_I_OFF 9
#define RRMP_REG_INT_STAT_BIT_SBIPE_I_MSK 0x00000100
#define RRMP_REG_INT_STAT_BIT_SBIPE_I_OFF 8
#define RRMP_REG_INT_STAT_BIT_COSSM_I_MSK 0x00000080
#define RRMP_REG_INT_STAT_BIT_COSSM_I_OFF 7
#define RRMP_REG_INT_STAT_BIT_COAPS_I_MSK 0x00000040
#define RRMP_REG_INT_STAT_BIT_COAPS_I_OFF 6
#define RRMP_REG_INT_STAT_BIT_APSBF_I_MSK 0x00000020
#define RRMP_REG_INT_STAT_BIT_APSBF_I_OFF 5
#define RRMP_REG_INT_STAT_BIT_LRDI_I_MSK  0x00000010
#define RRMP_REG_INT_STAT_BIT_LRDI_I_OFF  4
#define RRMP_REG_INT_STAT_BIT_LAIS_I_MSK  0x00000008
#define RRMP_REG_INT_STAT_BIT_LAIS_I_OFF  3
#define RRMP_REG_INT_STAT_BIT_LOS_I_MSK   0x00000004
#define RRMP_REG_INT_STAT_BIT_LOS_I_OFF   2
#define RRMP_REG_INT_STAT_BIT_LOF_I_MSK   0x00000002
#define RRMP_REG_INT_STAT_BIT_LOF_I_OFF   1
#define RRMP_REG_INT_STAT_BIT_OOF_I_MSK   0x00000001
#define RRMP_REG_INT_STAT_BIT_OOF_I_OFF   0

/*----------------------------.
 | Register 0x00000114 RX_APS |
 +----------------------------+
 | bit  15:8 R  K1V           |
 | bit  7:0  R  K2V           |
 +---------------------------*/
#define PMC_RRMP_REG_RX_APS_UNUSED_MASK 0xffff0000
#define RRMP_REG_RX_APS_BIT_K1V_MSK     0x0000ff00
#define RRMP_REG_RX_APS_BIT_K1V_OFF     8
#define RRMP_REG_RX_APS_BIT_K2V_MSK     0x000000ff
#define RRMP_REG_RX_APS_BIT_K2V_OFF     0

/*-----------------------------.
 | Register 0x00000118 RXD_SSM |
 +-----------------------------+
 | bit  15  R/W  BYTESSM       |
 | bit  14  R/W  FLTRSSM       |
 | bit  7:0 R    SSMV          |
 +----------------------------*/
#define PMC_RRMP_REG_RXD_SSM_UNUSED_MASK 0xffff3f00
#define RRMP_REG_RXD_SSM_BIT_BYTESSM_MSK 0x00008000
#define RRMP_REG_RXD_SSM_BIT_BYTESSM_OFF 15
#define RRMP_REG_RXD_SSM_BIT_FLTRSSM_MSK 0x00004000
#define RRMP_REG_RXD_SSM_BIT_FLTRSSM_OFF 14
#define RRMP_REG_RXD_SSM_BIT_SSMV_MSK    0x000000ff
#define RRMP_REG_RXD_SSM_BIT_SSMV_OFF    0

/*-----------------------------------------.
 | Register 0x00000120 SECTION_BIP_ERR_CNT |
 +-----------------------------------------+
 | bit  15:0 R  SBIPE                      |
 +----------------------------------------*/
#define PMC_RRMP_REG_SECTION_BIP_ERR_CNT_UNUSED_MASK 0xffff0000
#define RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_MSK   0x0000ffff
#define RRMP_REG_SECTION_BIP_ERR_CNT_BIT_SBIPE_OFF   0

/*------------------------------------------.
 | Register 0x00000124 LINE_BIP_ERR_CNT_LSB |
 +------------------------------------------+
 | bit  15:0 R  LBIPE                       |
 +-----------------------------------------*/
#define PMC_RRMP_REG_LINE_BIP_ERR_CNT_LSB_UNUSED_MASK 0xffff0000
#define RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_MSK   0x0000ffff
#define RRMP_REG_LINE_BIP_ERR_CNT_LSB_BIT_LBIPE_OFF   0

/*------------------------------------------.
 | Register 0x00000128 LINE_BIP_ERR_CNT_MSB |
 +------------------------------------------+
 | bit  9:0 R  LBIPE                        |
 +-----------------------------------------*/
#define PMC_RRMP_REG_LINE_BIP_ERR_CNT_MSB_UNUSED_MASK 0xfffffc00
#define RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_MSK   0x000003ff
#define RRMP_REG_LINE_BIP_ERR_CNT_MSB_BIT_LBIPE_OFF   0

/*------------------------------------------.
 | Register 0x0000012c LINE_REI_ERR_CNT_LSB |
 +------------------------------------------+
 | bit  15:0 R  LREIE                       |
 +-----------------------------------------*/
#define PMC_RRMP_REG_LINE_REI_ERR_CNT_LSB_UNUSED_MASK 0xffff0000
#define RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_MSK   0x0000ffff
#define RRMP_REG_LINE_REI_ERR_CNT_LSB_BIT_LREIE_OFF   0

/*------------------------------------------.
 | Register 0x00000130 LINE_REI_ERR_CNT_MSB |
 +------------------------------------------+
 | bit  9:0 R  LREIE                        |
 +-----------------------------------------*/
#define PMC_RRMP_REG_LINE_REI_ERR_CNT_MSB_UNUSED_MASK 0xfffffc00
#define RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_MSK   0x000003ff
#define RRMP_REG_LINE_REI_ERR_CNT_MSB_BIT_LREIE_OFF   0

/*---------------------------------.
 | Register 0x00000134 RXD_J0_BYTE |
 +---------------------------------+
 | bit  7:0 R  J0V                 |
 +--------------------------------*/
#define PMC_RRMP_REG_RXD_J0_BYTE_UNUSED_MASK 0xffffff00
#define RRMP_REG_RXD_J0_BYTE_BIT_J0V_MSK     0x000000ff
#define RRMP_REG_RXD_J0_BYTE_BIT_J0V_OFF     0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RRMP_REGS_H */
