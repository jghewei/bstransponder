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
 *     eclass block
 *****************************************************************************/
#ifndef _ECLASS_REGS_H
#define _ECLASS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ECLASS120_REG_ECLASS120_LINK_EN                              0x0000001c
#define PMC_ECLASS120_REG_GEN_CFG_VLAN_MPLS( N )                         (0x00000024 + (N) * 512)
#define PMC_ECLASS120_REG_VLAN_TPID( N )                                 (0x00000028 + (N) * 512)
#define PMC_ECLASS120_REG_MPLS_ETHERTYPE( N )                            (0x00000030 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_1( N )                    (0x00000038 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_2( N )                    (0x0000003c + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_1( N )                    (0x00000040 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_2( N )                    (0x00000044 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_1( N )                    (0x00000048 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_2( N )                    (0x0000004c + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_1( N )                    (0x00000050 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_2( N )                    (0x00000054 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_1( N )                    (0x00000058 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_2( N )                    (0x0000005c + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_1( N )                    (0x00000060 + (N) * 512)
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_2( N )                    (0x00000064 + (N) * 512)
#define PMC_ECLASS120_REG_MAN_VLAN_CFG( N )                              (0x00000070 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_LSB( N )                        (0x00000074 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_MSB( N )                        (0x00000078 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB( N )                    (0x0000007c + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB( N )                    (0x00000080 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DEST_PORT_UDP( N )                     (0x00000084 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4( N )                           (0x00000088 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_P2P( N )                       (0x0000008c + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_0( N )                         (0x00000090 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_1( N )                         (0x00000094 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_2( N )                         (0x00000098 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_3( N )                         (0x0000009c + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_0( N )                     (0x000000a4 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_1( N )                     (0x000000a8 + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_2( N )                     (0x000000ac + (N) * 512)
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_3( N )                     (0x000000b0 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_0( N )                     (0x000000d0 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_1( N )                     (0x000000d4 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_2( N )                     (0x000000d8 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_3( N )                     (0x000000dc + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_0( N )                      (0x000000e0 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_1( N )                      (0x000000e4 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_2( N )                      (0x000000e8 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_3( N )                      (0x000000ec + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_4( N )                      (0x000000f0 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_5( N )                      (0x000000f4 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_6( N )                      (0x000000f8 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_7( N )                      (0x000000fc + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0( N )               (0x00000110 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1( N )               (0x00000114 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2( N )               (0x00000118 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3( N )               (0x0000011c + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4( N )               (0x00000120 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5( N )               (0x00000124 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6( N )               (0x00000128 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7( N )               (0x0000012c + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_0( N )                      (0x00000140 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_1( N )                      (0x00000144 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_2( N )                      (0x00000148 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_3( N )                      (0x0000014c + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_4( N )                      (0x00000150 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_5( N )                      (0x00000154 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_6( N )                      (0x00000158 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_7( N )                      (0x0000015c + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_8( N )                      (0x00000160 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_9( N )                      (0x00000164 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_10( N )                     (0x00000168 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_11( N )                     (0x0000016c + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_12( N )                     (0x00000170 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_13( N )                     (0x00000174 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_14( N )                     (0x00000178 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_15( N )                     (0x0000017c + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW( N ) (0x0000018c + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG1( N )                  (0x000001a0 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG2( N )                  (0x000001a4 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG1( N )                  (0x000001a8 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG2( N )                  (0x000001ac + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG1( N )                  (0x000001b0 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG2( N )                  (0x000001b4 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG1( N )                  (0x000001b8 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG2( N )                  (0x000001bc + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG1( N )                  (0x000001c0 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG2( N )                  (0x000001c4 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG1( N )                  (0x000001c8 + (N) * 512)
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG2( N )                  (0x000001cc + (N) * 512)
#define PMC_ECLASS120_REG_LINK_ECO( N )                                  (0x000001f0 + (N) * 512)
#define PMC_ECLASS120_REG_GEN_CFG_HWIRED_FILTER( N )                     (0x00001b00 + (N) * 4)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1( N )           (0x00002000 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2( N )           (0x00002004 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1( N )           (0x00002008 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2( N )           (0x0000200c + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1( N )           (0x00002010 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2( N )           (0x00002014 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1( N )           (0x00002018 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2( N )           (0x0000201c + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1( N )           (0x00002020 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2( N )           (0x00002024 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1( N )           (0x00002028 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2( N )           (0x0000202c + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1( N )           (0x00002030 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2( N )           (0x00002034 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1( N )           (0x00002038 + (N) * 128)
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2( N )           (0x0000203c + (N) * 128)
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_1( N )                          (0x00002c00 + (N) * 4)
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_2( N )                          (0x00002c50 + (N) * 4)
#define PMC_ECLASS120_REG_FOUT_COMB_AND_1( N )                           (0x00002ca0 + (N) * 4)
#define PMC_ECLASS120_REG_FOUT_COMB_AND_2( N )                           (0x00002d50 + (N) * 4)
#define PMC_ECLASS120_REG_FOUT_COMB_AND_3( N )                           (0x00002e00 + (N) * 4)
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_1( N )                         (0x00002eb0 + (N) * 4)
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_2( N )                         (0x00002f30 + (N) * 4)
#define PMC_ECLASS120_REG_NO_MATCH_EN                                    0x00003900

/*---------------------------------------.
 | Register 0x0000001c ECLASS120_LINK_EN |
 +---------------------------------------+
 | bit  11 R/W  ECLASS120_LINK11_EN      |
 | bit  10 R/W  ECLASS120_LINK10_EN      |
 | bit  9  R/W  ECLASS120_LINK9_EN       |
 | bit  8  R/W  ECLASS120_LINK8_EN       |
 | bit  7  R/W  ECLASS120_LINK7_EN       |
 | bit  6  R/W  ECLASS120_LINK6_EN       |
 | bit  5  R/W  ECLASS120_LINK5_EN       |
 | bit  4  R/W  ECLASS120_LINK4_EN       |
 | bit  3  R/W  ECLASS120_LINK3_EN       |
 | bit  2  R/W  ECLASS120_LINK2_EN       |
 | bit  1  R/W  ECLASS120_LINK1_EN       |
 | bit  0  R/W  ECLASS120_LINK0_EN       |
 +--------------------------------------*/
#define PMC_ECLASS120_REG_ECLASS120_LINK_EN_UNUSED_MASK             0xfffff000
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK11_EN_MSK 0x00000800
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK11_EN_OFF 11
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK10_EN_MSK 0x00000400
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK10_EN_OFF 10
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK9_EN_MSK  0x00000200
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK9_EN_OFF  9
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK8_EN_MSK  0x00000100
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK8_EN_OFF  8
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK7_EN_MSK  0x00000080
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK7_EN_OFF  7
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK6_EN_MSK  0x00000040
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK6_EN_OFF  6
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK5_EN_MSK  0x00000020
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK5_EN_OFF  5
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK4_EN_MSK  0x00000010
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK4_EN_OFF  4
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK3_EN_MSK  0x00000008
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK3_EN_OFF  3
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK2_EN_MSK  0x00000004
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK2_EN_OFF  2
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK1_EN_MSK  0x00000002
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK1_EN_OFF  1
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK0_EN_MSK  0x00000001
#define ECLASS120_REG_ECLASS120_LINK_EN_BIT_ECLASS120_LINK0_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_CFG_VLAN_MPLS */
#define PMC_ECLASS120_REG_GEN_CFG_VLAN_MPLS_INDEX_N_MIN                   0
#define PMC_ECLASS120_REG_GEN_CFG_VLAN_MPLS_INDEX_N_MAX                   11
#define PMC_ECLASS120_REG_GEN_CFG_VLAN_MPLS_INDEX_N_SIZE                  12
#define PMC_ECLASS120_REG_GEN_CFG_VLAN_MPLS_INDEX_N_OFFSET                512

/*-----------------------------------------------------.
 | Register (0x00000024 + (N) * 512) GEN_CFG_VLAN_MPLS |
 +-----------------------------------------------------+
 | bit  30    R/W  MPLS_ETYPE_CUSTOM_EN                |
 | bit  29    R/W  MPLS_ETYPE_8848_EN                  |
 | bit  28    R/W  MPLS_ETYPE_8847_EN                  |
 | bit  27:26 R/W  MPLS_NB_LEVELS                      |
 | bit  25    R/W  MPLS_OVERFLOW_CFG                   |
 | bit  24:23 R/W  MPLS_MESS                           |
 | bit  22    R/W  VLAN_TPID_CUSTOM_OUTER_EN           |
 | bit  21    R/W  VLAN_TPID_CUSTOM_INNER_EN           |
 | bit  20    R/W  VLAN_TPID_88A8_EN                   |
 | bit  19    R/W  VLAN_TPID_9100_EN                   |
 | bit  18    R/W  VLAN_TPID_8100_EN                   |
 | bit  17    R/W  VLAN_OUTER_EN                       |
 | bit  16    R/W  VLAN_INNER_EN                       |
 | bit  10:9  R/W  VLAN_MPLS_STATE_EN                  |
 | bit  7     R/W  VLAN_MPLS_PRIO_EN                   |
 | bit  6:4   R/W  VLAN_MPLS_PRIO_DEFAULT              |
 | bit  3:2   R/W  VLAN_MPLS_PRIO_LEVEL                |
 | bit  1:0   R/W  VLAN_MPLS_PRIO_TYPE                 |
 +----------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_CFG_VLAN_MPLS_UNUSED_MASK                   0x8000f900
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_ETYPE_CUSTOM_EN_MSK      0x40000000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_ETYPE_CUSTOM_EN_OFF      30
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_ETYPE_8848_EN_MSK        0x20000000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_ETYPE_8848_EN_OFF        29
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_ETYPE_8847_EN_MSK        0x10000000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_ETYPE_8847_EN_OFF        28
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_NB_LEVELS_MSK            0x0c000000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_NB_LEVELS_OFF            26
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_OVERFLOW_CFG_MSK         0x02000000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_OVERFLOW_CFG_OFF         25
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_MESS_MSK                 0x01800000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_MPLS_MESS_OFF                 23
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_CUSTOM_OUTER_EN_MSK 0x00400000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_CUSTOM_OUTER_EN_OFF 22
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_CUSTOM_INNER_EN_MSK 0x00200000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_CUSTOM_INNER_EN_OFF 21
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_88A8_EN_MSK         0x00100000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_88A8_EN_OFF         20
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_9100_EN_MSK         0x00080000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_9100_EN_OFF         19
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_8100_EN_MSK         0x00040000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_TPID_8100_EN_OFF         18
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_OUTER_EN_MSK             0x00020000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_OUTER_EN_OFF             17
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_INNER_EN_MSK             0x00010000
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_INNER_EN_OFF             16
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_STATE_EN_MSK        0x00000600
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_STATE_EN_OFF        9
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_EN_MSK         0x00000080
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_EN_OFF         7
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_DEFAULT_MSK    0x00000070
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_DEFAULT_OFF    4
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_LEVEL_MSK      0x0000000c
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_LEVEL_OFF      2
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_TYPE_MSK       0x00000003
#define ECLASS120_REG_GEN_CFG_VLAN_MPLS_BIT_VLAN_MPLS_PRIO_TYPE_OFF       0

/* index definitions for PMC_ECLASS120_REG_VLAN_TPID */
#define PMC_ECLASS120_REG_VLAN_TPID_INDEX_N_MIN                0
#define PMC_ECLASS120_REG_VLAN_TPID_INDEX_N_MAX                11
#define PMC_ECLASS120_REG_VLAN_TPID_INDEX_N_SIZE               12
#define PMC_ECLASS120_REG_VLAN_TPID_INDEX_N_OFFSET             512

/*---------------------------------------------.
 | Register (0x00000028 + (N) * 512) VLAN_TPID |
 +---------------------------------------------+
 | bit  31:16 R/W  VLAN_CUSTOM_TPID_OUTER      |
 | bit  15:0  R/W  VLAN_CUSTOM_TPID_INNER      |
 +--------------------------------------------*/
#define PMC_ECLASS120_REG_VLAN_TPID_UNUSED_MASK                0x00000000
#define ECLASS120_REG_VLAN_TPID_BIT_VLAN_CUSTOM_TPID_OUTER_MSK 0xffff0000
#define ECLASS120_REG_VLAN_TPID_BIT_VLAN_CUSTOM_TPID_OUTER_OFF 16
#define ECLASS120_REG_VLAN_TPID_BIT_VLAN_CUSTOM_TPID_INNER_MSK 0x0000ffff
#define ECLASS120_REG_VLAN_TPID_BIT_VLAN_CUSTOM_TPID_INNER_OFF 0

/* index definitions for PMC_ECLASS120_REG_MPLS_ETHERTYPE */
#define PMC_ECLASS120_REG_MPLS_ETHERTYPE_INDEX_N_MIN               0
#define PMC_ECLASS120_REG_MPLS_ETHERTYPE_INDEX_N_MAX               11
#define PMC_ECLASS120_REG_MPLS_ETHERTYPE_INDEX_N_SIZE              12
#define PMC_ECLASS120_REG_MPLS_ETHERTYPE_INDEX_N_OFFSET            512

/*--------------------------------------------------.
 | Register (0x00000030 + (N) * 512) MPLS_ETHERTYPE |
 +--------------------------------------------------+
 | bit  15:0 R/W  MPLS_CUSTOM_ETHERTYPE             |
 +-------------------------------------------------*/
#define PMC_ECLASS120_REG_MPLS_ETHERTYPE_UNUSED_MASK               0xffff0000
#define ECLASS120_REG_MPLS_ETHERTYPE_BIT_MPLS_CUSTOM_ETHERTYPE_MSK 0x0000ffff
#define ECLASS120_REG_MPLS_ETHERTYPE_BIT_MPLS_CUSTOM_ETHERTYPE_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_1 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_1_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_1_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_1_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_1_INDEX_N_OFFSET        512

/*----------------------------------------------------------.
 | Register (0x00000038 + (N) * 512) VLANMPLS_FILT20_TAG0_1 |
 +----------------------------------------------------------+
 | bit  19:0 R/W  FILT20_TAG0_VALUE                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_1_UNUSED_MASK           0xfff00000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_1_BIT_FILT20_TAG0_VALUE_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_1_BIT_FILT20_TAG0_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_2 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_INDEX_N_MIN          0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_INDEX_N_MAX          11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_INDEX_N_SIZE         12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_INDEX_N_OFFSET       512

/*----------------------------------------------------------.
 | Register (0x0000003c + (N) * 512) VLANMPLS_FILT20_TAG0_2 |
 +----------------------------------------------------------+
 | bit  31:28 R/W  FILT20_TAG0_TYPE                         |
 | bit  24    R/W  FILT20_TAG0_INV                          |
 | bit  22:20 R/W  FILT20_TAG0_ADDR                         |
 | bit  19:0  R/W  FILT20_TAG0_MASK                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_UNUSED_MASK          0x0e800000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_TYPE_MSK 0xf0000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_TYPE_OFF 28
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_INV_MSK  0x01000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_INV_OFF  24
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_ADDR_MSK 0x00700000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_ADDR_OFF 20
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_MASK_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG0_2_BIT_FILT20_TAG0_MASK_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_1 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_1_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_1_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_1_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_1_INDEX_N_OFFSET        512

/*----------------------------------------------------------.
 | Register (0x00000040 + (N) * 512) VLANMPLS_FILT20_TAG1_1 |
 +----------------------------------------------------------+
 | bit  19:0 R/W  FILT20_TAG1_VALUE                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_1_UNUSED_MASK           0xfff00000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_1_BIT_FILT20_TAG1_VALUE_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_1_BIT_FILT20_TAG1_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_2 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_INDEX_N_MIN          0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_INDEX_N_MAX          11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_INDEX_N_SIZE         12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_INDEX_N_OFFSET       512

/*----------------------------------------------------------.
 | Register (0x00000044 + (N) * 512) VLANMPLS_FILT20_TAG1_2 |
 +----------------------------------------------------------+
 | bit  31:28 R/W  FILT20_TAG1_TYPE                         |
 | bit  24    R/W  FILT20_TAG1_INV                          |
 | bit  22:20 R/W  FILT20_TAG1_ADDR                         |
 | bit  19:0  R/W  FILT20_TAG1_MASK                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_UNUSED_MASK          0x0e800000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_TYPE_MSK 0xf0000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_TYPE_OFF 28
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_INV_MSK  0x01000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_INV_OFF  24
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_ADDR_MSK 0x00700000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_ADDR_OFF 20
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_MASK_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG1_2_BIT_FILT20_TAG1_MASK_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_1 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_1_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_1_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_1_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_1_INDEX_N_OFFSET        512

/*----------------------------------------------------------.
 | Register (0x00000048 + (N) * 512) VLANMPLS_FILT20_TAG2_1 |
 +----------------------------------------------------------+
 | bit  19:0 R/W  FILT20_TAG2_VALUE                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_1_UNUSED_MASK           0xfff00000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_1_BIT_FILT20_TAG2_VALUE_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_1_BIT_FILT20_TAG2_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_2 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_INDEX_N_MIN          0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_INDEX_N_MAX          11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_INDEX_N_SIZE         12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_INDEX_N_OFFSET       512

/*----------------------------------------------------------.
 | Register (0x0000004c + (N) * 512) VLANMPLS_FILT20_TAG2_2 |
 +----------------------------------------------------------+
 | bit  31:28 R/W  FILT20_TAG2_TYPE                         |
 | bit  24    R/W  FILT20_TAG2_INV                          |
 | bit  22:20 R/W  FILT20_TAG2_ADDR                         |
 | bit  19:0  R/W  FILT20_TAG2_MASK                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_UNUSED_MASK          0x0e800000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_TYPE_MSK 0xf0000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_TYPE_OFF 28
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_INV_MSK  0x01000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_INV_OFF  24
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_ADDR_MSK 0x00700000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_ADDR_OFF 20
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_MASK_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG2_2_BIT_FILT20_TAG2_MASK_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_1 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_1_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_1_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_1_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_1_INDEX_N_OFFSET        512

/*----------------------------------------------------------.
 | Register (0x00000050 + (N) * 512) VLANMPLS_FILT20_TAG3_1 |
 +----------------------------------------------------------+
 | bit  19:0 R/W  FILT20_TAG3_VALUE                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_1_UNUSED_MASK           0xfff00000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_1_BIT_FILT20_TAG3_VALUE_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_1_BIT_FILT20_TAG3_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_2 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_INDEX_N_MIN          0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_INDEX_N_MAX          11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_INDEX_N_SIZE         12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_INDEX_N_OFFSET       512

/*----------------------------------------------------------.
 | Register (0x00000054 + (N) * 512) VLANMPLS_FILT20_TAG3_2 |
 +----------------------------------------------------------+
 | bit  31:28 R/W  FILT20_TAG3_TYPE                         |
 | bit  24    R/W  FILT20_TAG3_INV                          |
 | bit  22:20 R/W  FILT20_TAG3_ADDR                         |
 | bit  19:0  R/W  FILT20_TAG3_MASK                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_UNUSED_MASK          0x0e800000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_TYPE_MSK 0xf0000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_TYPE_OFF 28
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_INV_MSK  0x01000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_INV_OFF  24
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_ADDR_MSK 0x00700000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_ADDR_OFF 20
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_MASK_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG3_2_BIT_FILT20_TAG3_MASK_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_1 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_1_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_1_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_1_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_1_INDEX_N_OFFSET        512

/*----------------------------------------------------------.
 | Register (0x00000058 + (N) * 512) VLANMPLS_FILT20_TAG4_1 |
 +----------------------------------------------------------+
 | bit  19:0 R/W  FILT20_TAG4_VALUE                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_1_UNUSED_MASK           0xfff00000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_1_BIT_FILT20_TAG4_VALUE_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_1_BIT_FILT20_TAG4_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_2 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_INDEX_N_MIN          0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_INDEX_N_MAX          11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_INDEX_N_SIZE         12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_INDEX_N_OFFSET       512

/*----------------------------------------------------------.
 | Register (0x0000005c + (N) * 512) VLANMPLS_FILT20_TAG4_2 |
 +----------------------------------------------------------+
 | bit  31:28 R/W  FILT20_TAG4_TYPE                         |
 | bit  24    R/W  FILT20_TAG4_INV                          |
 | bit  22:20 R/W  FILT20_TAG4_ADDR                         |
 | bit  19:0  R/W  FILT20_TAG4_MASK                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_UNUSED_MASK          0x0e800000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_TYPE_MSK 0xf0000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_TYPE_OFF 28
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_INV_MSK  0x01000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_INV_OFF  24
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_ADDR_MSK 0x00700000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_ADDR_OFF 20
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_MASK_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG4_2_BIT_FILT20_TAG4_MASK_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_1 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_1_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_1_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_1_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_1_INDEX_N_OFFSET        512

/*----------------------------------------------------------.
 | Register (0x00000060 + (N) * 512) VLANMPLS_FILT20_TAG5_1 |
 +----------------------------------------------------------+
 | bit  19:0 R/W  FILT20_TAG5_VALUE                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_1_UNUSED_MASK           0xfff00000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_1_BIT_FILT20_TAG5_VALUE_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_1_BIT_FILT20_TAG5_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_2 */
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_INDEX_N_MIN          0
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_INDEX_N_MAX          11
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_INDEX_N_SIZE         12
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_INDEX_N_OFFSET       512

/*----------------------------------------------------------.
 | Register (0x00000064 + (N) * 512) VLANMPLS_FILT20_TAG5_2 |
 +----------------------------------------------------------+
 | bit  31:28 R/W  FILT20_TAG5_TYPE                         |
 | bit  24    R/W  FILT20_TAG5_INV                          |
 | bit  22:20 R/W  FILT20_TAG5_ADDR                         |
 | bit  19:0  R/W  FILT20_TAG5_MASK                         |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_UNUSED_MASK          0x0e800000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_TYPE_MSK 0xf0000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_TYPE_OFF 28
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_INV_MSK  0x01000000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_INV_OFF  24
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_ADDR_MSK 0x00700000
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_ADDR_OFF 20
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_MASK_MSK 0x000fffff
#define ECLASS120_REG_VLANMPLS_FILT20_TAG5_2_BIT_FILT20_TAG5_MASK_OFF 0

/* index definitions for PMC_ECLASS120_REG_MAN_VLAN_CFG */
#define PMC_ECLASS120_REG_MAN_VLAN_CFG_INDEX_N_MIN                    0
#define PMC_ECLASS120_REG_MAN_VLAN_CFG_INDEX_N_MAX                    11
#define PMC_ECLASS120_REG_MAN_VLAN_CFG_INDEX_N_SIZE                   12
#define PMC_ECLASS120_REG_MAN_VLAN_CFG_INDEX_N_OFFSET                 512

/*------------------------------------------------.
 | Register (0x00000070 + (N) * 512) MAN_VLAN_CFG |
 +------------------------------------------------+
 | bit  27:16 R/W  MANAGEMENT_VLAN_TCI_VLANID     |
 | bit  15:0  R/W  MANAGEMENT_VLAN_TPID           |
 +-----------------------------------------------*/
#define PMC_ECLASS120_REG_MAN_VLAN_CFG_UNUSED_MASK                    0xf0000000
#define ECLASS120_REG_MAN_VLAN_CFG_BIT_MANAGEMENT_VLAN_TCI_VLANID_MSK 0x0fff0000
#define ECLASS120_REG_MAN_VLAN_CFG_BIT_MANAGEMENT_VLAN_TCI_VLANID_OFF 16
#define ECLASS120_REG_MAN_VLAN_CFG_BIT_MANAGEMENT_VLAN_TPID_MSK       0x0000ffff
#define ECLASS120_REG_MAN_VLAN_CFG_BIT_MANAGEMENT_VLAN_TPID_OFF       0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_ETH_LSB */
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_LSB_INDEX_N_MIN    0
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_LSB_INDEX_N_MAX    11
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_LSB_INDEX_N_SIZE   12
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_LSB_INDEX_N_OFFSET 512

/*------------------------------------------------------.
 | Register (0x00000074 + (N) * 512) UNICAST_DA_ETH_LSB |
 +------------------------------------------------------+
 | bit  31:0 R/W  ADDR_LSB                              |
 +-----------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_LSB_UNUSED_MASK    0x00000000
#define ECLASS120_REG_UNICAST_DA_ETH_LSB_BIT_ADDR_LSB_MSK   0xffffffff
#define ECLASS120_REG_UNICAST_DA_ETH_LSB_BIT_ADDR_LSB_OFF   0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_ETH_MSB */
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_MSB_INDEX_N_MIN    0
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_MSB_INDEX_N_MAX    11
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_MSB_INDEX_N_SIZE   12
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_MSB_INDEX_N_OFFSET 512

/*------------------------------------------------------.
 | Register (0x00000078 + (N) * 512) UNICAST_DA_ETH_MSB |
 +------------------------------------------------------+
 | bit  16   R/W  ADDR_EN                               |
 | bit  15:0 R/W  ADDR_MSB                              |
 +-----------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_MSB_UNUSED_MASK    0xfffe0000
#define ECLASS120_REG_UNICAST_DA_ETH_MSB_BIT_ADDR_EN_MSK    0x00010000
#define ECLASS120_REG_UNICAST_DA_ETH_MSB_BIT_ADDR_EN_OFF    16
#define ECLASS120_REG_UNICAST_DA_ETH_MSB_BIT_ADDR_MSB_MSK   0x0000ffff
#define ECLASS120_REG_UNICAST_DA_ETH_MSB_BIT_ADDR_MSB_OFF   0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB */
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB_INDEX_N_MAX      11
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB_INDEX_N_SIZE     12
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB_INDEX_N_OFFSET   512

/*----------------------------------------------------------.
 | Register (0x0000007c + (N) * 512) UNICAST_DA_ETH_P2P_LSB |
 +----------------------------------------------------------+
 | bit  31:0 R/W  ADDR_P2P_LSB                              |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB_UNUSED_MASK      0x00000000
#define ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB_BIT_ADDR_P2P_LSB_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_ETH_P2P_LSB_BIT_ADDR_P2P_LSB_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB */
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_INDEX_N_MAX      11
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_INDEX_N_SIZE     12
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_INDEX_N_OFFSET   512

/*----------------------------------------------------------.
 | Register (0x00000080 + (N) * 512) UNICAST_DA_ETH_P2P_MSB |
 +----------------------------------------------------------+
 | bit  16   R/W  ADDR_P2P_EN                               |
 | bit  15:0 R/W  ADDR_P2P_MSB                              |
 +---------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_UNUSED_MASK      0xfffe0000
#define ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_BIT_ADDR_P2P_EN_MSK  0x00010000
#define ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_BIT_ADDR_P2P_EN_OFF  16
#define ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_BIT_ADDR_P2P_MSB_MSK 0x0000ffff
#define ECLASS120_REG_UNICAST_DA_ETH_P2P_MSB_BIT_ADDR_P2P_MSB_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DEST_PORT_UDP */
#define PMC_ECLASS120_REG_UNICAST_DEST_PORT_UDP_INDEX_N_MIN       0
#define PMC_ECLASS120_REG_UNICAST_DEST_PORT_UDP_INDEX_N_MAX       11
#define PMC_ECLASS120_REG_UNICAST_DEST_PORT_UDP_INDEX_N_SIZE      12
#define PMC_ECLASS120_REG_UNICAST_DEST_PORT_UDP_INDEX_N_OFFSET    512

/*---------------------------------------------------------.
 | Register (0x00000084 + (N) * 512) UNICAST_DEST_PORT_UDP |
 +---------------------------------------------------------+
 | bit  31:16 R/W  UDP_PORT_IPV6                           |
 | bit  15:0  R/W  UDP_PORT_IPV4                           |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DEST_PORT_UDP_UNUSED_MASK       0x00000000
#define ECLASS120_REG_UNICAST_DEST_PORT_UDP_BIT_UDP_PORT_IPV6_MSK 0xffff0000
#define ECLASS120_REG_UNICAST_DEST_PORT_UDP_BIT_UDP_PORT_IPV6_OFF 16
#define ECLASS120_REG_UNICAST_DEST_PORT_UDP_BIT_UDP_PORT_IPV4_MSK 0x0000ffff
#define ECLASS120_REG_UNICAST_DEST_PORT_UDP_BIT_UDP_PORT_IPV4_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV4 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_INDEX_N_MIN    0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_INDEX_N_MAX    11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_INDEX_N_SIZE   12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_INDEX_N_OFFSET 512

/*---------------------------------------------------.
 | Register (0x00000088 + (N) * 512) UNICAST_DA_IPV4 |
 +---------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV4                          |
 +--------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_UNUSED_MASK    0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV4_BIT_ADDR_IPV4_MSK  0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV4_BIT_ADDR_IPV4_OFF  0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV4_P2P */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_P2P_INDEX_N_MIN       0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_P2P_INDEX_N_MAX       11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_P2P_INDEX_N_SIZE      12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_P2P_INDEX_N_OFFSET    512

/*-------------------------------------------------------.
 | Register (0x0000008c + (N) * 512) UNICAST_DA_IPV4_P2P |
 +-------------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV4_P2P                          |
 +------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV4_P2P_UNUSED_MASK       0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV4_P2P_BIT_ADDR_IPV4_P2P_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV4_P2P_BIT_ADDR_IPV4_P2P_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_0 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_0_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_0_INDEX_N_MAX     11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_0_INDEX_N_SIZE    12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_0_INDEX_N_OFFSET  512

/*-----------------------------------------------------.
 | Register (0x00000090 + (N) * 512) UNICAST_DA_IPV6_0 |
 +-----------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_0                          |
 +----------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_0_UNUSED_MASK     0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_0_BIT_ADDR_IPV6_0_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_0_BIT_ADDR_IPV6_0_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_1 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_1_INDEX_N_MAX     11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_1_INDEX_N_SIZE    12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_1_INDEX_N_OFFSET  512

/*-----------------------------------------------------.
 | Register (0x00000094 + (N) * 512) UNICAST_DA_IPV6_1 |
 +-----------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_1                          |
 +----------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_1_BIT_ADDR_IPV6_1_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_1_BIT_ADDR_IPV6_1_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_2 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_2_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_2_INDEX_N_MAX     11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_2_INDEX_N_SIZE    12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_2_INDEX_N_OFFSET  512

/*-----------------------------------------------------.
 | Register (0x00000098 + (N) * 512) UNICAST_DA_IPV6_2 |
 +-----------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_2                          |
 +----------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_2_UNUSED_MASK     0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_2_BIT_ADDR_IPV6_2_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_2_BIT_ADDR_IPV6_2_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_3 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_3_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_3_INDEX_N_MAX     11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_3_INDEX_N_SIZE    12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_3_INDEX_N_OFFSET  512

/*-----------------------------------------------------.
 | Register (0x0000009c + (N) * 512) UNICAST_DA_IPV6_3 |
 +-----------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_3                          |
 +----------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_3_UNUSED_MASK     0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_3_BIT_ADDR_IPV6_3_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_3_BIT_ADDR_IPV6_3_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_0 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_0_INDEX_N_MIN         0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_0_INDEX_N_MAX         11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_0_INDEX_N_SIZE        12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_0_INDEX_N_OFFSET      512

/*---------------------------------------------------------.
 | Register (0x000000a4 + (N) * 512) UNICAST_DA_IPV6_P2P_0 |
 +---------------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_P2P_0                          |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_0_UNUSED_MASK         0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_0_BIT_ADDR_IPV6_P2P_0_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_0_BIT_ADDR_IPV6_P2P_0_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_1 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_1_INDEX_N_MIN         0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_1_INDEX_N_MAX         11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_1_INDEX_N_SIZE        12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_1_INDEX_N_OFFSET      512

/*---------------------------------------------------------.
 | Register (0x000000a8 + (N) * 512) UNICAST_DA_IPV6_P2P_1 |
 +---------------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_P2P_1                          |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_1_UNUSED_MASK         0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_1_BIT_ADDR_IPV6_P2P_1_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_1_BIT_ADDR_IPV6_P2P_1_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_2 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_2_INDEX_N_MIN         0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_2_INDEX_N_MAX         11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_2_INDEX_N_SIZE        12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_2_INDEX_N_OFFSET      512

/*---------------------------------------------------------.
 | Register (0x000000ac + (N) * 512) UNICAST_DA_IPV6_P2P_2 |
 +---------------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_P2P_2                          |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_2_UNUSED_MASK         0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_2_BIT_ADDR_IPV6_P2P_2_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_2_BIT_ADDR_IPV6_P2P_2_OFF 0

/* index definitions for PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_3 */
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_3_INDEX_N_MIN         0
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_3_INDEX_N_MAX         11
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_3_INDEX_N_SIZE        12
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_3_INDEX_N_OFFSET      512

/*---------------------------------------------------------.
 | Register (0x000000b0 + (N) * 512) UNICAST_DA_IPV6_P2P_3 |
 +---------------------------------------------------------+
 | bit  31:0 R/W  ADDR_IPV6_P2P_3                          |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_UNICAST_DA_IPV6_P2P_3_UNUSED_MASK         0x00000000
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_3_BIT_ADDR_IPV6_P2P_3_MSK 0xffffffff
#define ECLASS120_REG_UNICAST_DA_IPV6_P2P_3_BIT_ADDR_IPV6_P2P_3_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_0 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_0_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_0_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_0_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_0_INDEX_N_OFFSET         512

/*---------------------------------------------------------.
 | Register (0x000000d0 + (N) * 512) GEN_FILTER_CFG_FAND_0 |
 +---------------------------------------------------------+
 | bit  31:29 R    FAND_0_UNUSEDZEROS                      |
 | bit  16    R/W  FAND_0_OR_CFG                           |
 | bit  15:0  R/W  FAND_0_F16BITS_EN                       |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_0_UNUSED_MASK            0x1ffe0000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_0_BIT_FAND_0_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_0_BIT_FAND_0_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_0_BIT_FAND_0_OR_CFG_MSK      0x00010000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_0_BIT_FAND_0_OR_CFG_OFF      16
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_0_BIT_FAND_0_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_0_BIT_FAND_0_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_1_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_1_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_1_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_1_INDEX_N_OFFSET         512

/*---------------------------------------------------------.
 | Register (0x000000d4 + (N) * 512) GEN_FILTER_CFG_FAND_1 |
 +---------------------------------------------------------+
 | bit  31:29 R    FAND_1_UNUSEDZEROS                      |
 | bit  16    R/W  FAND_1_OR_CFG                           |
 | bit  15:0  R/W  FAND_1_F16BITS_EN                       |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_1_UNUSED_MASK            0x1ffe0000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_1_BIT_FAND_1_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_1_BIT_FAND_1_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_1_BIT_FAND_1_OR_CFG_MSK      0x00010000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_1_BIT_FAND_1_OR_CFG_OFF      16
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_1_BIT_FAND_1_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_1_BIT_FAND_1_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_2_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_2_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_2_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_2_INDEX_N_OFFSET         512

/*---------------------------------------------------------.
 | Register (0x000000d8 + (N) * 512) GEN_FILTER_CFG_FAND_2 |
 +---------------------------------------------------------+
 | bit  31:29 R    FAND_2_UNUSEDZEROS                      |
 | bit  16    R/W  FAND_2_OR_CFG                           |
 | bit  15:0  R/W  FAND_2_F16BITS_EN                       |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_2_UNUSED_MASK            0x1ffe0000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_2_BIT_FAND_2_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_2_BIT_FAND_2_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_2_BIT_FAND_2_OR_CFG_MSK      0x00010000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_2_BIT_FAND_2_OR_CFG_OFF      16
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_2_BIT_FAND_2_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_2_BIT_FAND_2_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_3 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_3_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_3_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_3_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_3_INDEX_N_OFFSET         512

/*---------------------------------------------------------.
 | Register (0x000000dc + (N) * 512) GEN_FILTER_CFG_FAND_3 |
 +---------------------------------------------------------+
 | bit  31:29 R    FAND_3_UNUSEDZEROS                      |
 | bit  16    R/W  FAND_3_OR_CFG                           |
 | bit  15:0  R/W  FAND_3_F16BITS_EN                       |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FAND_3_UNUSED_MASK            0x1ffe0000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_3_BIT_FAND_3_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_3_BIT_FAND_3_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_3_BIT_FAND_3_OR_CFG_MSK      0x00010000
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_3_BIT_FAND_3_OR_CFG_OFF      16
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_3_BIT_FAND_3_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FAND_3_BIT_FAND_3_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_0 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_0_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_0_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_0_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_0_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000e0 + (N) * 512) GEN_FILTER_CFG_FOR_0 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_0_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_0_INV                              |
 | bit  19:16 R/W  FOR_0_FAND_EN                          |
 | bit  15:0  R/W  FOR_0_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_0_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_0_BIT_FOR_0_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_1_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_1_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_1_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_1_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000e4 + (N) * 512) GEN_FILTER_CFG_FOR_1 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_1_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_1_INV                              |
 | bit  19:16 R/W  FOR_1_FAND_EN                          |
 | bit  15:0  R/W  FOR_1_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_1_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_1_BIT_FOR_1_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_2_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_2_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_2_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_2_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000e8 + (N) * 512) GEN_FILTER_CFG_FOR_2 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_2_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_2_INV                              |
 | bit  19:16 R/W  FOR_2_FAND_EN                          |
 | bit  15:0  R/W  FOR_2_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_2_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_2_BIT_FOR_2_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_3 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_3_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_3_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_3_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_3_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000ec + (N) * 512) GEN_FILTER_CFG_FOR_3 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_3_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_3_INV                              |
 | bit  19:16 R/W  FOR_3_FAND_EN                          |
 | bit  15:0  R/W  FOR_3_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_3_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_3_BIT_FOR_3_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_4 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_4_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_4_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_4_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_4_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000f0 + (N) * 512) GEN_FILTER_CFG_FOR_4 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_4_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_4_INV                              |
 | bit  19:16 R/W  FOR_4_FAND_EN                          |
 | bit  15:0  R/W  FOR_4_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_4_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_4_BIT_FOR_4_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_5 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_5_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_5_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_5_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_5_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000f4 + (N) * 512) GEN_FILTER_CFG_FOR_5 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_5_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_5_INV                              |
 | bit  19:16 R/W  FOR_5_FAND_EN                          |
 | bit  15:0  R/W  FOR_5_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_5_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_5_BIT_FOR_5_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_6 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_6_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_6_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_6_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_6_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000f8 + (N) * 512) GEN_FILTER_CFG_FOR_6 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_6_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_6_INV                              |
 | bit  19:16 R/W  FOR_6_FAND_EN                          |
 | bit  15:0  R/W  FOR_6_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_6_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_6_BIT_FOR_6_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_7 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_7_INDEX_N_MIN           0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_7_INDEX_N_MAX           11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_7_INDEX_N_SIZE          12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_7_INDEX_N_OFFSET        512

/*--------------------------------------------------------.
 | Register (0x000000fc + (N) * 512) GEN_FILTER_CFG_FOR_7 |
 +--------------------------------------------------------+
 | bit  31:29 R    FOR_7_UNUSEDZEROS                      |
 | bit  20    R/W  FOR_7_INV                              |
 | bit  19:16 R/W  FOR_7_FAND_EN                          |
 | bit  15:0  R/W  FOR_7_F16BITS_EN                       |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FOR_7_UNUSED_MASK           0x1fe00000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_INV_MSK         0x00100000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_INV_OFF         20
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FOR_7_BIT_FOR_7_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x00000110 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_0 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_0_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_0_EN                                     |
 | bit  27:20 R/W  FOUT_0_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_0_FAND_EN                                |
 | bit  15:0  R/W  FOUT_0_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_0_BIT_FOUT_0_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x00000114 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_1 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_1_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_1_EN                                     |
 | bit  27:20 R/W  FOUT_1_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_1_FAND_EN                                |
 | bit  15:0  R/W  FOUT_1_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_1_BIT_FOUT_1_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x00000118 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_2 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_2_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_2_EN                                     |
 | bit  27:20 R/W  FOUT_2_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_2_FAND_EN                                |
 | bit  15:0  R/W  FOUT_2_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_2_BIT_FOUT_2_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x0000011c + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_3 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_3_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_3_EN                                     |
 | bit  27:20 R/W  FOUT_3_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_3_FAND_EN                                |
 | bit  15:0  R/W  FOUT_3_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_3_BIT_FOUT_3_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x00000120 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_4 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_4_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_4_EN                                     |
 | bit  27:20 R/W  FOUT_4_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_4_FAND_EN                                |
 | bit  15:0  R/W  FOUT_4_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_4_BIT_FOUT_4_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x00000124 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_5 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_5_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_5_EN                                     |
 | bit  27:20 R/W  FOUT_5_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_5_FAND_EN                                |
 | bit  15:0  R/W  FOUT_5_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_5_BIT_FOUT_5_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x00000128 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_6 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_6_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_6_EN                                     |
 | bit  27:20 R/W  FOUT_6_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_6_FAND_EN                                |
 | bit  15:0  R/W  FOUT_6_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_6_BIT_FOUT_6_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_INDEX_N_MIN            0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_INDEX_N_MAX            11
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_INDEX_N_SIZE           12
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_INDEX_N_OFFSET         512

/*---------------------------------------------------------------.
 | Register (0x0000012c + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_7 |
 +---------------------------------------------------------------+
 | bit  31:29 R    FOUT_7_UNUSEDZEROS                            |
 | bit  28    R/W  FOUT_7_EN                                     |
 | bit  27:20 R/W  FOUT_7_FOR_EN                                 |
 | bit  19:16 R/W  FOUT_7_FAND_EN                                |
 | bit  15:0  R/W  FOUT_7_F16BITS_EN                             |
 +--------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_UNUSED_MASK            0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_UNUSEDZEROS_MSK 0xe0000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_UNUSEDZEROS_OFF 29
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_EN_MSK          0x10000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_EN_OFF          28
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_FOR_EN_MSK      0x0ff00000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_FOR_EN_OFF      20
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_FAND_EN_MSK     0x000f0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_FAND_EN_OFF     16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_F16BITS_EN_MSK  0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER_OUT_7_BIT_FOUT_7_F16BITS_EN_OFF  0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_0 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_0_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_0_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_0_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_0_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000140 + (N) * 512) ROUTING_AND_FILTER_0 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND0_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND0_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_0_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_0_BIT_ROUTING_AND0_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_0_BIT_ROUTING_AND0_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_0_BIT_ROUTING_AND0_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_0_BIT_ROUTING_AND0_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_1 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_1_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_1_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_1_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_1_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000144 + (N) * 512) ROUTING_AND_FILTER_1 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND1_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND1_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_1_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_1_BIT_ROUTING_AND1_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_1_BIT_ROUTING_AND1_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_1_BIT_ROUTING_AND1_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_1_BIT_ROUTING_AND1_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_2 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_2_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_2_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_2_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_2_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000148 + (N) * 512) ROUTING_AND_FILTER_2 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND2_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND2_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_2_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_2_BIT_ROUTING_AND2_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_2_BIT_ROUTING_AND2_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_2_BIT_ROUTING_AND2_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_2_BIT_ROUTING_AND2_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_3 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_3_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_3_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_3_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_3_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x0000014c + (N) * 512) ROUTING_AND_FILTER_3 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND3_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND3_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_3_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_3_BIT_ROUTING_AND3_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_3_BIT_ROUTING_AND3_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_3_BIT_ROUTING_AND3_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_3_BIT_ROUTING_AND3_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_4 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_4_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_4_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_4_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_4_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000150 + (N) * 512) ROUTING_AND_FILTER_4 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND4_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND4_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_4_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_4_BIT_ROUTING_AND4_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_4_BIT_ROUTING_AND4_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_4_BIT_ROUTING_AND4_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_4_BIT_ROUTING_AND4_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_5 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_5_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_5_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_5_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_5_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000154 + (N) * 512) ROUTING_AND_FILTER_5 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND5_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND5_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_5_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_5_BIT_ROUTING_AND5_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_5_BIT_ROUTING_AND5_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_5_BIT_ROUTING_AND5_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_5_BIT_ROUTING_AND5_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_6 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_6_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_6_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_6_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_6_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000158 + (N) * 512) ROUTING_AND_FILTER_6 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND6_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND6_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_6_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_6_BIT_ROUTING_AND6_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_6_BIT_ROUTING_AND6_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_6_BIT_ROUTING_AND6_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_6_BIT_ROUTING_AND6_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_7 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_7_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_7_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_7_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_7_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x0000015c + (N) * 512) ROUTING_AND_FILTER_7 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND7_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND7_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_7_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_7_BIT_ROUTING_AND7_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_7_BIT_ROUTING_AND7_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_7_BIT_ROUTING_AND7_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_7_BIT_ROUTING_AND7_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_8 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_8_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_8_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_8_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_8_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000160 + (N) * 512) ROUTING_AND_FILTER_8 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND8_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND8_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_8_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_8_BIT_ROUTING_AND8_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_8_BIT_ROUTING_AND8_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_8_BIT_ROUTING_AND8_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_8_BIT_ROUTING_AND8_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_9 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_9_INDEX_N_MIN                        0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_9_INDEX_N_MAX                        11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_9_INDEX_N_SIZE                       12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_9_INDEX_N_OFFSET                     512

/*--------------------------------------------------------.
 | Register (0x00000164 + (N) * 512) ROUTING_AND_FILTER_9 |
 +--------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND9_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND9_INPUT_GEN_EN              |
 +-------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_9_UNUSED_MASK                        0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_9_BIT_ROUTING_AND9_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_9_BIT_ROUTING_AND9_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_9_BIT_ROUTING_AND9_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_9_BIT_ROUTING_AND9_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_10 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_10_INDEX_N_MIN                         0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_10_INDEX_N_MAX                         11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_10_INDEX_N_SIZE                        12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_10_INDEX_N_OFFSET                      512

/*---------------------------------------------------------.
 | Register (0x00000168 + (N) * 512) ROUTING_AND_FILTER_10 |
 +---------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND10_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND10_INPUT_GEN_EN              |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_10_UNUSED_MASK                         0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_10_BIT_ROUTING_AND10_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_10_BIT_ROUTING_AND10_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_10_BIT_ROUTING_AND10_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_10_BIT_ROUTING_AND10_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_11 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_11_INDEX_N_MIN                         0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_11_INDEX_N_MAX                         11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_11_INDEX_N_SIZE                        12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_11_INDEX_N_OFFSET                      512

/*---------------------------------------------------------.
 | Register (0x0000016c + (N) * 512) ROUTING_AND_FILTER_11 |
 +---------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND11_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND11_INPUT_GEN_EN              |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_11_UNUSED_MASK                         0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_11_BIT_ROUTING_AND11_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_11_BIT_ROUTING_AND11_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_11_BIT_ROUTING_AND11_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_11_BIT_ROUTING_AND11_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_12 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_12_INDEX_N_MIN                         0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_12_INDEX_N_MAX                         11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_12_INDEX_N_SIZE                        12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_12_INDEX_N_OFFSET                      512

/*---------------------------------------------------------.
 | Register (0x00000170 + (N) * 512) ROUTING_AND_FILTER_12 |
 +---------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND12_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND12_INPUT_GEN_EN              |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_12_UNUSED_MASK                         0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_12_BIT_ROUTING_AND12_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_12_BIT_ROUTING_AND12_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_12_BIT_ROUTING_AND12_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_12_BIT_ROUTING_AND12_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_13 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_13_INDEX_N_MIN                         0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_13_INDEX_N_MAX                         11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_13_INDEX_N_SIZE                        12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_13_INDEX_N_OFFSET                      512

/*---------------------------------------------------------.
 | Register (0x00000174 + (N) * 512) ROUTING_AND_FILTER_13 |
 +---------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND13_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND13_INPUT_GEN_EN              |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_13_UNUSED_MASK                         0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_13_BIT_ROUTING_AND13_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_13_BIT_ROUTING_AND13_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_13_BIT_ROUTING_AND13_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_13_BIT_ROUTING_AND13_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_14 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_14_INDEX_N_MIN                         0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_14_INDEX_N_MAX                         11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_14_INDEX_N_SIZE                        12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_14_INDEX_N_OFFSET                      512

/*---------------------------------------------------------.
 | Register (0x00000178 + (N) * 512) ROUTING_AND_FILTER_14 |
 +---------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND14_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND14_INPUT_GEN_EN              |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_14_UNUSED_MASK                         0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_14_BIT_ROUTING_AND14_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_14_BIT_ROUTING_AND14_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_14_BIT_ROUTING_AND14_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_14_BIT_ROUTING_AND14_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_15 */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_15_INDEX_N_MIN                         0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_15_INDEX_N_MAX                         11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_15_INDEX_N_SIZE                        12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_15_INDEX_N_OFFSET                      512

/*---------------------------------------------------------.
 | Register (0x0000017c + (N) * 512) ROUTING_AND_FILTER_15 |
 +---------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_AND15_INPUT_VLANMPLS_EN         |
 | bit  25:0  R/W  ROUTING_AND15_INPUT_GEN_EN              |
 +--------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_15_UNUSED_MASK                         0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_15_BIT_ROUTING_AND15_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_AND_FILTER_15_BIT_ROUTING_AND15_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_AND_FILTER_15_BIT_ROUTING_AND15_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_AND_FILTER_15_BIT_ROUTING_AND15_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW */
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_INDEX_N_MIN                      0
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_INDEX_N_MAX                      11
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_INDEX_N_SIZE                     12
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_INDEX_N_OFFSET                   512

/*-----------------------------------------------------------------------------.
 | Register (0x0000018c + (N) * 512) ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW |
 +-----------------------------------------------------------------------------+
 | bit  31:16 R/W  ROUTING_AND_MPLS_OVERFLOW_EN                                |
 | bit  15:0  R/W  ROUTING_AND_NO_MATCH_EN                                     |
 +----------------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_UNUSED_MASK                      0x00000000
#define ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_BIT_ROUTING_AND_MPLS_OVERFLOW_EN_MSK 0xffff0000
#define ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_BIT_ROUTING_AND_MPLS_OVERFLOW_EN_OFF 16
#define ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_BIT_ROUTING_AND_NO_MATCH_EN_MSK      0x0000ffff
#define ECLASS120_REG_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_BIT_ROUTING_AND_NO_MATCH_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG1 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_INDEX_N_MIN                       0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_INDEX_N_MAX                       11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_INDEX_N_SIZE                      12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_INDEX_N_OFFSET                    512

/*------------------------------------------------------------.
 | Register (0x000001a0 + (N) * 512) ROUTING_OR_FILTER_0_REG1 |
 +------------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_OR0_INPUT_VLANMPLS_EN              |
 | bit  25:0  R/W  ROUTING_OR0_INPUT_GEN_EN                   |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_UNUSED_MASK                       0x00000000
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_BIT_ROUTING_OR0_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_BIT_ROUTING_OR0_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_BIT_ROUTING_OR0_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG1_BIT_ROUTING_OR0_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG2 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_INDEX_N_MIN                            0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_INDEX_N_MAX                            11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_INDEX_N_SIZE                           12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_INDEX_N_OFFSET                         512

/*------------------------------------------------------------.
 | Register (0x000001a4 + (N) * 512) ROUTING_OR_FILTER_0_REG2 |
 +------------------------------------------------------------+
 | bit  31   R/W  ROUTING_OR0_OUTPUT_EN                       |
 | bit  28   R/W  ROUTING_OR0_OUTPUT_INV                      |
 | bit  20   R/W  ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN          |
 | bit  16   R/W  ROUTING_OR0_INPUT_NO_MATCH_ALL_EN           |
 | bit  15:0 R/W  ROUTING_OR0_INPUT_ANDOUT_EN                 |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_UNUSED_MASK                            0x6fee0000
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_OUTPUT_EN_MSK              0x80000000
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_OUTPUT_EN_OFF              31
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_OUTPUT_INV_MSK             0x10000000
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_OUTPUT_INV_OFF             28
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN_MSK 0x00100000
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN_OFF 20
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_INPUT_NO_MATCH_ALL_EN_MSK  0x00010000
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_INPUT_NO_MATCH_ALL_EN_OFF  16
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_INPUT_ANDOUT_EN_MSK        0x0000ffff
#define ECLASS120_REG_ROUTING_OR_FILTER_0_REG2_BIT_ROUTING_OR0_INPUT_ANDOUT_EN_OFF        0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG1 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_INDEX_N_MIN                       0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_INDEX_N_MAX                       11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_INDEX_N_SIZE                      12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_INDEX_N_OFFSET                    512

/*------------------------------------------------------------.
 | Register (0x000001a8 + (N) * 512) ROUTING_OR_FILTER_1_REG1 |
 +------------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_OR1_INPUT_VLANMPLS_EN              |
 | bit  25:0  R/W  ROUTING_OR1_INPUT_GEN_EN                   |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_UNUSED_MASK                       0x00000000
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_BIT_ROUTING_OR1_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_BIT_ROUTING_OR1_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_BIT_ROUTING_OR1_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG1_BIT_ROUTING_OR1_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG2 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_INDEX_N_MIN                            0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_INDEX_N_MAX                            11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_INDEX_N_SIZE                           12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_INDEX_N_OFFSET                         512

/*------------------------------------------------------------.
 | Register (0x000001ac + (N) * 512) ROUTING_OR_FILTER_1_REG2 |
 +------------------------------------------------------------+
 | bit  31   R/W  ROUTING_OR1_OUTPUT_EN                       |
 | bit  28   R/W  ROUTING_OR1_OUTPUT_INV                      |
 | bit  20   R/W  ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN          |
 | bit  16   R/W  ROUTING_OR1_INPUT_NO_MATCH_ALL_EN           |
 | bit  15:0 R/W  ROUTING_OR1_INPUT_ANDOUT_EN                 |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_UNUSED_MASK                            0x6fee0000
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_OUTPUT_EN_MSK              0x80000000
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_OUTPUT_EN_OFF              31
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_OUTPUT_INV_MSK             0x10000000
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_OUTPUT_INV_OFF             28
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN_MSK 0x00100000
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN_OFF 20
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_INPUT_NO_MATCH_ALL_EN_MSK  0x00010000
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_INPUT_NO_MATCH_ALL_EN_OFF  16
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_INPUT_ANDOUT_EN_MSK        0x0000ffff
#define ECLASS120_REG_ROUTING_OR_FILTER_1_REG2_BIT_ROUTING_OR1_INPUT_ANDOUT_EN_OFF        0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG1 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_INDEX_N_MIN                       0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_INDEX_N_MAX                       11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_INDEX_N_SIZE                      12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_INDEX_N_OFFSET                    512

/*------------------------------------------------------------.
 | Register (0x000001b0 + (N) * 512) ROUTING_OR_FILTER_2_REG1 |
 +------------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_OR2_INPUT_VLANMPLS_EN              |
 | bit  25:0  R/W  ROUTING_OR2_INPUT_GEN_EN                   |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_UNUSED_MASK                       0x00000000
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_BIT_ROUTING_OR2_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_BIT_ROUTING_OR2_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_BIT_ROUTING_OR2_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG1_BIT_ROUTING_OR2_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG2 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_INDEX_N_MIN                            0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_INDEX_N_MAX                            11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_INDEX_N_SIZE                           12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_INDEX_N_OFFSET                         512

/*------------------------------------------------------------.
 | Register (0x000001b4 + (N) * 512) ROUTING_OR_FILTER_2_REG2 |
 +------------------------------------------------------------+
 | bit  31   R/W  ROUTING_OR2_OUTPUT_EN                       |
 | bit  28   R/W  ROUTING_OR2_OUTPUT_INV                      |
 | bit  20   R/W  ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN          |
 | bit  16   R/W  ROUTING_OR2_INPUT_NO_MATCH_ALL_EN           |
 | bit  15:0 R/W  ROUTING_OR2_INPUT_ANDOUT_EN                 |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_UNUSED_MASK                            0x6fee0000
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_OUTPUT_EN_MSK              0x80000000
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_OUTPUT_EN_OFF              31
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_OUTPUT_INV_MSK             0x10000000
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_OUTPUT_INV_OFF             28
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN_MSK 0x00100000
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN_OFF 20
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_INPUT_NO_MATCH_ALL_EN_MSK  0x00010000
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_INPUT_NO_MATCH_ALL_EN_OFF  16
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_INPUT_ANDOUT_EN_MSK        0x0000ffff
#define ECLASS120_REG_ROUTING_OR_FILTER_2_REG2_BIT_ROUTING_OR2_INPUT_ANDOUT_EN_OFF        0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG1 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_INDEX_N_MIN                       0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_INDEX_N_MAX                       11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_INDEX_N_SIZE                      12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_INDEX_N_OFFSET                    512

/*------------------------------------------------------------.
 | Register (0x000001b8 + (N) * 512) ROUTING_OR_FILTER_3_REG1 |
 +------------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_OR3_INPUT_VLANMPLS_EN              |
 | bit  25:0  R/W  ROUTING_OR3_INPUT_GEN_EN                   |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_UNUSED_MASK                       0x00000000
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_BIT_ROUTING_OR3_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_BIT_ROUTING_OR3_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_BIT_ROUTING_OR3_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG1_BIT_ROUTING_OR3_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG2 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_INDEX_N_MIN                            0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_INDEX_N_MAX                            11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_INDEX_N_SIZE                           12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_INDEX_N_OFFSET                         512

/*------------------------------------------------------------.
 | Register (0x000001bc + (N) * 512) ROUTING_OR_FILTER_3_REG2 |
 +------------------------------------------------------------+
 | bit  31   R/W  ROUTING_OR3_OUTPUT_EN                       |
 | bit  28   R/W  ROUTING_OR3_OUTPUT_INV                      |
 | bit  20   R/W  ROUTING_OR3_INPUT_MPLS_OVERFLOW_EN          |
 | bit  16   R/W  ROUTING_OR3_INPUT_NO_MATCH_ALL_EN           |
 | bit  15:0 R/W  ROUTING_OR3_INPUT_ANDOUT_EN                 |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_UNUSED_MASK                            0x6fee0000
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_OUTPUT_EN_MSK              0x80000000
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_OUTPUT_EN_OFF              31
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_OUTPUT_INV_MSK             0x10000000
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_OUTPUT_INV_OFF             28
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_INPUT_MPLS_OVERFLOW_EN_MSK 0x00100000
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_INPUT_MPLS_OVERFLOW_EN_OFF 20
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_INPUT_NO_MATCH_ALL_EN_MSK  0x00010000
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_INPUT_NO_MATCH_ALL_EN_OFF  16
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_INPUT_ANDOUT_EN_MSK        0x0000ffff
#define ECLASS120_REG_ROUTING_OR_FILTER_3_REG2_BIT_ROUTING_OR3_INPUT_ANDOUT_EN_OFF        0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG1 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_INDEX_N_MIN                       0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_INDEX_N_MAX                       11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_INDEX_N_SIZE                      12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_INDEX_N_OFFSET                    512

/*------------------------------------------------------------.
 | Register (0x000001c0 + (N) * 512) ROUTING_OR_FILTER_4_REG1 |
 +------------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_OR4_INPUT_VLANMPLS_EN              |
 | bit  25:0  R/W  ROUTING_OR4_INPUT_GEN_EN                   |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_UNUSED_MASK                       0x00000000
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_BIT_ROUTING_OR4_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_BIT_ROUTING_OR4_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_BIT_ROUTING_OR4_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG1_BIT_ROUTING_OR4_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG2 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_INDEX_N_MIN                            0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_INDEX_N_MAX                            11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_INDEX_N_SIZE                           12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_INDEX_N_OFFSET                         512

/*------------------------------------------------------------.
 | Register (0x000001c4 + (N) * 512) ROUTING_OR_FILTER_4_REG2 |
 +------------------------------------------------------------+
 | bit  31   R/W  ROUTING_OR4_OUTPUT_EN                       |
 | bit  28   R/W  ROUTING_OR4_OUTPUT_INV                      |
 | bit  20   R/W  ROUTING_OR4_INPUT_MPLS_OVERFLOW_EN          |
 | bit  16   R/W  ROUTING_OR4_INPUT_NO_MATCH_ALL_EN           |
 | bit  15:0 R/W  ROUTING_OR4_INPUT_ANDOUT_EN                 |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_UNUSED_MASK                            0x6fee0000
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_OUTPUT_EN_MSK              0x80000000
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_OUTPUT_EN_OFF              31
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_OUTPUT_INV_MSK             0x10000000
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_OUTPUT_INV_OFF             28
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_INPUT_MPLS_OVERFLOW_EN_MSK 0x00100000
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_INPUT_MPLS_OVERFLOW_EN_OFF 20
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_INPUT_NO_MATCH_ALL_EN_MSK  0x00010000
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_INPUT_NO_MATCH_ALL_EN_OFF  16
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_INPUT_ANDOUT_EN_MSK        0x0000ffff
#define ECLASS120_REG_ROUTING_OR_FILTER_4_REG2_BIT_ROUTING_OR4_INPUT_ANDOUT_EN_OFF        0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG1 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_INDEX_N_MIN                       0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_INDEX_N_MAX                       11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_INDEX_N_SIZE                      12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_INDEX_N_OFFSET                    512

/*------------------------------------------------------------.
 | Register (0x000001c8 + (N) * 512) ROUTING_OR_FILTER_5_REG1 |
 +------------------------------------------------------------+
 | bit  31:26 R/W  ROUTING_OR5_INPUT_VLANMPLS_EN              |
 | bit  25:0  R/W  ROUTING_OR5_INPUT_GEN_EN                   |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_UNUSED_MASK                       0x00000000
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_BIT_ROUTING_OR5_INPUT_VLANMPLS_EN_MSK 0xfc000000
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_BIT_ROUTING_OR5_INPUT_VLANMPLS_EN_OFF 26
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_BIT_ROUTING_OR5_INPUT_GEN_EN_MSK      0x03ffffff
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG1_BIT_ROUTING_OR5_INPUT_GEN_EN_OFF      0

/* index definitions for PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG2 */
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_INDEX_N_MIN                            0
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_INDEX_N_MAX                            11
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_INDEX_N_SIZE                           12
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_INDEX_N_OFFSET                         512

/*------------------------------------------------------------.
 | Register (0x000001cc + (N) * 512) ROUTING_OR_FILTER_5_REG2 |
 +------------------------------------------------------------+
 | bit  31   R/W  ROUTING_OR5_OUTPUT_EN                       |
 | bit  28   R/W  ROUTING_OR5_OUTPUT_INV                      |
 | bit  20   R/W  ROUTING_OR5_INPUT_MPLS_OVERFLOW_EN          |
 | bit  16   R/W  ROUTING_OR5_INPUT_NO_MATCH_ALL_EN           |
 | bit  15:0 R/W  ROUTING_OR5_INPUT_ANDOUT_EN                 |
 +-----------------------------------------------------------*/
#define PMC_ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_UNUSED_MASK                            0x6fee0000
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_OUTPUT_EN_MSK              0x80000000
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_OUTPUT_EN_OFF              31
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_OUTPUT_INV_MSK             0x10000000
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_OUTPUT_INV_OFF             28
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_INPUT_MPLS_OVERFLOW_EN_MSK 0x00100000
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_INPUT_MPLS_OVERFLOW_EN_OFF 20
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_INPUT_NO_MATCH_ALL_EN_MSK  0x00010000
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_INPUT_NO_MATCH_ALL_EN_OFF  16
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_INPUT_ANDOUT_EN_MSK        0x0000ffff
#define ECLASS120_REG_ROUTING_OR_FILTER_5_REG2_BIT_ROUTING_OR5_INPUT_ANDOUT_EN_OFF        0

/* index definitions for PMC_ECLASS120_REG_LINK_ECO */
#define PMC_ECLASS120_REG_LINK_ECO_INDEX_N_MIN    0
#define PMC_ECLASS120_REG_LINK_ECO_INDEX_N_MAX    11
#define PMC_ECLASS120_REG_LINK_ECO_INDEX_N_SIZE   12
#define PMC_ECLASS120_REG_LINK_ECO_INDEX_N_OFFSET 512

/*--------------------------------------------.
 | Register (0x000001f0 + (N) * 512) LINK_ECO |
 +--------------------------------------------+
 | bit  31:0 R/W  LINK_ECO                    |
 +-------------------------------------------*/
#define PMC_ECLASS120_REG_LINK_ECO_UNUSED_MASK    0x00000000
#define ECLASS120_REG_LINK_ECO_BIT_LINK_ECO_MSK   0xffffffff
#define ECLASS120_REG_LINK_ECO_BIT_LINK_ECO_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_CFG_HWIRED_FILTER */
#define PMC_ECLASS120_REG_GEN_CFG_HWIRED_FILTER_INDEX_N_MIN                0
#define PMC_ECLASS120_REG_GEN_CFG_HWIRED_FILTER_INDEX_N_MAX                11
#define PMC_ECLASS120_REG_GEN_CFG_HWIRED_FILTER_INDEX_N_SIZE               12
#define PMC_ECLASS120_REG_GEN_CFG_HWIRED_FILTER_INDEX_N_OFFSET             4

/*-------------------------------------------------------.
 | Register (0x00001b00 + (N) * 4) GEN_CFG_HWIRED_FILTER |
 +-------------------------------------------------------+
 | bit  31  R/W  ONE_WORD_GENCLASS_EN                    |
 | bit  28  R/W  ADDR_IPV6_P2P_UCAST_EN                  |
 | bit  24  R/W  ADDR_IPV6_UCAST_EN                      |
 | bit  20  R/W  ADDR_IPV4_P2P_UCAST_EN                  |
 | bit  16  R/W  ADDR_IPV4_UCAST_EN                      |
 | bit  12  R/W  USE_UNICAST_NDP                         |
 | bit  7:4 R/W  PTP_VER_VALUE                           |
 | bit  0   R/W  PTP_VER_CHECK_EN                        |
 +------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_CFG_HWIRED_FILTER_UNUSED_MASK                0x6eeeef0e
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ONE_WORD_GENCLASS_EN_MSK   0x80000000
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ONE_WORD_GENCLASS_EN_OFF   31
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV6_P2P_UCAST_EN_MSK 0x10000000
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV6_P2P_UCAST_EN_OFF 28
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV6_UCAST_EN_MSK     0x01000000
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV6_UCAST_EN_OFF     24
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV4_P2P_UCAST_EN_MSK 0x00100000
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV4_P2P_UCAST_EN_OFF 20
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV4_UCAST_EN_MSK     0x00010000
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_ADDR_IPV4_UCAST_EN_OFF     16
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_USE_UNICAST_NDP_MSK        0x00001000
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_USE_UNICAST_NDP_OFF        12
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_PTP_VER_VALUE_MSK          0x000000f0
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_PTP_VER_VALUE_OFF          4
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_PTP_VER_CHECK_EN_MSK       0x00000001
#define ECLASS120_REG_GEN_CFG_HWIRED_FILTER_BIT_PTP_VER_CHECK_EN_OFF       0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002000 + (N) * 128) GEN_FILTER_CFG_FILTER16_0_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_0_MASK                                        |
 | bit  15:0  R/W  F16_0_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_BIT_F16_0_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_BIT_F16_0_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_BIT_F16_0_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_1_BIT_F16_0_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x00002004 + (N) * 128) GEN_FILTER_CFG_FILTER16_0_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_0_INV                                           |
 | bit  8   R/W  F16_0_ENABLE                                        |
 | bit  4:0 R/W  F16_0_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_BIT_F16_0_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_BIT_F16_0_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_BIT_F16_0_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_BIT_F16_0_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_BIT_F16_0_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_0_REG_2_BIT_F16_0_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002008 + (N) * 128) GEN_FILTER_CFG_FILTER16_1_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_1_MASK                                        |
 | bit  15:0  R/W  F16_1_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_BIT_F16_1_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_BIT_F16_1_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_BIT_F16_1_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_1_BIT_F16_1_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x0000200c + (N) * 128) GEN_FILTER_CFG_FILTER16_1_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_1_INV                                           |
 | bit  8   R/W  F16_1_ENABLE                                        |
 | bit  4:0 R/W  F16_1_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_BIT_F16_1_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_BIT_F16_1_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_BIT_F16_1_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_BIT_F16_1_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_BIT_F16_1_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_1_REG_2_BIT_F16_1_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002010 + (N) * 128) GEN_FILTER_CFG_FILTER16_2_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_2_MASK                                        |
 | bit  15:0  R/W  F16_2_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_BIT_F16_2_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_BIT_F16_2_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_BIT_F16_2_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_1_BIT_F16_2_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x00002014 + (N) * 128) GEN_FILTER_CFG_FILTER16_2_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_2_INV                                           |
 | bit  8   R/W  F16_2_ENABLE                                        |
 | bit  4:0 R/W  F16_2_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_BIT_F16_2_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_BIT_F16_2_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_BIT_F16_2_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_BIT_F16_2_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_BIT_F16_2_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_2_REG_2_BIT_F16_2_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002018 + (N) * 128) GEN_FILTER_CFG_FILTER16_3_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_3_MASK                                        |
 | bit  15:0  R/W  F16_3_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_BIT_F16_3_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_BIT_F16_3_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_BIT_F16_3_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_1_BIT_F16_3_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x0000201c + (N) * 128) GEN_FILTER_CFG_FILTER16_3_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_3_INV                                           |
 | bit  8   R/W  F16_3_ENABLE                                        |
 | bit  4:0 R/W  F16_3_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_BIT_F16_3_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_BIT_F16_3_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_BIT_F16_3_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_BIT_F16_3_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_BIT_F16_3_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_3_REG_2_BIT_F16_3_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002020 + (N) * 128) GEN_FILTER_CFG_FILTER16_4_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_4_MASK                                        |
 | bit  15:0  R/W  F16_4_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_BIT_F16_4_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_BIT_F16_4_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_BIT_F16_4_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_1_BIT_F16_4_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x00002024 + (N) * 128) GEN_FILTER_CFG_FILTER16_4_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_4_INV                                           |
 | bit  8   R/W  F16_4_ENABLE                                        |
 | bit  4:0 R/W  F16_4_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_BIT_F16_4_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_BIT_F16_4_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_BIT_F16_4_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_BIT_F16_4_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_BIT_F16_4_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_4_REG_2_BIT_F16_4_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002028 + (N) * 128) GEN_FILTER_CFG_FILTER16_5_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_5_MASK                                        |
 | bit  15:0  R/W  F16_5_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_BIT_F16_5_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_BIT_F16_5_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_BIT_F16_5_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_1_BIT_F16_5_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x0000202c + (N) * 128) GEN_FILTER_CFG_FILTER16_5_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_5_INV                                           |
 | bit  8   R/W  F16_5_ENABLE                                        |
 | bit  4:0 R/W  F16_5_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_BIT_F16_5_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_BIT_F16_5_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_BIT_F16_5_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_BIT_F16_5_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_BIT_F16_5_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_5_REG_2_BIT_F16_5_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002030 + (N) * 128) GEN_FILTER_CFG_FILTER16_6_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_6_MASK                                        |
 | bit  15:0  R/W  F16_6_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_BIT_F16_6_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_BIT_F16_6_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_BIT_F16_6_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_1_BIT_F16_6_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x00002034 + (N) * 128) GEN_FILTER_CFG_FILTER16_6_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_6_INV                                           |
 | bit  8   R/W  F16_6_ENABLE                                        |
 | bit  4:0 R/W  F16_6_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_BIT_F16_6_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_BIT_F16_6_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_BIT_F16_6_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_BIT_F16_6_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_BIT_F16_6_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_6_REG_2_BIT_F16_6_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_INDEX_N_MIN     0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_INDEX_N_MAX     23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_INDEX_N_SIZE    24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_INDEX_N_OFFSET  128

/*-------------------------------------------------------------------.
 | Register (0x00002038 + (N) * 128) GEN_FILTER_CFG_FILTER16_7_REG_1 |
 +-------------------------------------------------------------------+
 | bit  31:16 R/W  F16_7_MASK                                        |
 | bit  15:0  R/W  F16_7_VALUE                                       |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_UNUSED_MASK     0x00000000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_BIT_F16_7_MASK_MSK  0xffff0000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_BIT_F16_7_MASK_OFF  16
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_BIT_F16_7_VALUE_MSK 0x0000ffff
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_1_BIT_F16_7_VALUE_OFF 0

/* index definitions for PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2 */
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_INDEX_N_MIN      0
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_INDEX_N_MAX      23
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_INDEX_N_SIZE     24
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_INDEX_N_OFFSET   128

/*-------------------------------------------------------------------.
 | Register (0x0000203c + (N) * 128) GEN_FILTER_CFG_FILTER16_7_REG_2 |
 +-------------------------------------------------------------------+
 | bit  12  R/W  F16_7_INV                                           |
 | bit  8   R/W  F16_7_ENABLE                                        |
 | bit  4:0 R/W  F16_7_ADDR                                          |
 +------------------------------------------------------------------*/
#define PMC_ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_UNUSED_MASK      0xffffeee0
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_BIT_F16_7_INV_MSK    0x00001000
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_BIT_F16_7_INV_OFF    12
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_BIT_F16_7_ENABLE_MSK 0x00000100
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_BIT_F16_7_ENABLE_OFF 8
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_BIT_F16_7_ADDR_MSK   0x0000001f
#define ECLASS120_REG_GEN_FILTER_CFG_FILTER16_7_REG_2_BIT_F16_7_ADDR_OFF   0

/* index definitions for PMC_ECLASS120_REG_FOUT_COMB_ORIN_1 */
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_1_INDEX_N_MIN                      0
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_1_INDEX_N_MAX                      15
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_1_INDEX_N_SIZE                     16
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_1_INDEX_N_OFFSET                   4

/*--------------------------------------------------.
 | Register (0x00002c00 + (N) * 4) FOUT_COMB_ORIN_1 |
 +--------------------------------------------------+
 | bit  31:0 R/W  COMB_ORIN_HWIRED_FOUT_EN_LSB      |
 +-------------------------------------------------*/
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_1_UNUSED_MASK                      0x00000000
#define ECLASS120_REG_FOUT_COMB_ORIN_1_BIT_COMB_ORIN_HWIRED_FOUT_EN_LSB_MSK 0xffffffff
#define ECLASS120_REG_FOUT_COMB_ORIN_1_BIT_COMB_ORIN_HWIRED_FOUT_EN_LSB_OFF 0

/* index definitions for PMC_ECLASS120_REG_FOUT_COMB_ORIN_2 */
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_2_INDEX_N_MIN                      0
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_2_INDEX_N_MAX                      15
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_2_INDEX_N_SIZE                     16
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_2_INDEX_N_OFFSET                   4

/*--------------------------------------------------.
 | Register (0x00002c50 + (N) * 4) FOUT_COMB_ORIN_2 |
 +--------------------------------------------------+
 | bit  31    R/W  COMB_ORIN_OUT_INV                |
 | bit  30:23 R/W  COMB_ORIN_CFG_FOUT_EN            |
 | bit  17:0  R/W  COMB_ORIN_HWIRED_FOUT_EN_MSB     |
 +-------------------------------------------------*/
#define PMC_ECLASS120_REG_FOUT_COMB_ORIN_2_UNUSED_MASK                      0x007c0000
#define ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_OUT_INV_MSK            0x80000000
#define ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_OUT_INV_OFF            31
#define ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_CFG_FOUT_EN_MSK        0x7f800000
#define ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_CFG_FOUT_EN_OFF        23
#define ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK 0x0003ffff
#define ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_OFF 0

/* index definitions for PMC_ECLASS120_REG_FOUT_COMB_AND_1 */
#define PMC_ECLASS120_REG_FOUT_COMB_AND_1_INDEX_N_MIN                     0
#define PMC_ECLASS120_REG_FOUT_COMB_AND_1_INDEX_N_MAX                     39
#define PMC_ECLASS120_REG_FOUT_COMB_AND_1_INDEX_N_SIZE                    40
#define PMC_ECLASS120_REG_FOUT_COMB_AND_1_INDEX_N_OFFSET                  4

/*-------------------------------------------------.
 | Register (0x00002ca0 + (N) * 4) FOUT_COMB_AND_1 |
 +-------------------------------------------------+
 | bit  31:0 R/W  COMB_AND_HWIRED_FOUT_EN_LSB      |
 +------------------------------------------------*/
#define PMC_ECLASS120_REG_FOUT_COMB_AND_1_UNUSED_MASK                     0x00000000
#define ECLASS120_REG_FOUT_COMB_AND_1_BIT_COMB_AND_HWIRED_FOUT_EN_LSB_MSK 0xffffffff
#define ECLASS120_REG_FOUT_COMB_AND_1_BIT_COMB_AND_HWIRED_FOUT_EN_LSB_OFF 0

/* index definitions for PMC_ECLASS120_REG_FOUT_COMB_AND_2 */
#define PMC_ECLASS120_REG_FOUT_COMB_AND_2_INDEX_N_MIN                     0
#define PMC_ECLASS120_REG_FOUT_COMB_AND_2_INDEX_N_MAX                     39
#define PMC_ECLASS120_REG_FOUT_COMB_AND_2_INDEX_N_SIZE                    40
#define PMC_ECLASS120_REG_FOUT_COMB_AND_2_INDEX_N_OFFSET                  4

/*-------------------------------------------------.
 | Register (0x00002d50 + (N) * 4) FOUT_COMB_AND_2 |
 +-------------------------------------------------+
 | bit  31:24 R/W  COMB_AND_CFG_FOUT_EN            |
 | bit  17:0  R/W  COMB_AND_HWIRED_FOUT_EN_MSB     |
 +------------------------------------------------*/
#define PMC_ECLASS120_REG_FOUT_COMB_AND_2_UNUSED_MASK                     0x00fc0000
#define ECLASS120_REG_FOUT_COMB_AND_2_BIT_COMB_AND_CFG_FOUT_EN_MSK        0xff000000
#define ECLASS120_REG_FOUT_COMB_AND_2_BIT_COMB_AND_CFG_FOUT_EN_OFF        24
#define ECLASS120_REG_FOUT_COMB_AND_2_BIT_COMB_AND_HWIRED_FOUT_EN_MSB_MSK 0x0003ffff
#define ECLASS120_REG_FOUT_COMB_AND_2_BIT_COMB_AND_HWIRED_FOUT_EN_MSB_OFF 0

/* index definitions for PMC_ECLASS120_REG_FOUT_COMB_AND_3 */
#define PMC_ECLASS120_REG_FOUT_COMB_AND_3_INDEX_N_MIN              0
#define PMC_ECLASS120_REG_FOUT_COMB_AND_3_INDEX_N_MAX              39
#define PMC_ECLASS120_REG_FOUT_COMB_AND_3_INDEX_N_SIZE             40
#define PMC_ECLASS120_REG_FOUT_COMB_AND_3_INDEX_N_OFFSET           4

/*-------------------------------------------------.
 | Register (0x00002e00 + (N) * 4) FOUT_COMB_AND_3 |
 +-------------------------------------------------+
 | bit  31   R/W  COMB_AND_OUT_INV                 |
 | bit  15:0 R/W  COMB_AND_FILTORIN_EN             |
 +------------------------------------------------*/
#define PMC_ECLASS120_REG_FOUT_COMB_AND_3_UNUSED_MASK              0x7fff0000
#define ECLASS120_REG_FOUT_COMB_AND_3_BIT_COMB_AND_OUT_INV_MSK     0x80000000
#define ECLASS120_REG_FOUT_COMB_AND_3_BIT_COMB_AND_OUT_INV_OFF     31
#define ECLASS120_REG_FOUT_COMB_AND_3_BIT_COMB_AND_FILTORIN_EN_MSK 0x0000ffff
#define ECLASS120_REG_FOUT_COMB_AND_3_BIT_COMB_AND_FILTORIN_EN_OFF 0

/* index definitions for PMC_ECLASS120_REG_FOUT_COMB_OROUT_1 */
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_1_INDEX_N_MIN                   0
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_1_INDEX_N_MAX                   25
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_1_INDEX_N_SIZE                  26
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_1_INDEX_N_OFFSET                4

/*---------------------------------------------------.
 | Register (0x00002eb0 + (N) * 4) FOUT_COMB_OROUT_1 |
 +---------------------------------------------------+
 | bit  31:0 R/W  COMB_OROUT_FILTAND_EN_LSB          |
 +--------------------------------------------------*/
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_1_UNUSED_MASK                   0x00000000
#define ECLASS120_REG_FOUT_COMB_OROUT_1_BIT_COMB_OROUT_FILTAND_EN_LSB_MSK 0xffffffff
#define ECLASS120_REG_FOUT_COMB_OROUT_1_BIT_COMB_OROUT_FILTAND_EN_LSB_OFF 0

/* index definitions for PMC_ECLASS120_REG_FOUT_COMB_OROUT_2 */
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_2_INDEX_N_MIN                   0
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_2_INDEX_N_MAX                   25
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_2_INDEX_N_SIZE                  26
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_2_INDEX_N_OFFSET                4

/*---------------------------------------------------.
 | Register (0x00002f30 + (N) * 4) FOUT_COMB_OROUT_2 |
 +---------------------------------------------------+
 | bit  31    R/W  COMB_OROUT_OUT_EN                 |
 | bit  30    R/W  COMB_OROUT_OUT_INV                |
 | bit  23:16 R/W  COMB_OROUT_FILTAND_EN_MSB         |
 | bit  7:0   R/W  COMB_OROUT_CFG_FOUT_EN            |
 +--------------------------------------------------*/
#define PMC_ECLASS120_REG_FOUT_COMB_OROUT_2_UNUSED_MASK                   0x3f00ff00
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_OUT_EN_MSK         0x80000000
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_OUT_EN_OFF         31
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_OUT_INV_MSK        0x40000000
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_OUT_INV_OFF        30
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_FILTAND_EN_MSB_MSK 0x00ff0000
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_FILTAND_EN_MSB_OFF 16
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_CFG_FOUT_EN_MSK    0x000000ff
#define ECLASS120_REG_FOUT_COMB_OROUT_2_BIT_COMB_OROUT_CFG_FOUT_EN_OFF    0

/*--------------------------------------.
 | Register 0x00003900 NO_MATCH_EN      |
 +--------------------------------------+
 | bit  31:26 R/W  NO_MATCH_EN_VLANMPLS |
 | bit  25:0  R/W  NO_MATCH_EN_GEN      |
 +-------------------------------------*/
#define PMC_ECLASS120_REG_NO_MATCH_EN_UNUSED_MASK              0x00000000
#define ECLASS120_REG_NO_MATCH_EN_BIT_NO_MATCH_EN_VLANMPLS_MSK 0xfc000000
#define ECLASS120_REG_NO_MATCH_EN_BIT_NO_MATCH_EN_VLANMPLS_OFF 26
#define ECLASS120_REG_NO_MATCH_EN_BIT_NO_MATCH_EN_GEN_MSK      0x03ffffff
#define ECLASS120_REG_NO_MATCH_EN_BIT_NO_MATCH_EN_GEN_OFF      0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ECLASS_REGS_H */
