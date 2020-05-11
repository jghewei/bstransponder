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
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/


#include "eclass_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* Combiner ORIN stage, specific per FILTer (16) */

/* Combiner ORIN Stage HWired LSB Cfg.for DIGI use case plus Clause 8.3*/
#define comb_orin_hwired_fout_en_lsb_VALUES                \
    0x00C00000,    /* ORIN0: IPv4 UDP port */                              \
    0x80000000,    /* ORIN1: IPv6 UDP port */                \
    0x00000000,    /* ORIN2: PTP Event */                    \
    0x00000000,    /* ORIN3: PTP General */                \
    0x00000000,    /* ORIN4: PTP Sync or Deleay_Req */            \
    0x00000000,    /* ORIN5: PTP P_Delay_Req or P_Delay_Resp or P_Delay_Follow_Up */ \
    0x00000000,    /* ORIN6: All PTP but P_Delay_Req, P_Delay_Resp, and P_Delay_Follow_Up    */ \
    0x00000001,    /* ORIN7: not(Slow Protocol Typical Destinations)  */ \
    0x00000001,    /* ORIN8: not(Slow Protocol Known Destinations)  */ \
    0x00000000,    /* ORIN9: not(Slow Protocol Known Subtypes)  */    \
    0xFFFFFFFF,    /* ORIN10: Unused */                    \
    0xFFFFFFFF,    /* ORIN11: Unused */                    \
    0xFFFFFFFF,    /* ORIN12: Unused */                    \
    0xFFFFFFFF,    /* ORIN13: Unused */                    \
    0xFFFFFFFF,    /* ORIN14: Unused */                    \
    0xFFFFFFFF     /* ORIN15: Unused */

/* Combiner ORIN Stage HWired MSB Cfg.for DIGI use case plus Clause 8.3*/
#define  eclass_COMB_ORIN_HWIRED_FOUT_EN_MSB_VALUES            \
    0x00000,    /* ORIN0: IPv4 UDP port */                              \
    0x00001,    /* ORIN1: IPv6 UDP port */                \
    0x00F00,    /* ORIN2: PTP Event */                    \
    0x3F000,    /* ORIN3: PTP General */                \
    0x00300,    /* ORIN4: PTP Sync or Deleay_Req */            \
    0x04C00,    /* ORIN5: PTP P_Delay_Req or P_Delay_Resp or P_Delay_Follow_Up */ \
    0x3B300,    /* ORIN6: All PTP but P_Delay_Req, P_Delay_Resp, and P_Delay_Follow_Up    */ \
    0x00000,    /* ORIN7: not(Slow Protocol Typical Destinations) */ \
    0x00000,    /* ORIN8: not(Slow Protocol Known Destinations) */ \
    0x00078,    /* ORIN9: not(Slow Protocol Known Subtypes) */    \
    ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK, /* ORIN10: Unused */ \
    ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK, /* ORIN11: Unused */ \
    ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK, /* ORIN12: Unused */ \
    ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK, /* ORIN13: Unused */ \
    ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK, /* ORIN14: Unused */ \
    ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK /* ORIN15: Unused */


/* Combiner AND stage, specific per FILTer (40) */

/* Combiner AND Stage HWired LSB Cfg.for DIGI use case plus Clause 8.3*/
#define comb_and_hwired_fout_en_lsb_VALUES    \
  0x00000003,    /* AND0: Link OAM */                    \
    0x00000003,    /* AND1: SyncE ESMC */                    \
    0x00000003,    /* AND2: LACP */                    \
    0x00000003,    /* AND3: Marker */                    \
    0x00000004,    /* AND4: Management VLAN */                \
    0x00000100,    /* AND5: PTP over Ethernet */                \
    0x00018200,    /* AND6: PTP over UDP over IPv4 over Eth. */        \
    0x03000400,    /* AND7: PTP over UDP over IPv6 over Eth. */        \
    0x00000100,    /* AND8: PTP Event over Eth. */                \
    0x00418200,    /* AND9: PTP Event over UDP over IPv4 over Eth. */    \
    0x83000400,    /* AND10: PTP Event over UDP over IPv6 over Eth. */    \
    0x00000100,    /* AND11: PTP General over Eth. */            \
    0x00818200,    /* AND12: PTP General over UDP over IPv4 over Eth. */    \
    0x03000400,    /* AND13: PTP General over UDP over IPv6 over Eth. */    \
    0x00000100,    /* AND14: PTP (Sync or Delay_Req) over Eth. */        \
    0x00418200,    /* AND15: PTP (Sync or Delay_Req) over UDP over IPv4 over Eth. */ \
    0x83000400,    /* AND16: PTP (Sync or Delay_Req) over UDP over IPv6 over Eth. */ \
    0x00000108,    /* AND17: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00000110,    /* AND18: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */    \
    0x00038200,    /* AND19: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */    \
    0x00058200,    /* AND20: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x07000400,    /* AND21: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */    \
    0x0B000400,    /* AND22: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x00000120,    /* AND23: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00000140,    /* AND24: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00098220,    /* AND25: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x00108240,    /* AND26: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x13000420,    /* AND27: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x23000440,    /* AND28: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x00000180,    /* AND29: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00218200,    /* AND30: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */    \
    0x43000400,    /* AND31: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */    \
    0x00001000,    /* AND32: IPv4 ARP */                    \
    0x00002000,    /* AND33: IPv6 NDP */                    \
    0x00004000,    /* AND34: PRBS */                    \
    0x00000002,    /* AND35: Slow Protocol with Reserved Destination that is not the Typical Slow Protocol Destination */ \
    0x00000002,    /* AND36: Slow Protocol with not(known Destinations) */    \
    0x00000003,    /* AND37: Slow Protocol with typical destination and not(known subtype) */ \
    0x00000000,    /* AND38: Unused */                    \
    0x00000000    /* AND39: Unused */

/* Combiner AND Stage HWired MSB Cfg.for DIGI use case (Clause 8.3 does not impact this field)*/
#define eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_VALUES    \
  0x00008,    /* AND0: Link OAM */            \
    0x00010,    /* AND1: SyncE ESMC */            \
    0x00020,    /* AND2: LACP */            \
    0x00040,    /* AND3: Marker */            \
    0x00080,    /* AND4: Management VLAN */        \
    0x00000,    /* AND5: PTP over Ethernet */            \
    0x00000,    /* AND6: PTP over UDP over IPv4 over Eth. */    \
    0x00000,    /* AND7: PTP over UDP over IPv6 over Eth. */    \
    0x00000,    /* AND8: PTP Event over Eth. */            \
    0x00000,    /* AND9: PTP Event over UDP over IPv4 over Eth. */ \
    0x00000,    /* AND10: PTP Event over UDP over IPv6 over Eth. */ \
    0x00000,    /* AND11: PTP General over Eth. */        \
    0x00000,    /* AND12: PTP General over UDP over IPv4 over Eth. */ \
    0x00001,    /* AND13: PTP General over UDP over IPv6 over Eth. */ \
    0x00000,    /* AND14: PTP (Sync or Delay_Req) over Eth. */    \
    0x00000,    /* AND15: PTP (Sync or Delay_Req) over UDP over IPv4 over Eth. */ \
    0x00000,    /* AND16: PTP (Sync or Delay_Req) over UDP over IPv6 over Eth. */ \
    0x00000,    /* AND17: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00000,    /* AND18: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */    \
    0x00000,    /* AND19: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */    \
    0x00000,    /* AND20: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x00000,    /* AND21: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */    \
    0x00000,    /* AND22: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x00000,    /* AND23: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00000,    /* AND24: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00000,    /* AND25: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x00000,    /* AND26: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x00000,    /* AND27: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x00000,    /* AND28: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x00000,    /* AND29: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x00000,    /* AND30: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */    \
    0x00000,    /* AND31: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */    \
    0x00002,    /* AND32: IPv4 ARP */                \
    0x00004,    /* AND33: IPv6 NDP */                \
    0x00000,    /* AND34: PRBS */                \
    0x00000,    /* AND35: Slow Protocol with Reserved Destination that is not the Typical Slow Protocol Destination */ \
    0x00000,    /* AND36: Slow Protocol with not(known Destinations) */ \
    0x00000,    /* AND37: Slow Protocol with typical destination and not(known subtype) */ \
    0x00000,    /* AND38: Unused */                \
    0x00000    /* AND39: Unused */    

/* Combiner AND Stage FiltORIN LSB Cfg.for DIGI use case*/                
#define eclass_reg_FOUT_COMB_AND_1_array_VALUES \
  0x00000003,    /* AND0: Link OAM */                    \
    0x00000003,    /* AND1: SyncE ESMC */                    \
    0x00000003,    /* AND2: LACP */                    \
    0x00000003,    /* AND3: Marker */                    \
    0x00000004,    /* AND4: Management VLAN */                \
    0x00000100,    /* AND5: PTP over Ethernet */                \
    0x00018200,    /* AND6: PTP over UDP over IPv4 over Eth. */        \
    0x03000400,    /* AND7: PTP over UDP over IPv6 over Eth. */        \
    0x00000100,    /* AND8: PTP Event over Eth. */                \
    0x00418200,    /* AND9: PTP Event over UDP over IPv4 over Eth. */    \
    0x83000400,    /* AND10: PTP Event over UDP over IPv6 over Eth. */    \
    0x00000100,    /* AND11: PTP General over Eth. */            \
    0x00818200,    /* AND12: PTP General over UDP over IPv4 over Eth. */    \
    0x03000400,    /* AND13: PTP General over UDP over IPv6 over Eth. */    \
    0x00000100,    /* AND14: PTP (Sync or Delay_Req) over Eth. */        \
    0x00418200,    /* AND15: PTP (Sync or Delay_Req) over UDP over IPv4 over Eth. */    \
    0x83000400,    /* AND16: PTP (Sync or Delay_Req) over UDP over IPv6 over Eth. */    \
    0x00000108,    /* AND17: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */    \
    0x00000110,    /* AND18: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */        \
    0x00038200,    /* AND19: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */        \
    0x00058200,    /* AND20: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */        \
    0x07000400,    /* AND21: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */        \
    0x0B000400,    /* AND22: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */        \
    0x00000120,    /* AND23: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */            \
    0x00000140,    /* AND24: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */                \
    0x00098220,    /* AND25: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */        \
    0x00108240,    /* AND26: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */        \
    0x13000420,    /* AND27: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */        \
    0x23000440,    /* AND28: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */        \
    0x00000180,    /* AND29: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */            \
    0x00218200,    /* AND30: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */        \
    0x43000400,    /* AND31: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */        \
    0x00001000,    /* AND32: IPv4 ARP */                    \
    0x00002000,    /* AND33: IPv6 NDP */                    \
    0x00004000,    /* AND34: PRBS */                    \
    0x00000000, /* AND35: Unused */                    \
    0x00000000, /* AND36: Unused */                    \
    0x00000000,    /* AND37: Unused */                    \
    0x00000000,    /* AND38: Unused */                    \
    0x00000000  /* AND39: Unused */

/* Combiner AND Stage FiltORIN LSB Cfg.for DIGI use case (Clause 8.3 does not impact this field)  */
#define     eclass_field_COMB_AND_FILTORIN_EN_VALUES    \
  0x0000,    /* AND0: Link OAM */              \
    0x0000,    /* AND1: SyncE ESMC */        \
    0x0000,    /* AND2: LACP */        \
    0x0000,    /* AND3: Marker */        \
    0x0000,    /* AND4: Management VLAN */    \
    0x0000,    /* AND5: PTP over Ethernet */    \
    0x0001,    /* AND6: PTP over UDP over IPv4 over Eth. */    \
    0x0002,    /* AND7: PTP over UDP over IPv6 over Eth. */    \
    0x0004,    /* AND8: PTP Event over Eth. */            \
    0x0004,    /* AND9: PTP Event over UDP over IPv4 over Eth. */ \
    0x0004,    /* AND10: PTP Event over UDP over IPv6 over Eth. */ \
    0x0008,    /* AND11: PTP General over Eth. */        \
    0x0008,    /* AND12: PTP General over UDP over IPv4 over Eth. */ \
    0x0008,    /* AND13: PTP General over UDP over IPv6 over Eth. */ \
    0x0010,    /* AND14: PTP (Sync or Delay_Req) over Eth. */    \
    0x0010,    /* AND15: PTP (Sync or Delay_Req) over UDP over IPv4 over Eth. */ \
    0x0010,    /* AND16: PTP (Sync or Delay_Req) over UDP over IPv6 over Eth. */ \
    0x0040,    /* AND17: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x0020,    /* AND18: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x0041,    /* AND19: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */    \
    0x0021,    /* AND20: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x0042,    /* AND21: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */    \
    0x0022,    /* AND22: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x0040,    /* AND23: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x0020,    /* AND24: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x0041,    /* AND25: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x0021,    /* AND26: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */ \
    0x0042,    /* AND27: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x0022,    /* AND28: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */ \
    0x0020,    /* AND29: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */ \
    0x0021,    /* AND30: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */    \
    0x0022,    /* AND31: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */    \
    0x0000,    /* AND32: IPv4 ARP */                \
    0x0000,    /* AND33: IPv6 NDP */                \
    0x0000,    /* AND34: PRBS */                \
    0x0000,    /* AND35: Slow Protocol with Reserved Destination that is not the Typical Slow Protocol Destination */ \
    0x0000,    /* AND36: Slow Protocol with not(known Destinations) */ \
    0x0000,    /* AND37: Slow Protocol with typical destination and not(known subtype) */ \
    0x0000,    /* AND38: Unused */                \
    0x0000    /* AND39: Unused */


/* Combiner OROUT stage, specific per FILTer (26) */

/* Combiner OROUT FiltAND LSB Cfg.for DIGI use case*/
#define eclass_reg_FOUT_COMB_OROUT_1_array_VALUES    \
  0x00000001,        /* OROUT0-GenTag[0]: Link OAM */    \
    0x00000002,        /* OROUT1-GenTag[1]: SyncE ESMC */    \
    0x00000004,        /* OROUT2-GenTag[2]: LACP */        \
    0x00000008,        /* OROUT3-GenTag[3]: Marker */        \
    0x00000010,        /* OROUT4-GenTag[4]: Management VLAN */    \
    0x000000E0,        /* OROUT5-GenTag[5]: PTP */        \
    0x00000020,        /* OROUT6-GenTag[6]: PTP over Ethernet */    \
    0x00000040,        /* OROUT7-GenTag[7]: PTP over UDP ov IPv4 ov Eth. */ \
    0x00000080,        /* OROUT8-GenTag[8]: PTP over UDP ov IPv6 ov Eth. */ \
    0x00000700,        /* OROUT9-GenTag[9]: PTP Event */    \
    0x00003800,        /* OROUT10-GenTag[10]: PTP General */    \
    0x0001C000,        /* OROUT11-GenTag[11]: PTP Sync or Delay_Req */    \
    0x007E0000,        /* OROUT12-GenTag[12]: PTP Multicast DA */    \
    0x1F800000,        /* OROUT13-GenTag[13]: PTP Unicast DA */    \
    0xE0000000,        /* OROUT14-GenTag[14]: PTP not(Unicast DA) */    \
    0x00000000,        /* OROUT15-GenTag[15]: IPv4 ARP */    \
    0x00000000,        /* OROUT16-GenTag[16]: IPv6 NDP */    \
    0x00000000,        /* OROUT17-GenTag[17]: PRBS */        \
    0xFFFFFFFF,        /* OROUT25-GenTag[18]: Unused */    \
    0xFFFFFFFF,        /* OROUT25-GenTag[19]: Unused */    \
    0xFFFFFFFF,     /* OROUT25-GenTag[20]: Unused */    \
    0xFFFFFFFF,     /* OROUT25-GenTag[21]: Unused */    \
    0xFFFFFFFF,     /* OROUT25-GenTag[22]: Unused */    \
    0xFFFFFFFF,     /* OROUT25-GenTag[23]: Unused */    \
    0xFFFFFFFF,     /* OROUT25-GenTag[24]: Unused */    \
    0xFFFFFFFF          /* OROUT25-GenTag[25]: Unused */

/* Combiner OROUT FiltAND LSB Cfg. for DIGI use case plus Clause 8.3 */
#define comb_orout_filtand_en_lsb_VALUES    \
  0x00000001,        /* OROUT0-GenTag[0]: Link OAM */    \
    0x00000002,        /* OROUT1-GenTag[1]: SyncE ESMC */    \
    0x00000004,        /* OROUT2-GenTag[2]: LACP */        \
    0x00000008,        /* OROUT3-GenTag[3]: Marker */        \
    0x00000010,        /* OROUT4-GenTag[4]: Management VLAN */    \
    0x000000E0,        /* OROUT5-GenTag[5]: PTP */        \
    0x00000020,        /* OROUT6-GenTag[6]: PTP over Ethernet */    \
    0x00000040,        /* OROUT7-GenTag[7]: PTP over UDP ov IPv4 ov Eth. */ \
    0x00000080,        /* OROUT8-GenTag[8]: PTP over UDP ov IPv6 ov Eth. */ \
    0x00000700,        /* OROUT9-GenTag[9]: PTP Event */    \
    0x00003800,        /* OROUT10-GenTag[10]: PTP General */    \
    0x0001C000,        /* OROUT11-GenTag[11]: PTP Sync or Delay_Req */    \
    0x007E0000,        /* OROUT12-GenTag[12]: PTP Multicast DA */    \
    0x1F800000,        /* OROUT13-GenTag[13]: PTP Unicast DA */    \
    0xE0000000,        /* OROUT14-GenTag[14]: PTP not(Unicast DA) */    \
    0x00000000,        /* OROUT15-GenTag[15]: IPv4 ARP */    \
    0x00000000,        /* OROUT16-GenTag[16]: IPv6 NDP */    \
    0x00000000,        /* OROUT17-GenTag[17]: PRBS */        \
    0x00000000,        /* OROUT18-GenTag[18]: not(PRBS) */    \
    0x00000000,        /* OROUT19-GenTag[19]: Slow Protocol with Bridge Destination */    \
    0x00000000,        /* OROUT20-GenTag[20]: Slow Protocol with Reserved Destination (not the typical Slow Pr. Destination) */ \
    0x00000000,        /* OROUT21-GenTag[21]: Slow Protocol with GARP Destination */ \
    0x00000000,        /* OROUT22-GenTag[22]: Slow Protocol with not(known Destinations) */ \
    0x00000000,        /* OROUT23-GenTag[23]: Slow Protocol with typical destination and not(known subtype) */    \
    0xFFFFFFFF,        /* OROUT24-GenTag[24]: Unused */    \
    0xFFFFFFFF         /* OROUT25-GenTag[25]: Unused */

    /* lebeldan 28-09-2012: tags 15 to 18 were in error, tag 18 is in fact unused*/    
    /* Combiner OROUT FiltAND MSB Cfg. for DIGI use case*/
#define eclass_field_COMB_OROUT_FILTAND_EN_MSB_VALUES    \
  0x00,        /* OROUT0-GenTag[0]: Link OAM */    \
    0x00,    /* OROUT1-GenTag[1]: SyncE ESMC */    \
    0x00,    /* OROUT2-GenTag[2]: LACP */        \
    0x00,    /* OROUT3-GenTag[3]: Marker */        \
    0x00,    /* OROUT4-GenTag[4]: Management VLAN */    \
    0x00,    /* OROUT5-GenTag[5]: PTP */        \
    0x00,    /* OROUT6-GenTag[6]: PTP over Ethernet */    \
    0x00,    /* OROUT7-GenTag[7]: PTP over UDP ov IPv4 ov Eth. */ \
    0x00,    /* OROUT8-GenTag[8]: PTP over UDP ov IPv6 ov Eth. */ \
    0x00,    /* OROUT9-GenTag[9]: PTP Event */        \
    0x00,    /* OROUT10-GenTag[10]: PTP General */        \
    0x00,    /* OROUT11-GenTag[11]: PTP Sync or Delay_Req */    \
    0x00,    /* OROUT12-GenTag[12]: PTP Multicast DA */    \
    0x00,    /* OROUT13-GenTag[13]: PTP Unicast DA */    \
    0x00,    /* OROUT14-GenTag[14]: PTP not(Unicast DA) */    \
    0x01,    /* OROUT15-GenTag[15]: IPv4 ARP */        \
    0x02,    /* OROUT16-GenTag[16]: IPv6 NDP */        \
    0x04,    /* OROUT17-GenTag[17]: PRBS */            \
    0xFF,    /* OROUT18-GenTag[18]: Unused */    \
    0xFF,    /* OROUT19-GenTag[19]: Unused */        \
    0xFF,    /* OROUT20-GenTag[20]: Unused */        \
    0xFF,    /* OROUT21-GenTag[21]: Unused */        \
    0xFF,    /* OROUT22-GenTag[22]: Unused */        \
    0xFF,    /* OROUT23-GenTag[23]: Unused */        \
    0xFF,    /* OROUT24-GenTag[24]: Unused */        \
    0xFF     /* OROUT25-GenTag[25]: Unused */

/* lebeldan: Define Added for Clause 8.3 28-09-2012 */
/* Combiner OROUT FiltAND MSB Cfg. for DIGI use case plus Clause 8.3 */
#define eclass_field_COMB_OROUT_FILTAND_EN_MSB_VALUES_CLAUSE_8_3    \
  0x00,        /* OROUT0-GenTag[0]: Link OAM */    \
    0x00,    /* OROUT1-GenTag[1]: SyncE ESMC */    \
    0x00,    /* OROUT2-GenTag[2]: LACP */        \
    0x00,    /* OROUT3-GenTag[3]: Marker */        \
    0x00,    /* OROUT4-GenTag[4]: Management VLAN */    \
    0x00,    /* OROUT5-GenTag[5]: PTP */        \
    0x00,    /* OROUT6-GenTag[6]: PTP over Ethernet */    \
    0x00,    /* OROUT7-GenTag[7]: PTP over UDP ov IPv4 ov Eth. */ \
    0x00,    /* OROUT8-GenTag[8]: PTP over UDP ov IPv6 ov Eth. */ \
    0x00,    /* OROUT9-GenTag[9]: PTP Event */        \
    0x00,    /* OROUT10-GenTag[10]: PTP General */        \
    0x00,    /* OROUT11-GenTag[11]: PTP Sync or Delay_Req */    \
    0x00,    /* OROUT12-GenTag[12]: PTP Multicast DA */    \
    0x00,    /* OROUT13-GenTag[13]: PTP Unicast DA */    \
    0x00,    /* OROUT14-GenTag[14]: PTP not(Unicast DA) */    \
    0x01,    /* OROUT15-GenTag[15]: IPv4 ARP */        \
    0x02,    /* OROUT16-GenTag[16]: IPv6 NDP */        \
    0x04,    /* OROUT17-GenTag[17]: PRBS */            \
    0x04,    /* OROUT18-GenTag[18]: not(PRBS) */    \
    0x00,    /* OROUT19-GenTag[19]: Slow Protocol with Bridge Destination */    \
    0x08,    /* OROUT20-GenTag[20]: Slow Protocol with Reserved Destination (not the typical Slow Pr. Destination) */ \
    0x00,    /* OROUT21-GenTag[21]: Slow Protocol with GARP Destination */ \
    0x10,    /* OROUT22-GenTag[22]: Slow Protocol with not(known Destinations) */ \
    0x20,    /* OROUT23-GenTag[23]: Slow Protocol with typical destination and not(known subtype) */    \
    0xFF,    /* OROUT24-GenTag[24]: Unused */        \
    0xFF     /* OROUT25-GenTag[25]: Unused */

/* OROUT CFG Filter Ouput Enables for Clause 8.3*/

/* OROUT19: Slow Protocol with Bridge Destination uses CFG tag 0 */
#define eclass_field_COMB_OROUT_CFG_FOUT_EN_EQ_19_VALUE 0x01 
/* OROUT21: Slow Protocol with GARP   Destination uses CFG Tag 2 */
#define eclass_field_COMB_OROUT_CFG_FOUT_EN_EQ_21_VALUE 0x04 
/* Valid OROUTS not using CFG Filter Outputs */
#define eclass_field_COMB_OROUT_CFG_FOUT_EN_LET_23_VALUE 0x00 
/* Invalid OROUTS : below ok but not necessary for > 23 since Combiner output should be disabled */
#define eclass_field_COMB_OROUT_CFG_FOUT_EN_GT_23_VALUE 0xFF

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Forwarded references
*/
PRIVATE PMC_ERROR eclass_combiner_cfg_pkt_type_EXTRACT_PTP_PKTS(eclass_handle_t *hndl, 
                                                                UINT32 link);          
PRIVATE PMC_ERROR eclass_combiner_cfg_pkt_type_G8021_CLAUSE_8_3(eclass_handle_t *hndl, UINT32 link);

PRIVATE PMC_ERROR eclass_extract_cfg_idx_get(eclass_handle_t            *hndl,
                                             UINT32                      link, 
                                             BOOL8                       enable,
                                             eclass_pkt_extract_cfg_t   *extract_cfg_ptr,
                                             INT32                      *free_idx);

/*
** Log Messaging
*/
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
* The following creates the LOG/ERR strings from above by pulling out the text
* portion of the LOG_ERR_TABLE above
*/
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char ECLASS_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ECLASS_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ECLASS_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Public Functions
*/

/*******************************************************************************
* eclass_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ECLASS block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ECLASS block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   eclass_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC eclass_handle_t *eclass_ctxt_create(pmc_handle_t *parent,
                       UINT32 base_address, 
                       pmc_sys_handle_t *sys_handle, 
                       const char *tsb_name )
{
    eclass_handle_t *eclass_handle;
    
    PMC_ENTRY();

    eclass_handle = (eclass_handle_t*)PMC_CTXT_CALLOC(sizeof(eclass_handle_t), parent);

    pmc_handle_init( parent, eclass_handle, sys_handle, PMC_MID_DIGI_ECLASS, tsb_name, base_address );

    /* Register log strings. */
    pmc_log_block_strings_register(ECLASS_LOG_ERR_STRINGS[0], ECLASS_LOG_ERR_TABLE_BASE, ECLASS_LOG_ERR_COUNT);

    PMC_RETURN(eclass_handle);
} /* eclass_ctxt_create */

/*******************************************************************************
* eclass_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ECLASS block instance.
*
*
* INPUTS:
*   *eclass_handle        - pointer to ECLASS handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void eclass_ctxt_destroy(eclass_handle_t *eclass_handle)
{
    PMC_ENTRY();
    PMC_CTXT_FREE(&eclass_handle, eclass_handle);

    PMC_RETURN();
} /* eclass_ctxt_destroy */

/*******************************************************************************
* eclass_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a ECLASS block handle instance.
*
*
* INPUTS:
*   *eclass_handle        - pointer to ECLASS handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void eclass_handle_init(eclass_handle_t *eclass_handle)
{
    PMC_ENTRY();

    eclass_var_default_init(eclass_handle);

/*
    ((eclass_cfg_t *)(&eclass_handle->cfg))->num_eclass_chans = ECLASS_NUM_CHANNELS;
*/
    PMC_RETURN();
} /* eclass_handle_init */

/*******************************************************************************
* eclass_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on eclass.
*
*
* INPUTS:
*   *eclass_handle        - pointer to ECLASS handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_handle_restart_init(eclass_handle_t *eclass_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(ECLASS_LOG_ERR_STRINGS[0], ECLASS_LOG_ERR_TABLE_BASE, ECLASS_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* eclass_handle_restart_init */

/*******************************************************************************
* eclass_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the ECLASS instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to eclass_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance
*   energy_state          - enum defining state to set the ECLASS module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_init(eclass_handle_t *hndl, pmc_energy_state_t energy_state)
{
    UINT32 i;
    UINT32 j;

    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY();
    PMC_ASSERT(hndl!=NULL, ECLASS_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Configure non default values */
        eclass_block_non_default_cfg(hndl);
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* Configure eclass var_t as default */
        eclass_var_default_init(hndl);
            
    } else {
        PMC_ASSERT(FALSE, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    }    

    for (i = 0; i < ECLASS_NUM_LINKS; i++)
    {
        for (j = 0; j < ECLASS_NUM_SUPPORTED_PKT_PER_LINK; j++)
        {
            hndl->var.eclass_ctxt.extract_cfg_tbl[i][j].valid = FALSE;
            hndl->var.eclass_ctxt.extract_cfg_tbl[i][j].pkt_type = LAST_UTIL_GLOBAL_PKT_TYPE;
            if (ECLASS_SUPPORTED_PKT_TYPE > j)
            {
                hndl->var.eclass_ctxt.extract_cfg_tbl[i][j].is_vlan = FALSE;
            }
            else
            {
                hndl->var.eclass_ctxt.extract_cfg_tbl[i][j].is_vlan = TRUE;
            }
        }
    }
    PMC_RETURN(ret_val);

} /* eclass_init */

/*******************************************************************************
* eclass_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   eclass instance.
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                  - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL eclass_start_state_test(eclass_handle_t *hndl)
{
    BOOL start_state = TRUE;
    UINT32 link;
    
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);

    for(link = 0; link < ECLASS_NUM_LINKS; link++)
    {
        if(hndl->var.eclass_link_en[link] == TRUE)
            start_state = FALSE;
    }
    
    PMC_RETURN(start_state);

} /* eclass_start_state_test */

/*******************************************************************************
* eclass_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of a ECLASS block instance.
*
*
* INPUTS:
*   *eclass_handle        - pointer to ECLASS handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ECLASS.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 eclass_base_address_get(eclass_handle_t *eclass_handle)
{
    PMC_ENTRY();
    PMC_RETURN(eclass_handle->base.base_address);
} /* eclass_base_address_get */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/

/*******************************************************************************
* eclass_extract_packet
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Program the eclass to extract PTP packets
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   pkt_type              - the packet type. See eclass_extract_pkt_t for more
*                           infromation.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ECLASS.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_extract_packet(eclass_handle_t *hndl, 
                       UINT32 link, 
                       eclass_extract_pkt_t pkt_type) 
{

    PMC_ENTRY();


    eclass_combiner_cfg( hndl, link, pkt_type );

    eclass_routing_filter_cfg( hndl, link, pkt_type );

    eclass_nomatch_cfg( hndl, link, pkt_type);


    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* eclass_combiner_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Program the eclass to extract PTP packets
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   pkt_type              - the packet type. See eclass_extract_pkt_t for more
*                           infromation.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ECLASS.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_combiner_cfg(eclass_handle_t *hndl, 
                     UINT32 link, 
                     eclass_extract_pkt_t pkt_type )
{

    PMC_ENTRY();


    switch (pkt_type)
    {
    case ECLASS_EXTRACT_PTP_PKTS :
    {
    eclass_combiner_cfg_pkt_type_EXTRACT_PTP_PKTS(hndl, link);
    break;
    }

    case ECLASS_G8021_CLAUSE_8_3 :
    {

    eclass_combiner_cfg_pkt_type_G8021_CLAUSE_8_3(hndl, link);
    break;
    }
    default:
    {
    break;
    }
    }

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* eclass_routing_filter_CFG
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Program the eclass to extract PTP packets
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   pkt_type              - the packet type. See eclass_extract_pkt_t for more
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ECLASS.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_routing_filter_cfg(eclass_handle_t *hndl, 
                       UINT32 link, 
                       eclass_extract_pkt_t pkt_type )
{
    eclass_buffer_t eclass_buffer[1];

    PMC_ENTRY();

    /* Disable all ROUTING_tag */
    eclass_field_ROUTING_OR0_OUTPUT_EN_set( NULL, hndl, link, 0x0);
    eclass_field_ROUTING_OR1_OUTPUT_EN_set( NULL, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_OUTPUT_EN_set( NULL, hndl, link, 0x0);
    eclass_field_ROUTING_OR3_OUTPUT_EN_set( NULL, hndl, link, 0x0);
    eclass_field_ROUTING_OR4_OUTPUT_EN_set( NULL, hndl, link, 0x0);
    eclass_field_ROUTING_OR5_OUTPUT_EN_set( NULL, hndl, link, 0x0);


    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    switch (pkt_type)
    {
    case ECLASS_EXTRACT_PTP_PKTS :
    {
    /*Config Routing_tag[0] for no_match_all */
    eclass_field_ROUTING_OR0_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x1);
    eclass_field_ROUTING_OR0_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0x0);

    /* Config ROUTING_tag[1] (CPU extract) for Link OAM, SyncE ESMC, */
    /*  IPv4 ARP, Ipv6 NDP, PRBS & PTP packets */
    eclass_field_ROUTING_OR1_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR1_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x38023);
    eclass_field_ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR1_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR1_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0x0);
    
    /* Config ROUTING_tag[2] (Management extract) for LACP, Marker, & Management VLAN */
    eclass_field_ROUTING_OR2_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x1C);
    eclass_field_ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0x0);
    
    /* Config ROUTING_tag[5] (Timestamp flag) for all PTP packets */
    eclass_field_ROUTING_OR5_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR5_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x20);
    eclass_field_ROUTING_OR5_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR5_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR5_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0x0);
    
    eclass_field_ROUTING_OR0_OUTPUT_INV_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR1_OUTPUT_INV_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_OUTPUT_INV_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR5_OUTPUT_INV_set( eclass_buffer, hndl, link, 0x0);
    
    /* Enable ROUTING_tag[0], ROUTING_tag[1] and ROUTING_tag[2] */
    eclass_field_ROUTING_OR0_OUTPUT_EN_set( NULL, hndl, link, 0x1);
    eclass_field_ROUTING_OR1_OUTPUT_EN_set( NULL, hndl, link, 0x1);
    eclass_field_ROUTING_OR2_OUTPUT_EN_set( NULL, hndl, link, 0x1);    eclass_field_ROUTING_OR5_OUTPUT_EN_set( NULL, hndl, link, 0x1);
    break;
    }
    case ECLASS_EXTRACT_ALL_PKTS_MGMT:
    {
    /*Config Routing_tag[0] for no_match_all */
    eclass_field_ROUTING_OR0_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0x0);
          
    /* Config ROUTING_tag[2] (Management extract) for LACP, Marker, & Management VLAN */
    eclass_field_ROUTING_OR2_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0xFFFF);
        
    eclass_field_ROUTING_OR0_OUTPUT_INV_set( NULL, hndl, link, 0x0);
    eclass_field_ROUTING_OR2_OUTPUT_INV_set( NULL, hndl, link, 0x0);

    /* Enable ROUTING_tag[0], ROUTING_tag[1] and ROUTING_tag[2] */
    eclass_field_ROUTING_OR0_OUTPUT_EN_set( NULL, hndl, link, 0x1);
    eclass_field_ROUTING_OR2_OUTPUT_EN_set( NULL, hndl, link, 0x1);
    break;
    }
    case ECLASS_EXTRACT_ALL_PKTS_CPU:
    {
    /*Config Routing_tag[0] for no_match_all */
    eclass_field_ROUTING_OR0_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x0);
    eclass_field_ROUTING_OR0_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0x0);

    /* Config ROUTING_tag[1] (CPU extract) for Link OAM, SyncE ESMC, */
    /*  IPv4 ARP, Ipv6 NDP, PRBS & PTP packets */
    eclass_field_ROUTING_OR1_INPUT_VLANMPLS_EN_set( eclass_buffer, hndl, link, 0x3F);
    eclass_field_ROUTING_OR1_INPUT_GEN_EN_set( eclass_buffer, hndl, link, 0x3FFFFFF);
    eclass_field_ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN_set( eclass_buffer, hndl, link, 0x1);
    eclass_field_ROUTING_OR1_INPUT_NO_MATCH_ALL_EN_set( eclass_buffer, hndl, link, 0x1);
    eclass_field_ROUTING_OR1_INPUT_ANDOUT_EN_set( eclass_buffer, hndl, link, 0x0);
        
    eclass_field_ROUTING_OR0_OUTPUT_INV_set( NULL, hndl, link, 0x0);
    eclass_field_ROUTING_OR1_OUTPUT_INV_set( NULL, hndl, link, 0x0);

    /* Enable ROUTING_tag[0], ROUTING_tag[1] and ROUTING_tag[2] */
    eclass_field_ROUTING_OR0_OUTPUT_EN_set( NULL, hndl, link, 0x1);
    eclass_field_ROUTING_OR1_OUTPUT_EN_set( NULL, hndl, link, 0x1);
    break;
    }
    default :
    {
    break;
    }
    }

    eclass_buffer_flush(eclass_buffer);

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* eclass_nomatch_CFG
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Program the eclass to extract PTP packets
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   pkt_type              - the packet type. See eclass_extract_pkt_t for more
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ECLASS.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_nomatch_cfg(eclass_handle_t *hndl, 
                    UINT32 link, 
                    eclass_extract_pkt_t pkt_type )
{

    eclass_buffer_t eclass_buffer[1];

    PMC_ENTRY();

    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    switch ( pkt_type ) {
    case ECLASS_EXTRACT_PTP_PKTS :
    {
        /* 17 valid tags are used for the 'no match' */
        eclass_field_NO_MATCH_EN_GEN_set( eclass_buffer, hndl, 0x003FFFF);
    
        /*keep no_match_en_vlanmpls == ALLVALID; */
        eclass_field_NO_MATCH_EN_VLANMPLS_set( eclass_buffer, hndl, 0x3F);
        break;
    }
    case ECLASS_G8021_CLAUSE_8_3 :
    {
        /* 22 of the 23 tags are used for the 'no match' */
        /* Tag 18 - (not(PRBS)) does not impact the 'no match' */
        eclass_field_NO_MATCH_EN_GEN_set( eclass_buffer, hndl, 0x0FBFFFF);

        /*keep no_match_en_vlanmpls == ALLVALID;*/
        eclass_field_NO_MATCH_EN_VLANMPLS_set( eclass_buffer, hndl, 0x3F);
        break;
    }
    default :
    {
        break;
    }
    }

    eclass_buffer_flush(eclass_buffer);

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* eclass_activate_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activate/desactivate eclass
*
* INPUTS:
*   *hndl               - pointer to ECLASS handle instance.
*   link                - the link to be operated on.
*   enable              - true means activate, false means deactivate
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_activate_deactivate(eclass_handle_t *hndl, 
                                            UINT32 link,
                                            BOOL enable)
{

    PMC_ENTRY();
    
    hndl->var.eclass_link_en[link] = enable;

    switch (link) 
    {
    case 0 : {
    eclass_field_ECLASS120_LINK0_EN_set(NULL, hndl, enable);
    break;
    }
    case 1 : {
    eclass_field_ECLASS120_LINK1_EN_set(NULL, hndl, enable);
    break;
    }
    case 2 : {
    eclass_field_ECLASS120_LINK2_EN_set(NULL, hndl, enable);
    break;
    }
    case 3 : {
    eclass_field_ECLASS120_LINK3_EN_set(NULL, hndl, enable);
    break;
    }
    case 4 : {
    eclass_field_ECLASS120_LINK4_EN_set(NULL, hndl, enable);
    break;
    }
    case 5 : {
    eclass_field_ECLASS120_LINK5_EN_set(NULL, hndl, enable);
    break;
    }
    case 6 : {
    eclass_field_ECLASS120_LINK6_EN_set(NULL, hndl, enable);
    break;
    }
    case 7 : {
    eclass_field_ECLASS120_LINK7_EN_set(NULL, hndl, enable);
    break;
    }
    case 8 : {
    eclass_field_ECLASS120_LINK8_EN_set(NULL, hndl, enable);
    break;
    }
    case 9 : {
    eclass_field_ECLASS120_LINK9_EN_set(NULL, hndl, enable);
    break;
    }
    case 10 : {
    eclass_field_ECLASS120_LINK10_EN_set(NULL, hndl, enable);
    break;
    }
    case 11 : {
    eclass_field_ECLASS120_LINK11_EN_set(NULL, hndl, enable);
    break;
    }
    }

    PMC_RETURN(PMC_SUCCESS);
} /*eclass_activate_deactivate*/

/*******************************************************************************
*  eclass_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   hndl     - handle to the ECLASS instance to be operated on
*   link     - link to activate
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_activate(eclass_handle_t *hndl, 
                 UINT32 link)
{
    PMC_ERROR result;
    PMC_ENTRY();
    
    result = eclass_activate_deactivate(hndl, link, TRUE);
    PMC_RETURN(result);

} /* eclass_activate */

/*******************************************************************************
*  eclass_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable channel.
*
* INPUTS:
*   hndl     - handle to the ECLASS instance to be operated on
*   link     - link to configure

*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_deactivate(eclass_handle_t *hndl, 
                   UINT32 link)
{
    PMC_ERROR result;
    PMC_ENTRY();

    result = eclass_activate_deactivate(hndl, link, FALSE);
    PMC_RETURN(result);

} /* eclass_deactivate */

/*******************************************************************************
*  eclass_general_config_filter_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable channel.
*
* INPUTS:
*   hndl                  - handle to the ECLASS instance to be operated on
*   link                  - link to configure
*   pkt_type              - packet type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_general_config_filter_cfg(eclass_handle_t *hndl, 
                          UINT32 link, 
                          eclass_extract_pkt_t pkt_type )
{
    UINT32 i;
    eclass_buffer_t eclass_buffer[1];


    PMC_ENTRY();

    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    switch (pkt_type) {
    case ECLASS_G8021_CLAUSE_8_3 :

        /* CONFIG F16 */
        for(i = 0; i < ECLASS_F16_ARRAY_MAX_SIZE; i++) {
            if(i<=11) {
                /* F16_0: Slow Protocol DA[0] */
            eclass_field_F16_0_VALUE_set (eclass_buffer, hndl, i, 0x0180); 
                eclass_field_F16_0_MASK_set  (eclass_buffer, hndl, i, 0xFFFF);
        /* F16_1: Slow Protocol DA[1] */
                eclass_field_F16_1_VALUE_set (eclass_buffer, hndl, i, 0xC200); 
                eclass_field_F16_1_MASK_set  (eclass_buffer, hndl, i, 0xFFFF);
        /* F16_2: Slow Protocol DA[2] for Bridge Eth. Addr.*/
                eclass_field_F16_2_VALUE_set (eclass_buffer, hndl, i, 0x0010); 
                eclass_field_F16_2_MASK_set  (eclass_buffer, hndl, i, 0xFFFF);
        /* F16_3: Slow Protocol DA[2] for Reserved Eth. Addr.*/
                eclass_field_F16_3_VALUE_set (eclass_buffer, hndl, i, 0x000F); 
                eclass_field_F16_3_MASK_set  (eclass_buffer, hndl, i, 0xFFFF);
        /* F16_4: Slow Protocol DA[2] for GARP Eth. Addr.*/
                eclass_field_F16_4_VALUE_set (eclass_buffer, hndl, i, 0x002C); 
                eclass_field_F16_4_MASK_set  (eclass_buffer, hndl, i, 0xFFFF);
        /* F16_5: Slow Protocol Type */
                eclass_field_F16_5_VALUE_set (eclass_buffer, hndl, i, 0x8809); 
                eclass_field_F16_5_MASK_set  (eclass_buffer, hndl, i, 0xFFFF);

                eclass_field_F16_0_ADDR_set   (eclass_buffer, hndl, i, 0x0);
                eclass_field_F16_0_ENABLE_set (eclass_buffer, hndl, i, 1);
                eclass_field_F16_1_ADDR_set   (eclass_buffer, hndl, i, 0x1);
                eclass_field_F16_1_ENABLE_set (eclass_buffer, hndl, i, 1);
                eclass_field_F16_2_ADDR_set   (eclass_buffer, hndl, i, 0x2);
                eclass_field_F16_2_ENABLE_set (eclass_buffer, hndl, i, 1);
                eclass_field_F16_3_ADDR_set   (eclass_buffer, hndl, i, 0x2);
                eclass_field_F16_3_ENABLE_set (eclass_buffer, hndl, i, 1);
                eclass_field_F16_4_ADDR_set   (eclass_buffer, hndl, i, 0x2);
                eclass_field_F16_4_ENABLE_set (eclass_buffer, hndl, i, 1);
                eclass_field_F16_5_ADDR_set   (eclass_buffer, hndl, i, 0x6);
                eclass_field_F16_5_ENABLE_set (eclass_buffer, hndl, i, 1);
        eclass_buffer_flush(eclass_buffer);
            }
        }

        /* CONFIG FILTER_AND */
        for(i = 0; i < ECLASS_FANDS_ARRAY_MAX_SIZE; i++) {
        /* Slow Protocol with Bridge Destination */
            eclass_field_FAND_0_F16BITS_EN_set (NULL, hndl, i, 0x0027); 
        /* Slow Protocol with Reserved Destination (that is not the typical Slow Protocol Destination) */
            eclass_field_FAND_1_F16BITS_EN_set (NULL, hndl, i, 0x002B); 
        /* Slow Protocol with GARP Destination */
            eclass_field_FAND_2_F16BITS_EN_set (NULL, hndl, i, 0x0033); 
        }

        /* CONFIG FILTER_OUT */
        for(i = 0; i < ECLASS_FOUT_X_FAND_ARRAY_MAX_SIZE; i++) {
        /* FOUT_0: Slow Protocol with Bridge Destination */
            eclass_field_FOUT_0_FAND_EN_set    (eclass_buffer, hndl, i, 0x0001); 
            eclass_field_FOUT_0_EN_set         (eclass_buffer, hndl, i, 0x1);
        /* FOUT_1: Slow Protocol with Reserved Destination (that is not the typical Slow Protocol Destination) */
            eclass_field_FOUT_1_FAND_EN_set    (eclass_buffer, hndl, i, 0x0002); 
            eclass_field_FOUT_1_EN_set         (eclass_buffer, hndl, i, 0x1);
        /* FOUT_2: Slow Protocol with GARP Destination */
            eclass_field_FOUT_2_FAND_EN_set    (eclass_buffer, hndl, i, 0x0004); 
            eclass_field_FOUT_2_EN_set         (eclass_buffer, hndl, i, 0x1);
        /* FOUT_3: Slow Protocol Type */
            eclass_field_FOUT_3_F16BITS_EN_set (eclass_buffer, hndl, i, 0x0020); 
            eclass_field_FOUT_3_EN_set         (eclass_buffer, hndl, i, 0x1);
        eclass_buffer_flush(eclass_buffer);
        }

        break;
    default :
        break;
    }

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  eclass_ptp_unicastdacheck_over_eth_filter_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   cfg unicast da over ethernet check filter for PTP.
*
* INPUTS:
*   hndl                  - handle to the ECLASS instance to be operated on
*   link                  - link to configure
*   da_msb                - MAC destination address
*   da_lsb                - MAC destination address
*   enable                - enable/disable filter 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_ptp_unicastdacheck_over_eth_filter_cfg(eclass_handle_t *hndl, 
                          UINT32 link, 
                          UINT32 da_msb,
                          UINT32 da_lsb,
                          BOOL enable)
{
    PMC_ENTRY();
    if (enable) 
    {
         /*enable DA checking*/
        /*addr  0x726CA0 + 4 x 25*/
        UINT32 regvalue=0x0;
        regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 26);
        eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 26, 0xFFFFFFFF, 0, (regvalue | (1<<6)));
        /*addr  0x726CA0 + 4 x 26*/ 
        regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 25);
        eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 25, 0xFFFFFFFF, 0, (regvalue | (1<<5)));

         /*addr 0x724074*/
         eclass_reg_UNICAST_DA_ETH_LSB_array_write(0, hndl, link, (da_lsb&0xffffffff));
         /*addr 0x724078*/
         eclass_reg_UNICAST_DA_ETH_MSB_array_write(0, hndl, link, ((0x1<<16) | (da_msb & 0xffff)));
         /*addr 0x72407C*/
         eclass_reg_UNICAST_DA_ETH_P2P_LSB_array_write(0, hndl, link, (da_lsb & 0xffffffff) );
         /*addr 0x724080*/
         eclass_reg_UNICAST_DA_ETH_P2P_MSB_array_write(0, hndl, link, ((0x1<<16) | (da_msb & 0xffff)));

        /*configure Filter*/
        /*addr 0x724178*/
        eclass_reg_ROUTING_AND_FILTER_14_array_write( 0, hndl, link, 0x1040 );

        /*#addr 0x72417c*/
        eclass_reg_ROUTING_AND_FILTER_15_array_write(0, hndl, link, 0x2040);
        
        /*addr 0x7241a8*/
        eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write(0, hndl, link, 0x38003);

        /*addr 0x7241ac*/
        eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write(0, hndl, link, 0x8000c000);

        /*addr 0x7241c8*/
        eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write(0, hndl, link, 0x0);

        /*addr 0x7241cc*/
        eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write(0, hndl, link, 0x8000c000);

       
     }
     else 
     {
        /*addr0x7241cc*/
        eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write(0, hndl, link, 0x80007FFF);
        /*addr0x7241c8*/
        eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write(0, hndl, link, 0x1C0);
        /*addr0x7241ac*/
        eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write(0, hndl, link, 0x80007FFF);
        /*addr0x7241a8*/
        eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write(0, hndl, link, 0x381C3);
        /*addr0x72417c*/
        eclass_reg_ROUTING_AND_FILTER_15_array_write(0, hndl, link, 0x0);
        /*addr0x724178*/
        eclass_reg_ROUTING_AND_FILTER_14_array_write( 0, hndl, link, 0x82200000);

        /*addr0x724080*/
        eclass_reg_UNICAST_DA_ETH_P2P_MSB_array_write(0, hndl, link, 0x0);
        /*addr0x72407C*/
        eclass_reg_UNICAST_DA_ETH_P2P_LSB_array_write(0, hndl, link, 0x0);
        /*addr0x724078*/
        eclass_reg_UNICAST_DA_ETH_MSB_array_write(0, hndl, link, 0x0);
        /*addr0x724074*/
        eclass_reg_UNICAST_DA_ETH_LSB_array_write(0, hndl, link, 0x0);
         
     }

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  eclass_ptp_unicastdacheck_over_ipv4_filter_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   cfg unicast da over ipv4 check filter for PTP.
*
* INPUTS:
*   hndl                  - handle to the ECLASS instance to be operated on
*   link                  - link to configure
*   ipv4_da               - destination address
*   enable                - enable/disable filter 
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_ptp_unicastdacheck_over_ipv4_filter_cfg(eclass_handle_t *hndl, 
                          UINT32 link, 
                          UINT32 ipv4_da,
                          BOOL enable)
{
    PMC_ENTRY();

   
    
    if (enable)
    {
        /*enable unicast DA checkign for PTP over IPv4*/
        /*addr0x724088*/
        eclass_reg_UNICAST_DA_IPV4_array_write(0, hndl, link, (ipv4_da & 0xffffffff));
        /*addr 0x72408c*/
        eclass_reg_UNICAST_DA_IPV4_P2P_array_write(0, hndl, link, (ipv4_da & 0xffffffff));
        /*addr 0x725B00*/
         eclass_reg_GEN_CFG_HWIRED_FILTER_array_write(0, hndl, link, (0x80000021 | (1<<20) | (1<<16)));

        /*configure Filter*/
        /*addr 0x724178*/
        eclass_reg_ROUTING_AND_FILTER_14_array_write( 0, hndl, link, 0x1080 );
        /*addr 0x72417c*/
        eclass_reg_ROUTING_AND_FILTER_15_array_write(0, hndl, link, 0x2080);
        /*addr 0x7241a8*/
        eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write(0, hndl, link, 0x38003);
        /*addr 0x7241ac*/
        eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write(0, hndl, link, 0x8000c000);
        /*addr 0x7241c8*/
        eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write(0, hndl, link, 0x0);
        /*addr 0x7241cc*/
        eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write(0, hndl, link, 0x8000c000);

        /*To disable the Ethernet DA check from your IPV4 Unicast DA checking*/
        UINT32 regvalue=0x0;
        /*addr  0x726CA0 + 4 x 25*/
        regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 25);
        /*eclass_reg_FOUT_COMB_AND_1_array_burst_write(0, hndl, link, (regvalue & (~(1<<5))))*/
        eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 25, 0xFFFFFFFF, 0, (regvalue & (~(1<<5))));
        /*addr 0x726CA0 + 4 x 26*/
        regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 26);
        /*eclass_reg_FOUT_COMB_AND_1_array_burst_write(0, hndl, link, (regvalue & (~(1<<5))))*/
        eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 26, 0xFFFFFFFF, 0, (regvalue & (~(1<<6))));

    }
    else 
    {
        /*addr0x00726ca0*/
        /*
         *regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 26);
         *eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 26, 0xFFFFFFFF, 0, (regvalue & (1<<6)));
         *regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 25);
         *eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 25, 0xFFFFFFFF, 0, (regvalue & (1<<5)));
         */

        /*addr0x7241cc*/
        eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write(0, hndl, link, 0x80007FFF);
        /*addr0x7241c8*/
         eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write(0, hndl, link, 0x1C0);
        /*addr0x7241ac*/
        eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write(0, hndl, link, 0x80007FFF);
        /*addr0x7241a8*/
        eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write(0, hndl, link, 0x381C3);
        /*addr0x72417c*/
        eclass_reg_ROUTING_AND_FILTER_15_array_write(0, hndl, link, 0x0);
        /*addr0x724178*/
        eclass_reg_ROUTING_AND_FILTER_14_array_write( 0, hndl, link, 0x82200000);

        /*addr0x725B00*/
        eclass_reg_GEN_CFG_HWIRED_FILTER_array_write(0, hndl, link, 0x80000021);
        /*addr0x72408c*/
        eclass_reg_UNICAST_DA_IPV4_P2P_array_write(0, hndl, link, 0x0);
        /*addr0x724088*/
        eclass_reg_UNICAST_DA_IPV4_array_write(0, hndl, link, 0x0);
        
    }
    
    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  eclass_ptp_vlancheck_tci_filter_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   cfg vlan tci check filter for PTP.
*
* INPUTS:
*   hndl                  - handle to the ECLASS instance to be operated on
*   link                  - link to configure
*   filter_type           - filter type
*   vlan_cfg_type         - vlan cfg type
*   tci_value1_inner      - tci value of VLAN tag 1
*   tci_value2_outer      - tci value of VLAN tag 2
*   enable                - enable/disable filter 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_ptp_vlancheck_tci_filter_cfg(eclass_handle_t *hndl, 
                          UINT32 link, 
                          eclass_vlan_tci_check_filter_type_t filter_type,
                          eclass_vlan_cfg_t vlan_cfg_type,
                          UINT16 tci_value1_inner,
                          UINT16 tci_value2_outer,
                          BOOL enable)
{

    PMC_ENTRY();

    
    if (enable) 
    {
         /*enable DA checking*/
        /*addr  0x726CA0 + 4 x 25*/
        UINT32 regvalue=0x0;
        regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 26);
        eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 26, 0xFFFFFFFF, 0, (regvalue | (1<<6)));
        /*addr  0x726CA0 + 4 x 26*/ 
        regvalue = eclass_reg_FOUT_COMB_AND_1_array_read(0, hndl, 25);
        eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 25, 0xFFFFFFFF, 0, (regvalue | (1<<5)));

        
        eclass_vlan_cfg(hndl, link, vlan_cfg_type);

        switch (vlan_cfg_type) 
        {
            case ECLASS_VLAN_CFG_NO_VLAN:
              break;
            case ECLASS_VLAN_CFG_1VLAN:
              /*addr 0x724038*/
              eclass_reg_VLANMPLS_FILT20_TAG0_1_array_field_set(0, hndl, link, 0xFFFFF, 0,  ((tci_value1_inner<<4) & 0xFFFFF));
              /*addr 0x72403C*/
              eclass_reg_VLANMPLS_FILT20_TAG0_2_array_write(0, hndl, link, 0x1010FFF0);
              break;
            case ECLASS_VLAN_CFG_2VLAN:
              /*addr addr0x724038*/
              /*addr 0x72403C*/
              eclass_reg_VLANMPLS_FILT20_TAG0_1_array_field_set(0, hndl, link, 0xFFFFF, 0,  ((tci_value1_inner<<4) & 0xFFFFF));
              eclass_reg_VLANMPLS_FILT20_TAG1_1_array_field_set(0, hndl, link, 0xFFFFF, 0,  ((tci_value2_outer<<4) & 0xFFFFF));
              eclass_reg_VLANMPLS_FILT20_TAG0_2_array_write(0, hndl, link, 0x3010FFF0);
              eclass_reg_VLANMPLS_FILT20_TAG1_2_array_write(0, hndl, link, 0x3010FFF0);
              break;
        }
      
        /*ROUTING_AND13_INPUT_VLANMPLS_EN= 0x1;*/
        eclass_field_ROUTING_AND13_INPUT_VLANMPLS_EN_set(0, hndl, link, 1);

        /*addr 0x724174*/
        switch (filter_type) 
        {
            case ECLASS_VLAN_TCI_CHECK_OVER_ETHERNET: /*for Ethernet*/
                eclass_reg_ROUTING_AND_FILTER_13_array_write( 0, hndl, link, 0x04000040);
                break;
            case ECLASS_VLAN_TCI_CHECK_OVER_IPV4: /*for IPv4*/
                eclass_reg_ROUTING_AND_FILTER_13_array_write( 0, hndl, link, 0x04000080);
                break;
        }
       
        /*addr 0x7241a8*/
        eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write(0, hndl, link, 0x38003);
        /*addr 0x7241ac*/
        eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write(0, hndl, link, 0x80002000);
        /*addr 0x7241c8*/
        eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write(0, hndl, link, 0x0);
        /*addr 0x7241cc*/
        eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write(0, hndl, link, 0x80002000);

    }
    else 
    {
        /*addr0x7241cc*/
        eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write(0, hndl, link, 0x80007FFF);
        /*addr0x7241c8*/
        eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write(0, hndl, link, 0x1C0);
        /*addr0x7241ac*/
        eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write(0, hndl, link, 0x80007FFF);
        /*addr0x7241a8*/
        eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write(0, hndl, link, 0x381C3);

        /*addr0x724174*/
        eclass_reg_ROUTING_AND_FILTER_13_array_write( 0, hndl, link, 0x82100000);

        /*addr0x72403C*/
        eclass_reg_VLANMPLS_FILT20_TAG0_2_array_write(0, hndl, link, 0x102FFFF0);
        /*addr0x724038*/
        eclass_reg_VLANMPLS_FILT20_TAG0_1_array_field_set(0, hndl, link, 0xFFFFF, 0, 0x81000);

        /*addr0x72403C*/
        eclass_reg_VLANMPLS_FILT20_TAG0_2_array_write(0, hndl, link, 0x102FFFF0);
        /*addr0x724040*/
        eclass_reg_VLANMPLS_FILT20_TAG1_1_array_field_set(0, hndl, link, 0xFFFFF, 0, 0x91000);

        /*eclass_vlan_cfg(hndl, link, ECLASS_VLAN_CFG_NO_VLAN);*/
        /*addr0x724024*/
        eclass_reg_GEN_CFG_VLAN_MPLS_array_write(0, hndl, link, 0x7F0200);
        /*addr0x724030*/
        eclass_field_MPLS_CUSTOM_ETHERTYPE_set(0, hndl, link, 0x0000);

    }
    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  eclass_vlan_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable channel.
*
* INPUTS:
*   hndl                  - handle to the ECLASS instance to be operated on
*   link                  - link to configure
*   vlan_cfg_type         - VLAN type (none, dot1q or QinQ)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_vlan_cfg(eclass_handle_t *hndl, 
                 UINT32 link, 
                 eclass_vlan_cfg_t vlan_cfg_type )
{
    eclass_buffer_t eclass_buffer[1];


    PMC_ENTRY();

    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    switch (vlan_cfg_type) 
    {
    case ECLASS_VLAN_CFG_NO_VLAN :
    eclass_field_MPLS_CUSTOM_ETHERTYPE_set        ( NULL, hndl, link, 0x0000 );
    eclass_field_MPLS_ETYPE_CUSTOM_EN_set         ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_ETYPE_8848_EN_set           ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_ETYPE_8847_EN_set           ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_NB_LEVELS_set               ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_OVERFLOW_CFG_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_TPID_CUSTOM_OUTER_EN_set    ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_CUSTOM_INNER_EN_set    ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_88A8_EN_set            ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_9100_EN_set            ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_8100_EN_set            ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_OUTER_EN_set                ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_INNER_EN_set                ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_MPLS_STATE_EN_set           ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_PRIO_EN_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_PRIO_DEFAULT_set       ( eclass_buffer, hndl, link, 0x7 );
    eclass_field_VLAN_MPLS_PRIO_LEVEL_set         ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_PRIO_TYPE_set          ( eclass_buffer, hndl, link, 0x0 );
    eclass_buffer_flush(eclass_buffer);
    break;
    case ECLASS_VLAN_CFG_1VLAN :
    eclass_field_MPLS_CUSTOM_ETHERTYPE_set        ( NULL, hndl, link, 0x0000 );
    eclass_field_MPLS_ETYPE_CUSTOM_EN_set         ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_ETYPE_8848_EN_set           ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_ETYPE_8847_EN_set           ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_NB_LEVELS_set               ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_OVERFLOW_CFG_set            ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_CUSTOM_OUTER_EN_set    ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_CUSTOM_INNER_EN_set    ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_TPID_88A8_EN_set            ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_9100_EN_set            ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_8100_EN_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_OUTER_EN_set                ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_INNER_EN_set                ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_STATE_EN_set           ( eclass_buffer, hndl, link, 0x3 );
    eclass_field_VLAN_MPLS_PRIO_EN_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_PRIO_DEFAULT_set       ( eclass_buffer, hndl, link, 0x7 );
    eclass_field_VLAN_MPLS_PRIO_LEVEL_set         ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_PRIO_TYPE_set          ( eclass_buffer, hndl, link, 0x0 );
    eclass_buffer_flush(eclass_buffer);
    break;
    case ECLASS_VLAN_CFG_2VLAN :
    eclass_field_MPLS_CUSTOM_ETHERTYPE_set        ( NULL, hndl, link, 0x0000 );
    eclass_field_MPLS_ETYPE_CUSTOM_EN_set         ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_ETYPE_8848_EN_set           ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_ETYPE_8847_EN_set           ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_NB_LEVELS_set               ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_MPLS_OVERFLOW_CFG_set            ( eclass_buffer, hndl, link, 0x0 );
    eclass_field_VLAN_TPID_CUSTOM_OUTER_EN_set    ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_TPID_CUSTOM_INNER_EN_set    ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_TPID_88A8_EN_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_TPID_9100_EN_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_TPID_8100_EN_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_OUTER_EN_set                ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_INNER_EN_set                ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_STATE_EN_set           ( eclass_buffer, hndl, link, 0x3 );
    eclass_field_VLAN_MPLS_PRIO_EN_set            ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_PRIO_DEFAULT_set       ( eclass_buffer, hndl, link, 0x7 );
    eclass_field_VLAN_MPLS_PRIO_LEVEL_set         ( eclass_buffer, hndl, link, 0x1 );
    eclass_field_VLAN_MPLS_PRIO_TYPE_set          ( eclass_buffer, hndl, link, 0x0 );
    eclass_buffer_flush(eclass_buffer);
    break;
    default :
    break;
    }

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  eclass_vlan_custom_tpid_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable channel.
*
* INPUTS:
*   hndl                  - handle to the ECLASS instance to be operated on
*   link                  - link to configure
*   custom_tpid_inner     - inner VLAN TPID
*   custom_tpid_outer     - outer VLAN TPID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_vlan_custom_tpid_cfg(eclass_handle_t *hndl, 
                         UINT32 link, 
                         UINT32 custom_tpid_inner, 
                         UINT32 custom_tpid_outer )
{
    eclass_buffer_t eclass_buffer[1];


    PMC_ENTRY();

    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    eclass_field_VLAN_CUSTOM_TPID_OUTER_set       ( eclass_buffer, hndl, link, custom_tpid_outer);
    eclass_field_VLAN_CUSTOM_TPID_INNER_set       ( eclass_buffer, hndl, link, custom_tpid_inner);

    eclass_buffer_flush(eclass_buffer);

    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
*  eclass_filt20_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable channel.
*
* INPUTS:
*   hndl                  - handle to the ECLASS instance to be operated on
*   link                  - link to configure
*   tag                   - tag index (0 to 5)
*   value                 - value to compare
*   mask                  - mask to compare
*   addr                  - address offset
*   inv                   - tag inversion
*   type                  - tag type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_filt20_cfg(eclass_handle_t *hndl, 
                   UINT32 link, 
                   UINT32 tag, 
                   UINT32 value, 
                   UINT32 mask, 
                   UINT32 addr, 
                   UINT32 inv, 
                   UINT32 type )
{
    eclass_buffer_t eclass_buffer[1];


    PMC_ENTRY();

    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    switch (tag) {
    case 0 :
        eclass_field_FILT20_TAG0_VALUE_set       ( NULL, hndl, link, value );
        eclass_field_FILT20_TAG0_MASK_set        ( eclass_buffer, hndl, link, mask );
        eclass_field_FILT20_TAG0_ADDR_set        ( eclass_buffer, hndl, link, addr );
        eclass_field_FILT20_TAG0_INV_set         ( eclass_buffer, hndl, link, inv );
        eclass_field_FILT20_TAG0_TYPE_set        ( eclass_buffer, hndl, link, type );
        break;
    case 1 :
        eclass_field_FILT20_TAG1_VALUE_set       ( NULL, hndl, link, value );
        eclass_field_FILT20_TAG1_MASK_set        ( eclass_buffer, hndl, link, mask );
        eclass_field_FILT20_TAG1_ADDR_set        ( eclass_buffer, hndl, link, addr );
        eclass_field_FILT20_TAG1_INV_set         ( eclass_buffer, hndl, link, inv );
        eclass_field_FILT20_TAG1_TYPE_set        ( eclass_buffer, hndl, link, type );
        break;
    case 2 :
        eclass_field_FILT20_TAG2_VALUE_set       ( NULL, hndl, link, value );
        eclass_field_FILT20_TAG2_MASK_set        ( eclass_buffer, hndl, link, mask );
        eclass_field_FILT20_TAG2_ADDR_set        ( eclass_buffer, hndl, link, addr );
        eclass_field_FILT20_TAG2_INV_set         ( eclass_buffer, hndl, link, inv );
        eclass_field_FILT20_TAG2_TYPE_set        ( eclass_buffer, hndl, link, type );
        break;
    case 3 :
        eclass_field_FILT20_TAG3_VALUE_set       ( NULL, hndl, link, value );
        eclass_field_FILT20_TAG3_MASK_set        ( eclass_buffer, hndl, link, mask );
        eclass_field_FILT20_TAG3_ADDR_set        ( eclass_buffer, hndl, link, addr );
        eclass_field_FILT20_TAG3_INV_set         ( eclass_buffer, hndl, link, inv );
        eclass_field_FILT20_TAG3_TYPE_set        ( eclass_buffer, hndl, link, type );
        break;
    case 4 :
        eclass_field_FILT20_TAG4_VALUE_set       ( NULL, hndl, link, value );
        eclass_field_FILT20_TAG4_MASK_set        ( eclass_buffer, hndl, link, mask );
        eclass_field_FILT20_TAG4_ADDR_set        ( eclass_buffer, hndl, link, addr );
        eclass_field_FILT20_TAG4_INV_set         ( eclass_buffer, hndl, link, inv );
        eclass_field_FILT20_TAG4_TYPE_set        ( eclass_buffer, hndl, link, type );
        break;
    case 5 :
        eclass_field_FILT20_TAG5_VALUE_set       ( NULL, hndl, link, value );
        eclass_field_FILT20_TAG5_MASK_set        ( eclass_buffer, hndl, link, mask );
        eclass_field_FILT20_TAG5_ADDR_set        ( eclass_buffer, hndl, link, addr );
        eclass_field_FILT20_TAG5_INV_set         ( eclass_buffer, hndl, link, inv );
        eclass_field_FILT20_TAG5_TYPE_set        ( eclass_buffer, hndl, link, type );
        break;
    default :
        break;
    }

    eclass_buffer_flush(eclass_buffer);
    PMC_RETURN(PMC_SUCCESS);
}


/*******************************************************************************
* eclass_activated_link_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns link status.
*                                                                               
* INPUTS:                                                                       
*   *hndl             - pointer to ECLASS handle instanec
*                                                                             
* OUTPUTS:
*   *actv_link         - links status: TRUE -> activated, FALSE -> deactivated.
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void eclass_activated_link_get(eclass_handle_t   *hndl,
                                      BOOL8             *actv_link)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(hndl != NULL, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < ECLASS_NUM_LINKS; itr++)
    {
        switch (itr) 
        {
        case 0 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK0_EN_get(NULL, hndl);
            break;
        }
        case 1 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK1_EN_get(NULL, hndl);
            break;
        }
        case 2 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK2_EN_get(NULL, hndl);
            break;
        }
        case 3 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK3_EN_get(NULL, hndl);
            break;
        }
        case 4 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK4_EN_get(NULL, hndl);
            break;
        }
        case 5 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK5_EN_get(NULL, hndl);
            break;
        }
        case 6 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK6_EN_get(NULL, hndl);
            break;
        }
        case 7 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK7_EN_get(NULL, hndl);
            break;
        }
        case 8 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK8_EN_get(NULL, hndl);
            break;
        }
        case 9 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK9_EN_get(NULL, hndl);
            break;
        }
        case 10 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK10_EN_get(NULL, hndl);
            break;
        }
        case 11 : {
            actv_link[itr] = eclass_field_ECLASS120_LINK11_EN_get(NULL, hndl);
            break;
        }
        default:
            break;
        }
    }

    PMC_RETURN();
}  /* eclass_activated_link_get */

/*
** Private Functions
*/

/*******************************************************************************
* eclass_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Non-default register settings associated with the ECLASS block in 
*   ENET subsystem.
*
*
* INPUTS:
*   *hndl         - pointer to ECLASS handle instance
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void eclass_block_non_default_cfg(eclass_handle_t *hndl)
{
    UINT32 i;

    PMC_ENTRY();
    
    for(i = 0; i < ECLASS_NUM_LINKS; i++ )
      {
        eclass_reg_GEN_CFG_VLAN_MPLS_array_write( 0, hndl, i, 0 );
      }
      
/* This code was inactive in eclass_access_init() because clear_rams argument
   was always false.

    if( clear_rams ) 
      {
        
        for(i = 0; i < ECLASS_NUM_LINKS; i++ )
          {
            eclass_reg_VLAN_TPID_array_write( 0, hndl, i, 0 );
            eclass_reg_MPLS_ETHERTYPE_array_write( 0, hndl, i, 0 );

            eclass_reg_VLANMPLS_FILT20_TAG0_1_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG0_2_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG1_1_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG1_2_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG2_1_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG2_2_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG3_1_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG3_2_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG4_1_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG4_2_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG5_1_array_write( 0, hndl, i, 0 );
            eclass_reg_VLANMPLS_FILT20_TAG5_2_array_write( 0, hndl, i, 0 );

            eclass_reg_ROUTING_AND_FILTER_0_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_1_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_2_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_3_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_4_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_5_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_6_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_7_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_8_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_9_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_10_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_11_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_12_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_13_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_14_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_AND_FILTER_15_array_write( 0, hndl, i, 0 );

            eclass_reg_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_array_write( 0, hndl, i, 0 );

            eclass_reg_ROUTING_OR_FILTER_0_REG1_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_0_REG2_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_2_REG1_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_2_REG2_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_3_REG1_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_3_REG2_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_4_REG1_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_4_REG2_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write( 0, hndl, i, 0 );
            eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write( 0, hndl, i, 0 );


            eclass_reg_GEN_FILTER_CFG_FAND_0_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FAND_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FAND_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FAND_3_array_write( 0, hndl, i, 0 );

            eclass_reg_GEN_FILTER_CFG_FOR_0_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FOR_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FOR_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FOR_3_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FOR_4_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FOR_5_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FOR_6_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FOR_7_array_write( 0, hndl, i, 0 );

            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_0_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_3_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_4_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_5_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_6_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER_OUT_7_array_write( 0, hndl, i, 0 );

            eclass_reg_MAN_VLAN_CFG_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DEST_PORT_UDP_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV4_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV4_P2P_array_write( 0, hndl, i, 0 );

            eclass_reg_UNICAST_DA_ETH_MSB_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_ETH_LSB_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_ETH_P2P_MSB_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_ETH_P2P_LSB_array_write( 0, hndl, i, 0 );

            eclass_reg_UNICAST_DA_IPV6_P2P_0_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV6_P2P_1_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV6_P2P_2_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV6_P2P_3_array_write( 0, hndl, i, 0 );

            eclass_reg_UNICAST_DA_IPV6_0_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV6_1_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV6_2_array_write( 0, hndl, i, 0 );
            eclass_reg_UNICAST_DA_IPV6_3_array_write( 0, hndl, i, 0 );
           
          }

        for(i = 0; i < (2*ECLASS_NUM_LINKS); i++ )
          {
            eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_2_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_1_array_write( 0, hndl, i, 0 );
            eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_2_array_write( 0, hndl, i, 0 );
          }

      }
*/

    PMC_RETURN();
} /* eclass_block_non_default_cfg */

/*******************************************************************************
*  eclass_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes ECLASS var_t variables.
*
* INPUTS:
*   *hndl          - pointer to ECLASS handle instance to be operated on 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void eclass_var_default_init(eclass_handle_t *hndl)
{
    UINT32 i;
    PMC_ENTRY();
    
    /* Init eclass_var_t as devault */
    for(i = 0; i < ECLASS_NUM_LINKS; i++ )
    {
        hndl->var.eclass_link_en[i] = FALSE;
    }

    PMC_RETURN();
} /* eclass_var_default_init */

/*******************************************************************************
* eclass_combiner_cfg_EXTRACT_PTP_PKTS
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Program the eclass to clasify and route DIGI usecase packets: slow packets, PTP packets.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ECLASS.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR eclass_combiner_cfg_pkt_type_EXTRACT_PTP_PKTS(eclass_handle_t *hndl, 
                                UINT32 link)
{

    UINT32 i;
    eclass_buffer_t eclass_buffer[1];
    UINT32 eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_val[] = {eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_VALUES};
/*    UINT32 eclass_reg_FOUT_COMB_AND_1_array_val[] = {eclass_reg_FOUT_COMB_AND_1_array_VALUES};*/
    UINT32 eclass_field_COMB_AND_FILTORIN_EN_val[] = {eclass_field_COMB_AND_FILTORIN_EN_VALUES};
    UINT32 eclass_reg_FOUT_COMB_OROUT_1_array_val[] = {eclass_reg_FOUT_COMB_OROUT_1_array_VALUES};
    UINT32 eclass_field_COMB_OROUT_FILTAND_EN_MSB_val[] = {eclass_field_COMB_OROUT_FILTAND_EN_MSB_VALUES};

    PMC_ENTRY();
    
    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    /*ORIN stages */    

    for(i = 0; i < ECLASS_COMB_ORIN_OUT_INV_ARRAY_MAX_SIZE; i++ )
    {
        eclass_field_COMB_ORIN_OUT_INV_set( NULL, hndl, i, 0x0);
    }
    
    /* ORIN CFG Filter Enables */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 0,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 1,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 2,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 3,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 4,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 5,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 6,  0x00); /* CFG Filter Unused */
    /* below lines added lebeldan 28-09-2012 */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 7,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 8,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 9,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 10,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 11,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 12,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 13,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 14,  0x00); /* CFG Filter Unused */
    eclass_field_COMB_ORIN_CFG_FOUT_EN_set( NULL, hndl, 15,  0x00); /* CFG Filter Unused */

    /* Combiner ORIN Stage HWired LSB Cfg.for DIGI use case */    
    /*specific per FILTer */
    /* ORIN0: IPv4 UDP port */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 0, 0xFFFFFFFF, 0, 0x00C00000);
    /* ORIN1: IPv6 UDP port */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 1, 0xFFFFFFFF, 0, 0x80000000);
    /* ORIN2: PTP Event */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 2, 0xFFFFFFFF, 0, 0x00000000);
    /* ORIN3: PTP General */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 3, 0xFFFFFFFF, 0, 0x00000000);
    /* ORIN4: PTP Sync or Deleay_Req */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 4, 0xFFFFFFFF, 0, 0x00000000);
    /* ORIN5: PTP P_Delay_Req or P_Delay_Resp or P_Delay_Follow_Up */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 5, 0xFFFFFFFF, 0, 0x00000000);
    /* ORIN6: All PTP but P_Delay_Req, P_Delay_Resp, and P_Delay_Follow_Up    */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 6, 0xFFFFFFFF, 0, 0x00000000);
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 7, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    /* below lines added lebeldan 28-09-2012 */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 8, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 9, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 10, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 11, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 12, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 13, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 14, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */
    eclass_reg_FOUT_COMB_ORIN_1_array_field_set( NULL, hndl, 15, 0xFFFFFFFF, 0, 0xFFFFFFFF); /*unused */

    /* Combiner ORIN Stage HWired MSB Cfg.for DIGI use case plus Clause 8.3*/
    /*specific per FILTer */  
    /* ORIN0: IPv4 UDP port */  
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 0,  0x00000);
    /* ORIN1: IPv6 UDP port */
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 1,  0x00001);
    /* ORIN2: PTP Event */
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 2,  0x00F00);
    /* ORIN3: PTP General */
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 3,  0x3F000);
    /* ORIN4: PTP Sync or Deleay_Req */
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 4,  0x00300);
    /* ORIN5: PTP P_Delay_Req or P_Delay_Resp or P_Delay_Follow_Up */
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 5,  0x04C00);
    /* ORIN6: All PTP but P_Delay_Req, P_Delay_Resp, and P_Delay_Follow_Up    */
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 6,  0x3B300);
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 7,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    /* below lines added lebeldan 28-09-2012 */
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 8,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 9,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 10,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 11,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 12,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 13,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 14,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set( NULL, hndl, 15,  ECLASS120_REG_FOUT_COMB_ORIN_2_BIT_COMB_ORIN_HWIRED_FOUT_EN_MSB_MSK);/*unused*/
    
    /*AND stages */
    /*common CFGs for all */
    /* specific per FILTer */
/*    eclass_lfield_COMB_AND_HWIRED_FOUT_EN_LSB_set( NULL, hndl, eclass_reg_FOUT_COMB_AND_1_array_val); */
 
          /* Combiner AND Stage HWired LSB Cfg.for DIGI use case*/
          /* AND0: Link OAM */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 0, 0xFFFFFFFF, 0, 0x00000003);
          /* AND1: SyncE ESMC */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 1, 0xFFFFFFFF, 0, 0x00000003);
      /* AND2: LACP */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 2, 0xFFFFFFFF, 0, 0x00000003);
      /* AND3: Marker */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 3, 0xFFFFFFFF, 0, 0x00000003);
      /* AND4: Management VLAN */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 4, 0xFFFFFFFF, 0, 0x00000004);
      /* AND5: PTP over Ethernet */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 5, 0xFFFFFFFF, 0, 0x00000100);
      /* AND6: PTP over UDP over IPv4 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 6, 0xFFFFFFFF, 0, 0x00018200);
      /* AND7: PTP over UDP over IPv6 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 7, 0xFFFFFFFF, 0, 0x03000400);
      /* AND8: PTP Event over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 8, 0xFFFFFFFF, 0, 0x00000100);
      /* AND9: PTP Event over UDP over IPv4 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 9, 0xFFFFFFFF, 0, 0x00418200);
      /* AND10: PTP Event over UDP over IPv6 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 10, 0xFFFFFFFF, 0, 0x83000400);
      /* AND11: PTP General over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 11, 0xFFFFFFFF, 0, 0x00000100);
      /* AND12: PTP General over UDP over IPv4 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 12, 0xFFFFFFFF, 0, 0x00818200);
      /* AND13: PTP General over UDP over IPv6 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 13, 0xFFFFFFFF, 0, 0x03000400);
      /* AND14: PTP (Sync or Delay_Req) over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 14, 0xFFFFFFFF, 0, 0x00000100);
      /* AND15: PTP (Sync or Delay_Req) over UDP over IPv4 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 16, 0xFFFFFFFF, 0, 0x00418200);
      /* AND16: PTP (Sync or Delay_Req) over UDP over IPv6 over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 16, 0xFFFFFFFF, 0, 0x83000400);
      /* AND17: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 17, 0xFFFFFFFF, 0, 0x00000108);
      /* AND18: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 18, 0xFFFFFFFF, 0, 0x00000110);
      /* AND19: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 19, 0xFFFFFFFF, 0, 0x00038200);
      /* AND20: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 20, 0xFFFFFFFF, 0, 0x00058200);
      /* AND21: PTP Multicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 21, 0xFFFFFFFF, 0, 0x07000400);
      /* AND22: PTP Multicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 22, 0xFFFFFFFF, 0, 0x0B000400);
      /* AND23: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 23, 0xFFFFFFFF, 0, 0x00000120);
      /* AND24: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 24, 0xFFFFFFFF, 0, 0x00000140);
      /* AND25: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 25, 0xFFFFFFFF, 0, 0x00098220);
      /* AND26: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 26, 0xFFFFFFFF, 0, 0x00108240);
      /* AND27: PTP Unicast DA not(P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 27, 0xFFFFFFFF, 0, 0x13000420);
      /* AND28: PTP Unicast DA (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 28, 0xFFFFFFFF, 0, 0x23000440);
      /* AND29: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) over Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 29, 0xFFFFFFFF, 0, 0x00000180);
      /* AND30: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv4 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 30, 0xFFFFFFFF, 0, 0x00218200);
      /* AND31: PTP Not(Unicast DA) (P_Delay_Req or P_Delay_Resp or P_Delay_Resp_Follow_Up) ov UDP ov IPv6 ov Eth. */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 31, 0xFFFFFFFF, 0, 0x43000400);
      /* AND32: IPv4 ARP */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 32, 0xFFFFFFFF, 0, 0x00001000);
      /* AND33: IPv6 NDP */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 33, 0xFFFFFFFF, 0, 0x00002000);
      /* AND34: PRBS */
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 34, 0xFFFFFFFF, 0, 0x00004000);
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 35, 0xFFFFFFFF, 0, 0x00000000);/*unused*/
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 36, 0xFFFFFFFF, 0, 0x00000000);/*unused*/
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 37, 0xFFFFFFFF, 0, 0x00000000);/*unused*/
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 38, 0xFFFFFFFF, 0, 0x00000000);/*unused*/
          eclass_reg_FOUT_COMB_AND_1_array_field_set( NULL, hndl, 39, 0xFFFFFFFF, 0, 0x00000000);/*unused*/

    for(i = 0; i < ECLASS_COMB_AND_ARRAY_MAX_SIZE; i++ )
    {
        eclass_field_COMB_AND_CFG_FOUT_EN_set( eclass_buffer, hndl, i, 0x00);
        eclass_field_COMB_AND_OUT_INV_set( eclass_buffer, hndl, i, 0x0);
        eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_set( eclass_buffer, hndl, i, eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_val[i]);
        eclass_field_COMB_AND_FILTORIN_EN_set( eclass_buffer, hndl, i, eclass_field_COMB_AND_FILTORIN_EN_val[i]);
    }
   
    eclass_buffer_flush(eclass_buffer);

    /*OROUT stages*/
    /*common CFGs for all*/

    /*specific per FILTer*/
    eclass_lfield_COMB_OROUT_FILTAND_EN_LSB_set( NULL, hndl, eclass_reg_FOUT_COMB_OROUT_1_array_val);

    for (i = 0; i < ECLASS_COMB_OROUT_FILTAND_EN_MSB_ARRAY_MAX_SIZE; i++){
        eclass_field_COMB_OROUT_FILTAND_EN_MSB_set( NULL, hndl, i, eclass_field_COMB_OROUT_FILTAND_EN_MSB_val[i]);
    };

    
        for(i = 0; i < ECLASS_FOUT_COMB_OROUT_1_ARRAY_MAX_SIZE; i++ ) {

    /* lebeldan 28-09-2012: the below code does not make sense since the CFG filter is unused here, commented out*/
    /*    if( i <= 7 ) {
        eclass_field_COMB_OROUT_CFG_FOUT_EN_set( NULL, hndl, i,  0x00);
    } else {
        eclass_field_COMB_OROUT_CFG_FOUT_EN_set( NULL, hndl, i,  0xFF);
    }*/

    /* lebeldan 28-09-2012: the below code is good and is uncommented*/    
       eclass_field_COMB_OROUT_CFG_FOUT_EN_set( eclass_buffer, hndl, i,  0x00); /* Cfg tag are not used at all, just set them to 0x00*/ 

        if( i <= 17 ) { /* Valid OROUT Filters */
            eclass_field_COMB_OROUT_OUT_EN_set( eclass_buffer, hndl, i,  0x1);
        } else {
            eclass_field_COMB_OROUT_OUT_EN_set( eclass_buffer, hndl, i,  0x0);
        }

        eclass_field_COMB_OROUT_OUT_INV_set( eclass_buffer, hndl, i,  0x0);
    }

    eclass_buffer_flush(eclass_buffer);

    PMC_RETURN(PMC_SUCCESS);
}


/*******************************************************************************
* eclass_combiner_cfg_pkt_type_G8021_CLAUSE_8_3
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Program the eclass to clasify and route DIGI usecase packets: slow packets, 
*   PTP packets, and G8021 clause 8.3 packets.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address of ECLASS.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR eclass_combiner_cfg_pkt_type_G8021_CLAUSE_8_3(eclass_handle_t *hndl, 
                                                                UINT32 link)
{
    UINT32 i;
    UINT32 comb_orin_hwired_fout_en_lsb_val[] = {comb_orin_hwired_fout_en_lsb_VALUES};
    UINT32 comb_and_hwired_fout_en_lsb_val[] = {comb_and_hwired_fout_en_lsb_VALUES};
    UINT32 comb_orout_filtand_en_lsb_val[] = {comb_orout_filtand_en_lsb_VALUES};
    eclass_buffer_t eclass_buffer[1];    
    UINT32 eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_val[] = {eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_VALUES};
    UINT32 eclass_field_COMB_AND_FILTORIN_EN_val[] = {eclass_field_COMB_AND_FILTORIN_EN_VALUES};
    UINT32 eclass_field_COMB_OROUT_FILTAND_EN_MSB_val[] = {eclass_field_COMB_OROUT_FILTAND_EN_MSB_VALUES_CLAUSE_8_3};
    UINT32 eclass_COMB_ORIN_HWIRED_FOUT_EN_MSB_val[] = {eclass_COMB_ORIN_HWIRED_FOUT_EN_MSB_VALUES};

    PMC_ENTRY();

    /* initialize ECLASS buffer used to optimize read/modify access */
    eclass_buffer_init(eclass_buffer, hndl);

    /* OR stages */
    /* common cfgs for all */
    for(i = 0; i < ECLASS_COMB_ORIN_OUT_INV_ARRAY_MAX_SIZE; i++) {
    if(i>=7 && i<=9) {
        eclass_field_COMB_ORIN_OUT_INV_set (NULL, hndl, i, 1);
        /* ORIN7: not(Slow Protocol Typical Destination) */
        /* ORIN8: not(Slow Protocol known Destinations) */
        /* ORIN9: not(Slow Protocol known subtypes) */
    } else {
        eclass_field_COMB_ORIN_OUT_INV_set (NULL, hndl, i, 0);
    }

    if(i==8) {
        /* ORIN8: not(Slow Protocol known Destinations) */
        eclass_field_COMB_ORIN_CFG_FOUT_EN_set (NULL, hndl, i, 0x07);
    } else if(i<=9) {
        /* All other used ORIN Filters*/
        eclass_field_COMB_ORIN_CFG_FOUT_EN_set (NULL, hndl, i, 0x00);
    } else {
        /* Unused ORIN Filters */
        eclass_field_COMB_ORIN_CFG_FOUT_EN_set (NULL, hndl, i, 0xFF);
    }

    }

    eclass_buffer_flush(eclass_buffer);

    /* specific per filter */
    eclass_lfield_COMB_ORIN_HWIRED_FOUT_EN_LSB_set (NULL, hndl, comb_orin_hwired_fout_en_lsb_val);

    for (i = 0; i < 16; i++){
    eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set (NULL, hndl, i, eclass_COMB_ORIN_HWIRED_FOUT_EN_MSB_val[i]);
    };

    /* AND stages */
    /* common cfgs for all */
    for(i = 0; i < ECLASS_COMB_AND_ARRAY_MAX_SIZE; i++) {
    if(i!=35) {
        /* AND Filters that do not use teh CFG Filter outputs */
        eclass_field_COMB_AND_CFG_FOUT_EN_set (NULL, hndl, i, 0x00);
    } else {
      /* AND35: Uses CFG Filter output tag 1 */
          /* AND35: Slow Protocol with Reserved Destination that is not the Typical Slow Protocol Destination */
        eclass_field_COMB_AND_CFG_FOUT_EN_set (NULL, hndl, i, 0x02);
    }
    }

    for(i = 0; i < ECLASS_COMB_AND_ARRAY_MAX_SIZE; i++) {
    eclass_field_COMB_AND_OUT_INV_set (NULL, hndl, i, 0);
    };

    /* specific per filter */
    eclass_lfield_COMB_AND_HWIRED_FOUT_EN_LSB_set (NULL, hndl, comb_and_hwired_fout_en_lsb_val);


    for(i = 0; i < ECLASS_COMB_AND_ARRAY_MAX_SIZE; i++ )
    {
    eclass_field_COMB_AND_CFG_FOUT_EN_set( eclass_buffer, hndl, i, 0x00);
    eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_set( 
        eclass_buffer, hndl, 0, 
        eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_val[i]);
    eclass_buffer_flush(eclass_buffer);

    eclass_field_COMB_AND_OUT_INV_set( eclass_buffer, hndl, i, 0x0);
    eclass_field_COMB_AND_FILTORIN_EN_set( 
        eclass_buffer, hndl, i, eclass_field_COMB_AND_FILTORIN_EN_val[i]);
    }    

    eclass_buffer_flush(eclass_buffer);

    /* OROUT stages */
    /* common cfgs for all */

    eclass_lfield_COMB_OROUT_FILTAND_EN_LSB_set (NULL, hndl, comb_orout_filtand_en_lsb_val);

    for (i = 0; i < ECLASS_COMB_OROUT_FILTAND_EN_MSB_ARRAY_MAX_SIZE; i++){

    eclass_field_COMB_OROUT_FILTAND_EN_MSB_set( 
        NULL, hndl, i, 
        eclass_field_COMB_OROUT_FILTAND_EN_MSB_val[i]);

    };

    for(i = 0; i < ECLASS_FOUT_COMB_OROUT_1_ARRAY_MAX_SIZE; i++) {
        if(i == 19) { /* OROUT19 uses CFG Filter Outputs : Slow Protocol with Bridge Destination */
        eclass_field_COMB_OROUT_CFG_FOUT_EN_set (eclass_buffer, hndl, i, eclass_field_COMB_OROUT_CFG_FOUT_EN_EQ_19_VALUE);
    } 
    else if(i == 21) /* OROUT21 uses CFG Filter Outputs : Slow Protocol with GARP Destination */
    {
        eclass_field_COMB_OROUT_CFG_FOUT_EN_set (eclass_buffer, hndl, i, eclass_field_COMB_OROUT_CFG_FOUT_EN_EQ_21_VALUE);
    } 
    else if(i <= 23) /* OROUT23 uses CFG Filter Outputs : Slow Protocol with GARP Destination */
    {
        eclass_field_COMB_OROUT_CFG_FOUT_EN_set (eclass_buffer, hndl, i, eclass_field_COMB_OROUT_CFG_FOUT_EN_LET_23_VALUE);
    } 
    else /* OROUT Outputs that do not directly use the CFG Filter Outputs */
    {
        eclass_field_COMB_OROUT_CFG_FOUT_EN_set (eclass_buffer, hndl, i, eclass_field_COMB_OROUT_CFG_FOUT_EN_GT_23_VALUE);
    }

    if (i == 18) /* OROUT18 inversion: not(PRBS) */
    {
        eclass_field_COMB_OROUT_OUT_INV_set (eclass_buffer, hndl, i, 1);
    } else {
        eclass_field_COMB_OROUT_OUT_INV_set (eclass_buffer, hndl, i, 0);
    }

    if(i <= 23) /* Valid OROUT Filters */
    {
        eclass_field_COMB_OROUT_OUT_EN_set (eclass_buffer, hndl, i, 1);
    }  
    else 
    {
        eclass_field_COMB_OROUT_OUT_EN_set (eclass_buffer, hndl, i, 0);
    }
    eclass_buffer_flush(eclass_buffer);
    }


    PMC_RETURN(PMC_SUCCESS);
}

/*******************************************************************************
* eclass_pkt_extract_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures eclass pkt extraction properties.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   enable                - enable/disable packet extraction
*   extract_cfg_ptr       - pointer to extraction configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR eclass_pkt_extract_cfg(eclass_handle_t            *hndl, 
                                        UINT32                      link, 
                                        BOOL8                       enable,
                                        eclass_pkt_extract_cfg_t   *extract_cfg_ptr)
{
    INT32 free_idx = -1;
    BOOL8 found = FALSE;
    UINT32 pkt_type;    
    PMC_ERROR rc = PMC_SUCCESS;


    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ECLASS_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != extract_cfg_ptr,  ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(ECLASS_NUM_LINKS > link, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    
    pkt_type = (UINT32)extract_cfg_ptr->pkt_type;
   
    rc = eclass_extract_cfg_idx_get(hndl,
                                    link, 
                                    enable,
                                    extract_cfg_ptr,
                                    &free_idx);

    if (PMC_SUCCESS == rc && -1 != free_idx)
    {
        found = TRUE;
    }
    
    if (PMC_SUCCESS == rc &&
        TRUE == found)
    {
        /* update extract entry depending on operation: add/remove */ 
        if (TRUE == enable)
        {
            PMC_MEMCPY(&hndl->var.eclass_ctxt.extract_cfg_tbl[link][free_idx],
                       extract_cfg_ptr,
                       sizeof(eclass_pkt_extract_cfg_t));
            hndl->var.eclass_ctxt.extract_cfg_tbl[link][free_idx].valid = TRUE;
        }
        else
        {
            hndl->var.eclass_ctxt.extract_cfg_tbl[link][free_idx].valid = FALSE;
            hndl->var.eclass_ctxt.extract_cfg_tbl[link][free_idx].pkt_type = LAST_UTIL_GLOBAL_PKT_TYPE;
            hndl->var.eclass_ctxt.extract_cfg_tbl[link][free_idx].is_vlan = FALSE;
        }
    }
    if (PMC_SUCCESS == rc && FALSE == found)
    {
        rc = ECLASS_ERR_EXTRACT_VLAN_NOT_FOUND;
    }

    PMC_RETURN(rc);
}/* eclass_pkt_extract_cfg */

/*******************************************************************************
* eclass_link_pkt_extract_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup all entries in eclass context associated to given link.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be cleaned.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void eclass_link_pkt_extract_cleanup(eclass_handle_t            *hndl, 
                                            UINT32                      link)
{
    INT32 itr;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ECLASS_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ECLASS_NUM_LINKS > link, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
   
    
    for (itr = 0; itr < ECLASS_NUM_SUPPORTED_PKT_PER_LINK; itr += 1 )
    {        
        /* update extract entry depending on operation: add/remove */ 
        hndl->var.eclass_ctxt.extract_cfg_tbl[link][itr].valid = FALSE;
        hndl->var.eclass_ctxt.extract_cfg_tbl[link][itr].pkt_type = LAST_UTIL_GLOBAL_PKT_TYPE;
        hndl->var.eclass_ctxt.extract_cfg_tbl[link][itr].is_vlan = FALSE;
    }

    PMC_RETURN();
}/* eclass_pkt_extract_cfg */

/*******************************************************************************
* eclass_extract_cfg_idx_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns index in which extract configuration will be saved or will be retrieved.
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   enable                - enable/disable packet extraction
*   extract_cfg_ptr       - pointer to extraction configuration
*
* OUTPUTS:
*   *free_idx             - extract table index
*                           when minus 1 not index is found
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR eclass_extract_cfg_idx_get(eclass_handle_t            *hndl,
                                             UINT32                      link, 
                                             BOOL8                       enable,
                                             eclass_pkt_extract_cfg_t   *extract_cfg_ptr,
                                             INT32                      *free_idx)
{
    UINT32 pkt_type;
    UINT32                      idx;
    UINT32 first_idx;
    UINT32 last_idx;
    UINT32 nb_vlan_entries = 0;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ECLASS_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != extract_cfg_ptr,  ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(ECLASS_NUM_LINKS > link, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    
    pkt_type = (UINT32)extract_cfg_ptr->pkt_type;

        
    *free_idx = -1;
    /* index 0..ECLASS_SUPPORTED_PKT_TYPE-1: non vlan entries */
    /* index ECLASS_SUPPORTED_PKT_TYPE..ECLASS_NUM_SUPPORTED_PKT_PER_LINK: vlan entries */
    if (FALSE == extract_cfg_ptr->is_vlan)
    {
        first_idx = 0;
        last_idx = ECLASS_SUPPORTED_PKT_TYPE;     
    }
    else
    {        
        first_idx = ECLASS_SUPPORTED_PKT_TYPE;
        last_idx = ECLASS_NUM_SUPPORTED_PKT_PER_LINK;
    }
    
    /* run through extract table */
    for (idx = first_idx; idx < last_idx && PMC_SUCCESS == rc; idx++)
    {
        if (TRUE == enable &&
            FALSE ==  hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].valid)
        {
            *free_idx = idx;
        }
        
        if (TRUE == enable && 
            TRUE ==  hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].valid &&
            TRUE == extract_cfg_ptr->is_vlan &&
            TRUE == hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].is_vlan)
        {
            nb_vlan_entries += 1;
        }
        
        if (extract_cfg_ptr->pkt_type == hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].pkt_type)
        {   
            if (extract_cfg_ptr->is_vlan == hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].is_vlan) 
            {
                
                if (FALSE == enable && FALSE ==  hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].valid)
                {
                    if (FALSE ==  extract_cfg_ptr->is_vlan ||
                        (TRUE ==  extract_cfg_ptr->is_vlan &&
                         0 == PMC_MEMCMP(&extract_cfg_ptr->vlan_entry, &hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].vlan_entry, sizeof(util_global_vlan_cfg_t))))
                    {            
                        rc = ECLASS_ERR_EXTRACT_ALREADY_DISABLED;
                    }
                }
                else if (TRUE == enable && TRUE ==  hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].valid)
                {
                    if (FALSE ==  extract_cfg_ptr->is_vlan ||
                        (TRUE ==  extract_cfg_ptr->is_vlan &&
                         0 == PMC_MEMCMP(&extract_cfg_ptr->vlan_entry, &hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].vlan_entry, sizeof(util_global_vlan_cfg_t))))
                    {
                        rc = ECLASS_ERR_EXTRACT_ALREADY_ENABLED;
                    }
                }
                else if (FALSE == enable && TRUE ==  hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].valid)
                {
                    if (FALSE ==  extract_cfg_ptr->is_vlan ||
                        (TRUE ==  extract_cfg_ptr->is_vlan &&
                         0 == PMC_MEMCMP(&extract_cfg_ptr->vlan_entry, &hndl->var.eclass_ctxt.extract_cfg_tbl[link][idx].vlan_entry, sizeof(util_global_vlan_cfg_t))))
                    {
                        *free_idx = idx;
                    }
                }

            }
        }    
    }
  
    if (ECLASS_MAX_NUM_VLAN_ID_PER_LINK <= nb_vlan_entries)
    {
        rc = ECLASS_ERR_EXTRACT_VLAN_TABLE_FULL;
    }

    if (PMC_SUCCESS != rc)
    {
        *free_idx = -1;
    }
    
    PMC_RETURN(rc);      
} /* eclass_extract_cfg_idx_get */
                            
/*******************************************************************************
* eclass_pkt_extract_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns extract status for a given configuration.
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*   extract_cfg_ptr       - pointer to extraction configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when extraction is configured otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 eclass_pkt_extract_status_get(eclass_handle_t *hndl, 
                                           UINT32 link, 
                                           eclass_pkt_extract_cfg_t *extract_cfg_ptr)
{
    
    UINT32 i;
    UINT32 pkt_type;
    BOOL8 rc = FALSE;


    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ECLASS_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != extract_cfg_ptr,  ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(ECLASS_NUM_LINKS > link, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    
    pkt_type = (UINT32)extract_cfg_ptr->pkt_type;
    
    /* run through eclass extract configuration table and return TRUE if */
    /* given extraction config is found */
    for (i = 0; i < ECLASS_NUM_SUPPORTED_PKT_PER_LINK && FALSE == rc; i++)
    {             
        if (TRUE == hndl->var.eclass_ctxt.extract_cfg_tbl[link][i].valid &&
            extract_cfg_ptr->pkt_type == hndl->var.eclass_ctxt.extract_cfg_tbl[link][i].pkt_type && 
            extract_cfg_ptr->is_vlan == hndl->var.eclass_ctxt.extract_cfg_tbl[link][i].is_vlan)
        {
            rc = TRUE;
        }
    }

    PMC_RETURN(rc);
} /*eclass_pkt_extract_status_get*/


/*******************************************************************************
* eclass_pkt_extract_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns extract status for a given configuration.
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*
* OUTPUTS:
*   array_size_ptr        - size of extract_cfg_ptr array
*   array_used_size_ptr   - number of used elements in extract_cfg_array
*   extract_cfg_ptr       - pointer to extraction configuration
*
* RETURNS:
*   TRUE when extraction is configured otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void eclass_pkt_extract_cfg_get(eclass_handle_t *hndl, 
                                       UINT32 link, 
                                       UINT32 *array_size_ptr,
                                       UINT32 *array_used_size_ptr,
                                       eclass_pkt_extract_cfg_t *extract_cfg_ptr)
{
    
    UINT32 i;

    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ECLASS_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != extract_cfg_ptr,  ECLASS_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(ECLASS_NUM_LINKS > link, ECLASS_ERR_INVALID_PARAMETERS, 0, 0);

    
    *array_used_size_ptr = ECLASS_NUM_SUPPORTED_PKT_PER_LINK;
    *array_size_ptr = ECLASS_NUM_SUPPORTED_PKT_PER_LINK;

    /* run through eclass extract configuration table and return TRUE if */
    /* given extraction config is found */
    for (i = 0; i < ECLASS_NUM_SUPPORTED_PKT_PER_LINK; i++)
    {             
        if(FALSE == hndl->var.eclass_ctxt.extract_cfg_tbl[link][i].valid)
        {
            *array_used_size_ptr = *array_used_size_ptr - 1;
        }
        PMC_MEMCPY(&extract_cfg_ptr[i],
                   &hndl->var.eclass_ctxt.extract_cfg_tbl[link][i],
                   sizeof(eclass_pkt_extract_cfg_t));
    }

    PMC_RETURN();
} /*eclass_pkt_extract_cfg_get*/

/*******************************************************************************
* eclass_pkt_extract_all_cfg_disabled_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns TRUE when extraction is not configured.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when extraction is not configured otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 eclass_pkt_extract_all_cfg_disabled_get(eclass_handle_t *hndl)
{
    
    UINT32 i;
    UINT32 j;
    BOOL8 rc = TRUE;


    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ECLASS_ERR_NULL_HANDLE, 0, 0);
    
    for (i = 0; i < ECLASS_NUM_LINKS && TRUE == rc; i++)
    {     
        for (j = 0; j < ECLASS_NUM_SUPPORTED_PKT_PER_LINK && TRUE == rc; j++)
        {     
            if (TRUE == hndl->var.eclass_ctxt.extract_cfg_tbl[i][j].valid)
            {
                rc = FALSE;
            }
        }
    }

    PMC_RETURN(rc);
}/*eclass_pkt_extract_all_cfg_disabled_get*/

/*******************************************************************************
* eclass_pkt_extract_link_cfg_disabled_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns TRUE when no extraction is configured for a given link.
*
*
* INPUTS:
*   *hndl                 - pointer to ECLASS handle instance.
*   link                  - the link to be operated on.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when extraction is configured for a given link otherwise FALSE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 eclass_pkt_extract_link_cfg_disabled_get(eclass_handle_t *hndl,
                                                      UINT32 link)
{
    UINT32 j;
    BOOL8 rc = TRUE;


    PMC_ENTRY();
    PMC_ASSERT(NULL != hndl, ECLASS_ERR_NULL_HANDLE, 0, 0);
    
    for (j = 0; j < ECLASS_NUM_SUPPORTED_PKT_PER_LINK && TRUE == rc; j++)
    {     
        if (TRUE == hndl->var.eclass_ctxt.extract_cfg_tbl[link][j].valid)
        {
            rc = FALSE;
        }
    }
    
    PMC_RETURN(rc);
} /*eclass_pkt_extract_link_cfg_disabled_get*/

/*
** Private Functions
*/

/*
** End of file
*/
