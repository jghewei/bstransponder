/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*  DESCRIPTION:   This file contains Module ID definitions that are
*                 unique across the products. This allows for reuse of code
*                 bases, identifying log/error/id codes to a module.
*  
*                 Typical use model is to reserve a new ID in the list using
*                 a global mnuemonic, then use the PMC_CREATE_UNIQUE_ID() macro
*                 to create the log codes/error codes etc. uniquely within
*                 the module along with the module id defined here.
*  
*  NOTES:
*  
*******************************************************************************/

#ifndef _PMC_MID_H
#define _PMC_MID_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/

/* Encode the module id into the upper 16 bits for logging, tagging, error codes*/
#define PMC_CREATE_UNIQUE_ID(mid, id)               ((mid << 16) + id) 

/* Base ID */
#define PMC_MID_BASE                                (0x1000)

/* Global MID */
#define PMC_MID_GLOBAL                              (PMC_MID_BASE + 0x0F) /* Global error/logging/values */

/* INVALID MID */
#define PMC_MID_INVALID                             (PMC_MID_BASE + 0xFFF) /* Invalid MID */

/* HyPHY IDs - not TSB block based */
#define PMC_MID_HYPHY                               (PMC_MID_BASE + 0x00)
#define PMC_MID_HYPHY_CPB                           (PMC_MID_BASE + 0x01)
#define PMC_MID_HYPHY_LSCI                          (PMC_MID_BASE + 0x02)
#define PMC_MID_HYPHY_HSCI                          (PMC_MID_BASE + 0x03)
#define PMC_MID_HYPHY_PIF                           (PMC_MID_BASE + 0x04)
#define PMC_MID_HYPHY_GFP                           (PMC_MID_BASE + 0x05)
#define PMC_MID_HYPHY_CBRI                          (PMC_MID_BASE + 0x06)
#define PMC_MID_HYPHY_TSE                           (PMC_MID_BASE + 0x07)
#define PMC_MID_HYPHY_VCAT                          (PMC_MID_BASE + 0x08)
#define PMC_MID_HYPHY_SERDES                        (PMC_MID_BASE + 0x09)
#define PMC_MID_HYPHY_HOSTMSG                       (PMC_MID_BASE + 0x0A)
#define PMC_MID_HYPHY_OTN_OH_RAM                    (PMC_MID_BASE + 0x0B)
#define PMC_MID_HYPHY_ODUKPP_OH_RAM                 (PMC_MID_BASE + 0x0C)
#define PMC_MID_HYPHY_RTOS                          (PMC_MID_BASE + 0x0D)
#define PMC_MID_HYPHY_SCHD                          (PMC_MID_BASE + 0x0E)
/* HyPHY IDs - End */

/* POLO IDs - not released to customers yet,... feel free to modify */
#define PMC_MID_POLO                               (PMC_MID_BASE + 0x100)
#define PMC_MID_POLO_GFEC                          (PMC_MID_BASE + 0x101)
#define PMC_MID_POLO_DSP                           (PMC_MID_BASE + 0x102)
#define PMC_MID_POLO_DSP_QEF                       (PMC_MID_BASE + 0x103)
#define PMC_MID_POLO_DSP_MFD                       (PMC_MID_BASE + 0x104)
#define PMC_MID_POLO_DSP_PMD_FIR                   (PMC_MID_BASE + 0x105)
#define PMC_MID_POLO_DSP_PMD_ERR                   (PMC_MID_BASE + 0x106)
#define PMC_MID_POLO_DSP_STAT                      (PMC_MID_BASE + 0x107)
#define PMC_MID_POLO_DSP_CFAT                      (PMC_MID_BASE + 0x108)
#define PMC_MID_POLO_DSP_DDI                       (PMC_MID_BASE + 0x109)
#define PMC_MID_POLO_DSP_FFS                       (PMC_MID_BASE + 0x10a)
#define PMC_MID_POLO_DSP_SIB                       (PMC_MID_BASE + 0x10b)
#define PMC_MID_POLO_DSP_CDC_FFT                   (PMC_MID_BASE + 0x10c)
#define PMC_MID_POLO_DSP_CPR                       (PMC_MID_BASE + 0x10d)
#define PMC_MID_POLO_SWIZZLE                       (PMC_MID_BASE + 0x10e)
#define PMC_MID_POLO_SFI                           (PMC_MID_BASE + 0x10f)
#define PMC_MID_POLO_SFI_CSU                       (PMC_MID_BASE + 0x110)
#define PMC_MID_POLO_OTU3AU                        (PMC_MID_BASE + 0x111)
#define PMC_MID_POLO_OTU3A                         (PMC_MID_BASE + 0x112)
/* POLO IDs - End */

/* DIGI IDs - not released to customers yet,... feel free to modify */
#define PMC_MID_DIGI                               (PMC_MID_BASE + 0x200)

/* COMMON */
#define PMC_MID_DIGI_CPACT_OBUF                    (PMC_MID_BASE + 0x201)
#define PMC_MID_DIGI_EXPAND_IBUF                   (PMC_MID_BASE + 0x202)
#define PMC_MID_DIGI_MPMA                          (PMC_MID_BASE + 0x203)
#define PMC_MID_DIGI_MP_MGEN                       (PMC_MID_BASE + 0x204)
#define PMC_MID_DIGI_MPMO                          (PMC_MID_BASE + 0x205)
#define PMC_MID_DIGI_OHFS_REMOVE                   (PMC_MID_BASE + 0x206)
#define PMC_MID_DIGI_OHFS_INSERT                   (PMC_MID_BASE + 0x207)
#define PMC_MID_DIGI_RXJC_PROC                     (PMC_MID_BASE + 0x208)
#define PMC_MID_DIGI_SCBS3                         (PMC_MID_BASE + 0x209)
#define PMC_MID_DIGI_TXJC_PROC                     (PMC_MID_BASE + 0x20A)
#define PMC_MID_DIGI_GSUP43_C73                    (PMC_MID_BASE + 0x20B)
#define PMC_MID_DIGI_ODUKSC                        (PMC_MID_BASE + 0x20C)
#define PMC_MID_DIGI_CFC                           (PMC_MID_BASE + 0x20D)
#define PMC_MID_DIGI_UTIL_SCHD                     (PMC_MID_BASE + 0x20E)
#define PMC_MID_DIGI_UTIL_OPSA                     (PMC_MID_BASE + 0x20F)
#define PMC_MID_DIGI_UTIL_GLOBAL                   (PMC_MID_BASE + 0x210)
#define PMC_MID_DIGI_ENET_FEGE                     (PMC_MID_BASE + 0x211)
#define PMC_MID_DIGI_PMON                          (PMC_MID_BASE + 0x212)
#define PMC_MID_DIGI_HOSTMSG                       (PMC_MID_BASE + 0x213)
#define PMC_MID_DIGI_S16_SERDES                    (PMC_MID_BASE + 0x214)
#define PMC_MID_DIGI_ACB_FT                        (PMC_MID_BASE + 0x215)
#define PMC_MID_DIGI_D8_RX_2X_SERDES               (PMC_MID_BASE + 0x216)
#define PMC_MID_DIGI_D8_TX_2X_SERDES               (PMC_MID_BASE + 0x217)
#define PMC_MID_DIGI_SFI51_18X_RX_SERDES_GLUE      (PMC_MID_BASE + 0x218)
#define PMC_MID_DIGI_SFI51_18X_TX_SERDES_GLUE      (PMC_MID_BASE + 0x219)
#define PMC_MID_DIGI_T8_SERDES                     (PMC_MID_BASE + 0x21A)
#define PMC_MID_DIGI_C8_SERDES                     (PMC_MID_BASE + 0x21B) 
#define PMC_MID_DIGI_ODU_RCP                       (PMC_MID_BASE + 0x21C) 
#define PMC_MID_DIGI_ODU_OHP                       (PMC_MID_BASE + 0x21D) 
#define PMC_MID_DIGI_GHAO                          (PMC_MID_BASE + 0x21E)

/* MAPOTN */
#define PMC_MID_DIGI_MAPOTN                        (PMC_MID_BASE + 0x220)
#define PMC_MID_DIGI_RGFP                          (PMC_MID_BASE + 0x221)
#define PMC_MID_DIGI_TGFP                          (PMC_MID_BASE + 0x222)
#define PMC_MID_DIGI_RGMP                          (PMC_MID_BASE + 0x223)
#define PMC_MID_DIGI_TGMP                          (PMC_MID_BASE + 0x224)

/* COREOTN */
#define PMC_MID_DIGI_COREOTN                       (PMC_MID_BASE + 0x230)
#define PMC_MID_DIGI_ODTU_DMX                      (PMC_MID_BASE + 0x231)
#define PMC_MID_DIGI_ODTU_MUX                      (PMC_MID_BASE + 0x232)
#define PMC_MID_DIGI_ODU_RFRM                      (PMC_MID_BASE + 0x233)
#define PMC_MID_DIGI_ODU_TFRM                      (PMC_MID_BASE + 0x234)
#define PMC_MID_DIGI_STS_SW                        (PMC_MID_BASE + 0x235)
#define PMC_MID_DIGI_ODUJAT                        (PMC_MID_BASE + 0x236) 
#define PMC_MID_DIGI_OTN_PRBS                      (PMC_MID_BASE + 0x237)

/* ENET */
#define PMC_MID_DIGI_ENET                          (PMC_MID_BASE + 0x240)
#define PMC_MID_DIGI_EMAC                          (PMC_MID_BASE + 0x241)
#define PMC_MID_DIGI_ETIME                         (PMC_MID_BASE + 0x242)
#define PMC_MID_DIGI_EPMM                          (PMC_MID_BASE + 0x243)
#define PMC_MID_DIGI_EMPEI_I                       (PMC_MID_BASE + 0x244)
#define PMC_MID_DIGI_EMPEI_E                       (PMC_MID_BASE + 0x245)
#define PMC_MID_DIGI_ECLASS                        (PMC_MID_BASE + 0x246)
#define PMC_MID_DIGI_ETRANS                        (PMC_MID_BASE + 0x247)
#define PMC_MID_DIGI_MGMT_FEGE                     (PMC_MID_BASE + 0x248)
#define PMC_MID_DIGI_EMPEI_WRAP                    (PMC_MID_BASE + 0x249)
#define PMC_MID_DIGI_EXCO3M                        (PMC_MID_BASE + 0x24A)
#define PMC_MID_DIGI_EXCO4M                        (PMC_MID_BASE + 0x24B)

/* LINEOTN */
#define PMC_MID_DIGI_LINEOTN                       (PMC_MID_BASE + 0x260)
#define PMC_MID_DIGI_FEC_PMON                      (PMC_MID_BASE + 0x261)
#define PMC_MID_DIGI_OTU_FRM                       (PMC_MID_BASE + 0x262)    

/* CBRC */
#define PMC_MID_DIGI_CBRC                          (PMC_MID_BASE + 0x280)
#define PMC_MID_DIGI_SDH_PMG                       (PMC_MID_BASE + 0x281)
#define PMC_MID_DIGI_CBR_FSGM                      (PMC_MID_BASE + 0x282)
#define PMC_MID_DIGI_FC800_PMG                     (PMC_MID_BASE + 0x283)
#define PMC_MID_DIGI_FC1200_PMON                   (PMC_MID_BASE + 0x284)

/* CPB */
#define PMC_MID_DIGI_CPB                           (PMC_MID_BASE + 0x290)

/* SIFD */
#define PMC_MID_DIGI_SIFD                          (PMC_MID_BASE + 0x2A0)

/* CBR_DINT,CBR_INT */
#define PMC_MID_DIGI_CBR_DINT                      (PMC_MID_BASE + 0x2B0)    
#define PMC_MID_DIGI_CBR_INT                       (PMC_MID_BASE + 0x2B1)  

/* LIFD */
#define PMC_MID_DIGI_LIFD                          (PMC_MID_BASE + 0x2C0)


/* UTIL_GEN_DB */ 
#define PMC_MID_DIGI_UTIL_GEN_DB                   (PMC_MID_BASE + 0x2D0)

/* DIGI IDs - End */



#ifdef __cplusplus
}
#endif

#endif /* _PMC_MID_H */
/* end of file */
