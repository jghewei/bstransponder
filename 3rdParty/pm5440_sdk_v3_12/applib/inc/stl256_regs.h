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
 *     stl256 block
 *****************************************************************************/
#ifndef _STL256_REGS_H
#define _STL256_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_STL256_REG_CTRL    0x00000400
#define PMC_STL256_REG_INT_STA 0x00000404
#define PMC_STL256_REG_INT_EN  0x0000040c
#define PMC_STL256_REG_INT_VAL 0x00000414

/*----------------------------------------.
 | Register 0x00000400 CTRL               |
 +----------------------------------------+
 | bit  8   R/W  FORCE_DESKEW_REALIGNMENT |
 | bit  6   R/W  AIS_ACT_DIS              |
 | bit  5   R/W  FORCE_AIS                |
 | bit  4:0 R/W  AF_THRESH                |
 +---------------------------------------*/
#define PMC_STL256_REG_CTRL_UNUSED_MASK                  0xfffffe00
#define STL256_REG_CTRL_BIT_FORCE_DESKEW_REALIGNMENT_MSK 0x00000100
#define STL256_REG_CTRL_BIT_FORCE_DESKEW_REALIGNMENT_OFF 8
#define STL256_REG_CTRL_BIT_AIS_ACT_DIS_MSK              0x00000040
#define STL256_REG_CTRL_BIT_AIS_ACT_DIS_OFF              6
#define STL256_REG_CTRL_BIT_FORCE_AIS_MSK                0x00000020
#define STL256_REG_CTRL_BIT_FORCE_AIS_OFF                5
#define STL256_REG_CTRL_BIT_AF_THRESH_MSK                0x0000001f
#define STL256_REG_CTRL_BIT_AF_THRESH_OFF                0

/*---------------------------------------.
 | Register 0x00000404 INT_STA           |
 +---------------------------------------+
 | bit  30 R/W  EXCESSIVE_SKEW_I         |
 | bit  29 R/W  LANE_ID_NOT_CONSISTENT_I |
 | bit  28 R/W  MARKERS_NOT_UNIQUE_I     |
 | bit  27 R/W  LANE3_OOR_STL_I          |
 | bit  26 R/W  LANE2_OOR_STL_I          |
 | bit  25 R/W  LANE1_OOR_STL_I          |
 | bit  24 R/W  LANE0_OOR_STL_I          |
 | bit  23 R/W  LANE3_DLOS_I             |
 | bit  22 R/W  LANE2_DLOS_I             |
 | bit  21 R/W  LANE1_DLOS_I             |
 | bit  20 R/W  LANE0_DLOS_I             |
 | bit  18 R/W  ATSF_I                   |
 | bit  17 R/W  DLOL_I                   |
 | bit  16 R/W  DSTL_AIS_I               |
 | bit  15 R/W  LANE3_DLOF_STL_I         |
 | bit  14 R/W  LANE2_DLOF_STL_I         |
 | bit  13 R/W  LANE1_DLOF_STL_I         |
 | bit  12 R/W  LANE0_DLOF_STL_I         |
 | bit  11 R/W  LANE3_OOF_STL_I          |
 | bit  10 R/W  LANE2_OOF_STL_I          |
 | bit  9  R/W  LANE1_OOF_STL_I          |
 | bit  8  R/W  LANE0_OOF_STL_I          |
 | bit  4  R/W  LANE_FIFO_OVR_I          |
 | bit  1  R/W  OOF_I                    |
 | bit  0  R/W  DAIS_I                   |
 +--------------------------------------*/
#define PMC_STL256_REG_INT_STA_UNUSED_MASK                  0x800800ec
#define STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_MSK         0x40000000
#define STL256_REG_INT_STA_BIT_EXCESSIVE_SKEW_I_OFF         30
#define STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_MSK 0x20000000
#define STL256_REG_INT_STA_BIT_LANE_ID_NOT_CONSISTENT_I_OFF 29
#define STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_MSK     0x10000000
#define STL256_REG_INT_STA_BIT_MARKERS_NOT_UNIQUE_I_OFF     28
#define STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_MSK          0x08000000
#define STL256_REG_INT_STA_BIT_LANE3_OOR_STL_I_OFF          27
#define STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_MSK          0x04000000
#define STL256_REG_INT_STA_BIT_LANE2_OOR_STL_I_OFF          26
#define STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_MSK          0x02000000
#define STL256_REG_INT_STA_BIT_LANE1_OOR_STL_I_OFF          25
#define STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_MSK          0x01000000
#define STL256_REG_INT_STA_BIT_LANE0_OOR_STL_I_OFF          24
#define STL256_REG_INT_STA_BIT_LANE3_DLOS_I_MSK             0x00800000
#define STL256_REG_INT_STA_BIT_LANE3_DLOS_I_OFF             23
#define STL256_REG_INT_STA_BIT_LANE2_DLOS_I_MSK             0x00400000
#define STL256_REG_INT_STA_BIT_LANE2_DLOS_I_OFF             22
#define STL256_REG_INT_STA_BIT_LANE1_DLOS_I_MSK             0x00200000
#define STL256_REG_INT_STA_BIT_LANE1_DLOS_I_OFF             21
#define STL256_REG_INT_STA_BIT_LANE0_DLOS_I_MSK             0x00100000
#define STL256_REG_INT_STA_BIT_LANE0_DLOS_I_OFF             20
#define STL256_REG_INT_STA_BIT_ATSF_I_MSK                   0x00040000
#define STL256_REG_INT_STA_BIT_ATSF_I_OFF                   18
#define STL256_REG_INT_STA_BIT_DLOL_I_MSK                   0x00020000
#define STL256_REG_INT_STA_BIT_DLOL_I_OFF                   17
#define STL256_REG_INT_STA_BIT_DSTL_AIS_I_MSK               0x00010000
#define STL256_REG_INT_STA_BIT_DSTL_AIS_I_OFF               16
#define STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_MSK         0x00008000
#define STL256_REG_INT_STA_BIT_LANE3_DLOF_STL_I_OFF         15
#define STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_MSK         0x00004000
#define STL256_REG_INT_STA_BIT_LANE2_DLOF_STL_I_OFF         14
#define STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_MSK         0x00002000
#define STL256_REG_INT_STA_BIT_LANE1_DLOF_STL_I_OFF         13
#define STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_MSK         0x00001000
#define STL256_REG_INT_STA_BIT_LANE0_DLOF_STL_I_OFF         12
#define STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_MSK          0x00000800
#define STL256_REG_INT_STA_BIT_LANE3_OOF_STL_I_OFF          11
#define STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_MSK          0x00000400
#define STL256_REG_INT_STA_BIT_LANE2_OOF_STL_I_OFF          10
#define STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_MSK          0x00000200
#define STL256_REG_INT_STA_BIT_LANE1_OOF_STL_I_OFF          9
#define STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_MSK          0x00000100
#define STL256_REG_INT_STA_BIT_LANE0_OOF_STL_I_OFF          8
#define STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_MSK          0x00000010
#define STL256_REG_INT_STA_BIT_LANE_FIFO_OVR_I_OFF          4
#define STL256_REG_INT_STA_BIT_OOF_I_MSK                    0x00000002
#define STL256_REG_INT_STA_BIT_OOF_I_OFF                    1
#define STL256_REG_INT_STA_BIT_DAIS_I_MSK                   0x00000001
#define STL256_REG_INT_STA_BIT_DAIS_I_OFF                   0

/*---------------------------------------.
 | Register 0x0000040c INT_EN            |
 +---------------------------------------+
 | bit  30 R/W  EXCESSIVE_SKEW_E         |
 | bit  29 R/W  LANE_ID_NOT_CONSISTENT_E |
 | bit  28 R/W  MARKERS_NOT_UNIQUE_E     |
 | bit  27 R/W  LANE3_OOR_STL_E          |
 | bit  26 R/W  LANE2_OOR_STL_E          |
 | bit  25 R/W  LANE1_OOR_STL_E          |
 | bit  24 R/W  LANE0_OOR_STL_E          |
 | bit  23 R/W  LANE3_DLOS_E             |
 | bit  22 R/W  LANE2_DLOS_E             |
 | bit  21 R/W  LANE1_DLOS_E             |
 | bit  20 R/W  LANE0_DLOS_E             |
 | bit  18 R/W  ATSF_E                   |
 | bit  17 R/W  DLOL_E                   |
 | bit  16 R/W  DSTL_AIS_E               |
 | bit  15 R/W  LANE3_DLOF_STL_E         |
 | bit  14 R/W  LANE2_DLOF_STL_E         |
 | bit  13 R/W  LANE1_DLOF_STL_E         |
 | bit  12 R/W  LANE0_DLOF_STL_E         |
 | bit  11 R/W  LANE3_OOF_STL_E          |
 | bit  10 R/W  LANE2_OOF_STL_E          |
 | bit  9  R/W  LANE1_OOF_STL_E          |
 | bit  8  R/W  LANE0_OOF_STL_E          |
 | bit  4  R/W  LANE_FIFO_OVR_E          |
 | bit  1  R/W  OOF_E                    |
 | bit  0  R/W  DAIS_E                   |
 +--------------------------------------*/
#define PMC_STL256_REG_INT_EN_UNUSED_MASK                  0x800800ec
#define STL256_REG_INT_EN_BIT_EXCESSIVE_SKEW_E_MSK         0x40000000
#define STL256_REG_INT_EN_BIT_EXCESSIVE_SKEW_E_OFF         30
#define STL256_REG_INT_EN_BIT_LANE_ID_NOT_CONSISTENT_E_MSK 0x20000000
#define STL256_REG_INT_EN_BIT_LANE_ID_NOT_CONSISTENT_E_OFF 29
#define STL256_REG_INT_EN_BIT_MARKERS_NOT_UNIQUE_E_MSK     0x10000000
#define STL256_REG_INT_EN_BIT_MARKERS_NOT_UNIQUE_E_OFF     28
#define STL256_REG_INT_EN_BIT_LANE3_OOR_STL_E_MSK          0x08000000
#define STL256_REG_INT_EN_BIT_LANE3_OOR_STL_E_OFF          27
#define STL256_REG_INT_EN_BIT_LANE2_OOR_STL_E_MSK          0x04000000
#define STL256_REG_INT_EN_BIT_LANE2_OOR_STL_E_OFF          26
#define STL256_REG_INT_EN_BIT_LANE1_OOR_STL_E_MSK          0x02000000
#define STL256_REG_INT_EN_BIT_LANE1_OOR_STL_E_OFF          25
#define STL256_REG_INT_EN_BIT_LANE0_OOR_STL_E_MSK          0x01000000
#define STL256_REG_INT_EN_BIT_LANE0_OOR_STL_E_OFF          24
#define STL256_REG_INT_EN_BIT_LANE3_DLOS_E_MSK             0x00800000
#define STL256_REG_INT_EN_BIT_LANE3_DLOS_E_OFF             23
#define STL256_REG_INT_EN_BIT_LANE2_DLOS_E_MSK             0x00400000
#define STL256_REG_INT_EN_BIT_LANE2_DLOS_E_OFF             22
#define STL256_REG_INT_EN_BIT_LANE1_DLOS_E_MSK             0x00200000
#define STL256_REG_INT_EN_BIT_LANE1_DLOS_E_OFF             21
#define STL256_REG_INT_EN_BIT_LANE0_DLOS_E_MSK             0x00100000
#define STL256_REG_INT_EN_BIT_LANE0_DLOS_E_OFF             20
#define STL256_REG_INT_EN_BIT_ATSF_E_MSK                   0x00040000
#define STL256_REG_INT_EN_BIT_ATSF_E_OFF                   18
#define STL256_REG_INT_EN_BIT_DLOL_E_MSK                   0x00020000
#define STL256_REG_INT_EN_BIT_DLOL_E_OFF                   17
#define STL256_REG_INT_EN_BIT_DSTL_AIS_E_MSK               0x00010000
#define STL256_REG_INT_EN_BIT_DSTL_AIS_E_OFF               16
#define STL256_REG_INT_EN_BIT_LANE3_DLOF_STL_E_MSK         0x00008000
#define STL256_REG_INT_EN_BIT_LANE3_DLOF_STL_E_OFF         15
#define STL256_REG_INT_EN_BIT_LANE2_DLOF_STL_E_MSK         0x00004000
#define STL256_REG_INT_EN_BIT_LANE2_DLOF_STL_E_OFF         14
#define STL256_REG_INT_EN_BIT_LANE1_DLOF_STL_E_MSK         0x00002000
#define STL256_REG_INT_EN_BIT_LANE1_DLOF_STL_E_OFF         13
#define STL256_REG_INT_EN_BIT_LANE0_DLOF_STL_E_MSK         0x00001000
#define STL256_REG_INT_EN_BIT_LANE0_DLOF_STL_E_OFF         12
#define STL256_REG_INT_EN_BIT_LANE3_OOF_STL_E_MSK          0x00000800
#define STL256_REG_INT_EN_BIT_LANE3_OOF_STL_E_OFF          11
#define STL256_REG_INT_EN_BIT_LANE2_OOF_STL_E_MSK          0x00000400
#define STL256_REG_INT_EN_BIT_LANE2_OOF_STL_E_OFF          10
#define STL256_REG_INT_EN_BIT_LANE1_OOF_STL_E_MSK          0x00000200
#define STL256_REG_INT_EN_BIT_LANE1_OOF_STL_E_OFF          9
#define STL256_REG_INT_EN_BIT_LANE0_OOF_STL_E_MSK          0x00000100
#define STL256_REG_INT_EN_BIT_LANE0_OOF_STL_E_OFF          8
#define STL256_REG_INT_EN_BIT_LANE_FIFO_OVR_E_MSK          0x00000010
#define STL256_REG_INT_EN_BIT_LANE_FIFO_OVR_E_OFF          4
#define STL256_REG_INT_EN_BIT_OOF_E_MSK                    0x00000002
#define STL256_REG_INT_EN_BIT_OOF_E_OFF                    1
#define STL256_REG_INT_EN_BIT_DAIS_E_MSK                   0x00000001
#define STL256_REG_INT_EN_BIT_DAIS_E_OFF                   0

/*-------------------------------------.
 | Register 0x00000414 INT_VAL         |
 +-------------------------------------+
 | bit  30 R  EXCESSIVE_SKEW_V         |
 | bit  29 R  LANE_ID_NOT_CONSISTENT_V |
 | bit  28 R  MARKERS_NOT_UNIQUE_V     |
 | bit  27 R  LANE3_OOR_STL_V          |
 | bit  26 R  LANE2_OOR_STL_V          |
 | bit  25 R  LANE1_OOR_STL_V          |
 | bit  24 R  LANE0_OOR_STL_V          |
 | bit  23 R  LANE3_DLOS_V             |
 | bit  22 R  LANE2_DLOS_V             |
 | bit  21 R  LANE1_DLOS_V             |
 | bit  20 R  LANE0_DLOS_V             |
 | bit  18 R  ATSF_V                   |
 | bit  17 R  DLOL_V                   |
 | bit  16 R  DSTL_AIS_V               |
 | bit  15 R  LANE3_DLOF_STL_V         |
 | bit  14 R  LANE2_DLOF_STL_V         |
 | bit  13 R  LANE1_DLOF_STL_V         |
 | bit  12 R  LANE0_DLOF_STL_V         |
 | bit  11 R  LANE3_OOF_STL_V          |
 | bit  10 R  LANE2_OOF_STL_V          |
 | bit  9  R  LANE1_OOF_STL_V          |
 | bit  8  R  LANE0_OOF_STL_V          |
 | bit  1  R  OOF_V                    |
 | bit  0  R  DAIS_V                   |
 +------------------------------------*/
#define PMC_STL256_REG_INT_VAL_UNUSED_MASK                  0x800800fc
#define STL256_REG_INT_VAL_BIT_EXCESSIVE_SKEW_V_MSK         0x40000000
#define STL256_REG_INT_VAL_BIT_EXCESSIVE_SKEW_V_OFF         30
#define STL256_REG_INT_VAL_BIT_LANE_ID_NOT_CONSISTENT_V_MSK 0x20000000
#define STL256_REG_INT_VAL_BIT_LANE_ID_NOT_CONSISTENT_V_OFF 29
#define STL256_REG_INT_VAL_BIT_MARKERS_NOT_UNIQUE_V_MSK     0x10000000
#define STL256_REG_INT_VAL_BIT_MARKERS_NOT_UNIQUE_V_OFF     28
#define STL256_REG_INT_VAL_BIT_LANE3_OOR_STL_V_MSK          0x08000000
#define STL256_REG_INT_VAL_BIT_LANE3_OOR_STL_V_OFF          27
#define STL256_REG_INT_VAL_BIT_LANE2_OOR_STL_V_MSK          0x04000000
#define STL256_REG_INT_VAL_BIT_LANE2_OOR_STL_V_OFF          26
#define STL256_REG_INT_VAL_BIT_LANE1_OOR_STL_V_MSK          0x02000000
#define STL256_REG_INT_VAL_BIT_LANE1_OOR_STL_V_OFF          25
#define STL256_REG_INT_VAL_BIT_LANE0_OOR_STL_V_MSK          0x01000000
#define STL256_REG_INT_VAL_BIT_LANE0_OOR_STL_V_OFF          24
#define STL256_REG_INT_VAL_BIT_LANE3_DLOS_V_MSK             0x00800000
#define STL256_REG_INT_VAL_BIT_LANE3_DLOS_V_OFF             23
#define STL256_REG_INT_VAL_BIT_LANE2_DLOS_V_MSK             0x00400000
#define STL256_REG_INT_VAL_BIT_LANE2_DLOS_V_OFF             22
#define STL256_REG_INT_VAL_BIT_LANE1_DLOS_V_MSK             0x00200000
#define STL256_REG_INT_VAL_BIT_LANE1_DLOS_V_OFF             21
#define STL256_REG_INT_VAL_BIT_LANE0_DLOS_V_MSK             0x00100000
#define STL256_REG_INT_VAL_BIT_LANE0_DLOS_V_OFF             20
#define STL256_REG_INT_VAL_BIT_ATSF_V_MSK                   0x00040000
#define STL256_REG_INT_VAL_BIT_ATSF_V_OFF                   18
#define STL256_REG_INT_VAL_BIT_DLOL_V_MSK                   0x00020000
#define STL256_REG_INT_VAL_BIT_DLOL_V_OFF                   17
#define STL256_REG_INT_VAL_BIT_DSTL_AIS_V_MSK               0x00010000
#define STL256_REG_INT_VAL_BIT_DSTL_AIS_V_OFF               16
#define STL256_REG_INT_VAL_BIT_LANE3_DLOF_STL_V_MSK         0x00008000
#define STL256_REG_INT_VAL_BIT_LANE3_DLOF_STL_V_OFF         15
#define STL256_REG_INT_VAL_BIT_LANE2_DLOF_STL_V_MSK         0x00004000
#define STL256_REG_INT_VAL_BIT_LANE2_DLOF_STL_V_OFF         14
#define STL256_REG_INT_VAL_BIT_LANE1_DLOF_STL_V_MSK         0x00002000
#define STL256_REG_INT_VAL_BIT_LANE1_DLOF_STL_V_OFF         13
#define STL256_REG_INT_VAL_BIT_LANE0_DLOF_STL_V_MSK         0x00001000
#define STL256_REG_INT_VAL_BIT_LANE0_DLOF_STL_V_OFF         12
#define STL256_REG_INT_VAL_BIT_LANE3_OOF_STL_V_MSK          0x00000800
#define STL256_REG_INT_VAL_BIT_LANE3_OOF_STL_V_OFF          11
#define STL256_REG_INT_VAL_BIT_LANE2_OOF_STL_V_MSK          0x00000400
#define STL256_REG_INT_VAL_BIT_LANE2_OOF_STL_V_OFF          10
#define STL256_REG_INT_VAL_BIT_LANE1_OOF_STL_V_MSK          0x00000200
#define STL256_REG_INT_VAL_BIT_LANE1_OOF_STL_V_OFF          9
#define STL256_REG_INT_VAL_BIT_LANE0_OOF_STL_V_MSK          0x00000100
#define STL256_REG_INT_VAL_BIT_LANE0_OOF_STL_V_OFF          8
#define STL256_REG_INT_VAL_BIT_OOF_V_MSK                    0x00000002
#define STL256_REG_INT_VAL_BIT_OOF_V_OFF                    1
#define STL256_REG_INT_VAL_BIT_DAIS_V_MSK                   0x00000001
#define STL256_REG_INT_VAL_BIT_DAIS_V_OFF                   0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _STL256_REGS_H */
