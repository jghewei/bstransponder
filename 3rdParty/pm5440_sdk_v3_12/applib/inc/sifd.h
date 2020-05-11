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
#ifndef _SIFD_H
#define _SIFD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "pmc_sys.h"
#include "sifd_api.h"
#include "util_global.h"
#include "util_gen_db.h"



/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: sifd_ilkn_en_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum, defining the Interlaken block enabled state.
*
* ELEMENTS:
*   SIFD_ILKN_DISABLE               - Disable
*   SIFD_ILKN_ENABLE                - Enable
*
*******************************************************************************/
typedef enum {
    SIFD_ILKN_DISABLE                = 0,
    SIFD_ILKN_ENABLE                 = 1
} sifd_ilkn_en_t;


/*******************************************************************************
* ENUM: sifd_blk_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SERDES lane assignment.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    SIFD_BLK_ILKN1     = 0,   /* Interlaken 1 */
    SIFD_BLK_ILKN2     = 1,   /* Interlaken 2 */
    SIFD_BLK_LOTN      = 4,   /* LineOTN */
    SIFD_BLK_DSIS_CBRC = 5,   /* CBRC DSIS */
    SIFD_BLK_DSIS_OTN  = 6,   /* OTN DSIS */
    SIFD_BLK_DSIS_ENET = 7,   /* ENET DSIS */
    SIFD_LAST_BLK             /* out of range value */
} sifd_blk_type_t;

/*******************************************************************************
* ENUM: sifd_dsis_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum to define the subsystems which may be connected to DSIS block.
*
* ELEMENTS:
*       SIFD_DSIS_NONE                 - Init or default value.
*       SIFD_DSIS_CBRC                 - CBRC SS
*       SIFD_DSIS_OTN                  - OTN SS
*       SIFD_DSIS_ENET                 - ENET SS
*       SIFD_LAST_DSIS                 - out of range value
*
*******************************************************************************/
typedef enum {

    SIFD_DSIS_NONE     = 0,
    SIFD_DSIS_CBRC     = 1,
    SIFD_DSIS_OTN      = 2,
    SIFD_DSIS_ENET     = 3,
    SIFD_DSIS_LAST
} sifd_dsis_type_t;

/*******************************************************************************
* ENUM: sifd_fc_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying the flow control mode for
*   an Interlaken interface.
*
* ELEMENTS:
*   SIFD_FC_MODE_NONE         - No Flow Control Implemented
*   SIFD_FC_MODE_IN_BAND_16   - In-band Flow Control 16 indications per ICW/BCW
*   SIFD_FC_MODE_IN_BAND_24   - In-band Flow Control 24 indications per ICW/BCW
*   SIFD_FC_MODE_OUT_OF_BAND  - Out-of-band Flow Control
*
*******************************************************************************/
typedef enum
{
    SIFD_FC_MODE_NONE = 0,
    SIFD_FC_MODE_IN_BAND_16,
    SIFD_FC_MODE_IN_BAND_24,
    SIFD_FC_MODE_OUT_OF_BAND,
    LAST_SIFD_FC_MODE,
} sifd_fc_mode_t;

/*******************************************************************************
* ENUM:  sifd_ilkn_serdes_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying packet or segment mode transfers
*   from CPB and ODUK.
*
* ELEMENTS:
*       SIFD_SEGMENT_MODE              - segment mode
*       SIFD_PACKET_MODE               - packet mode
*
*******************************************************************************/
typedef enum {
    SIFD_SEGMENT_MODE                = 0,
    SIFD_PACKET_MODE                 = 1,
} sifd_mode_t;

/*******************************************************************************
* ENUM: sifd_oduk_hdr_insert_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying ODUK header format which can be
*   nothing, 2B, 4B or 6B (2B + 4B).  These headers will be inserted before
*   Interlaken transmission and extracted on reception.
*
* ELEMENTS:
*       SIFD_ODUK_NO_HEADER            - no header
*       SIFD_ODUK_2B_HEADER            - 2B header
*       SIFD_ODUK_4B_HEADER            - 4B header
*       SIFD_ODUK_6B_HEADER            - 6B (2B + 4B) header
*
*******************************************************************************/
typedef enum {
    SIFD_ODUK_NO_HEADER              = 0,
    SIFD_ODUK_2B_HEADER              = 1,
    SIFD_ODUK_4B_HEADER              = 2,
    SIFD_ODUK_6B_HEADER              = 3,
} sifd_oduk_hdr_insert_t;

/*******************************************************************************
* ENUM: sifd_oc_burst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying COPI/Interlaken burst size.
*   When the burst size is used for Interlaken configuration, it must be
*   incremented by 1.
*
* ELEMENTS:
*       SIFD_ODUK_CPB_BURST_128        - 128 bytes
*       SIFD_ODUK_CPB_BURST_192        - 192 bytes
*       SIFD_ODUK_CPB_BURST_256        - 256 bytes
*
*******************************************************************************/
typedef enum {
    SIFD_ODUK_CPB_BURST_128          = 0,
    SIFD_ODUK_CPB_BURST_192          = 1,
    SIFD_ODUK_CPB_BURST_256          = 2,
} sifd_oc_burst_t;

/*******************************************************************************
* ENUM: sifd_burst_short_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying the Interlaken short burst size.
*
* ELEMENTS:
*       SIFD_BURST_SHORT_32            - 32 bytes
*       SIFD_BURST_SHORT_64            - 64 bytes
*       LAST_SIFD_BURST_SHORT          - out of range value
*
*******************************************************************************/
typedef enum
{
    SIFD_BURST_SHORT_32 = 0,
    SIFD_BURST_SHORT_64,
    LAST_SIFD_BURST_SHORT,
} sifd_burst_short_t;

/*******************************************************************************
* ENUM: sifd_burst_min_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying the Interlaken min burst size.
*
* ELEMENTS:
*       SIFD_BURST_MIN_32            - 32 bytes
*       SIFD_BURST_MIN_64            - 64 bytes
*       SIFD_BURST_MIN_DISABLE       - No minimum burst size expected
*       LAST_SIFD_BURST_MIN          - out of range value
*
*******************************************************************************/
typedef enum
{
    SIFD_BURST_MIN_32 = 0,
    SIFD_BURST_MIN_64 = 1,
    SIFD_BURST_MIN_DISABLE,
    LAST_SIFD_BURST_MIN,
} sifd_burst_min_t;

/*******************************************************************************
* ENUM: sifd_ilkn_mf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying the Interlaken meta-frame period
*   (in 1024, 2048 or 4096 word increments).
*
* ELEMENTS:
*       SIFD_ILKN_MF_1024              - 1024 words (8KB interval)
*       SIFD_ILKN_MF_2048              - 2048 words (16KB interval)
*       SIFD_ILKN_MF_4096              - 4096 words (32KB interval)
*       LAST_SIFD_ILKN_MF              - out of range value
*
*******************************************************************************/
typedef enum
{
    SIFD_ILKN_MF_1024 = 1024,
    SIFD_ILKN_MF_2048 = 2048,
    SIFD_ILKN_MF_4096 = 4096,
    LAST_SIFD_ILKN_MF,
} sifd_ilkn_mf_t;

/*******************************************************************************
* ENUM:  sifd_ilkn_serdes_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for specifying SERDES line rate.
*
* ELEMENTS:
*       SIFD_ILKN_SERDES_RATE_3G125    - 3.125 Gbps
*       SIFD_ILKN_SERDES_RATE_6G25     - 6.25 Gbps
*       SIFD_ILKN_SERDES_RATE_10G3125  - 10.3125 Gbps
*       SIFD_ILKN_SERDES_RATE_11G5     - 11.5 Gbps
*       SIFD_ILKN_SERDES_RATE_12G5     - 12.5 Gbps
*       LAST_SIFD_ILKN_SERDES_RATE     - out of range value
*
*******************************************************************************/
typedef enum
{
    SIFD_ILKN_SERDES_RATE_3G125 = 0,
    SIFD_ILKN_SERDES_RATE_6G25,
    SIFD_ILKN_SERDES_RATE_10G3125,
    SIFD_ILKN_SERDES_RATE_11G5,
    SIFD_ILKN_SERDES_RATE_12G5,
    LAST_SIFD_ILKN_SERDES_RATE,
} sifd_ilkn_serdes_rate_t;

/*******************************************************************************
* ENUM: sifd_ilkn_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum, defining the Interlaken block instance.
*
* ELEMENTS:
*   SIFD_COPI_ILKN1     - Interlaken 1 Instance
*   SIFD_COPI_ILKN2     - Interlaken 2 Instance
*
*******************************************************************************/
typedef enum {
    SIFD_COPI_ILKN1     = 0,
    SIFD_COPI_ILKN2     = 1,

    LAST_SIFD_ILKN
} sifd_ilkn_inst_t;


/*******************************************************************************
* ENUM: sifd_loopback_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SIFD loopbacks.
*
* ELEMENTS:
*       SIFD_DISABLE_LOOPBACK                    - Disable all loopbacks
*       SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK     - Data from CPB loops back at the ILKN
*                                                  block inside the COPI block. Inside
*                                                  SIFD this is called the TX to RX loopback.
*       SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK - Data from ODU loops back at the ILKN block
*                                                  inside the COPI block. Inside SIFD this is
*                                                  called the TX to RX loopback.
*       SIFD_COPI_FACILITY_LOOPBACK              - Data from the System SERDES loops back at
*                                                  the ILKN block inside the COPI block. Inside
*                                                  SIFD this is called RX to TX loopback.
*
*       Note: The CPB/ODU external loopback is not specified in this
*       structure.
*
*******************************************************************************/

typedef enum {
    SIFD_DISABLE_LOOPBACK = 0,
    SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK,
    SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK,
    SIFD_COPI_FACILITY_LOOPBACK,

} sifd_loopback_cfg_t;

/*******************************************************************************
* ENUM: sifd_sig_src_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enum, defining the subsystem that supplies C_CKCTL for ACB lane.
*
* ELEMENTS:
*   SIFD_SRC_NONE                   - No subsystem
*   SIFD_SRC_CBRC                   - CBRC
*   SIFD_SRC_ENET                   - ENET
*   SIFD_SRC_OTN_IPM                - OTN Ingress Packet Monitor and CBRC
*   SIFD_SRC_OTN_LPM                - OTN LINE side Egress Packet Monitor
*   SIFD_SRC_OTN_EPM                - OTN SYS side Egress Packet Monitor
*   SIFD_SRC_DIG_M1                 - DIG M1
*   SIFD_SRC_DIG_IPME               - OTN Ingress Packet Monitor and ENET
*   SIFD_LAST_SRC                   - Last entry
*
*******************************************************************************/
typedef enum {

    SIFD_SRC_NONE     = 0,
    SIFD_SRC_CBRC     = 1,
    SIFD_SRC_ENET     = 2,
    SIFD_SRC_OTN_IPM  = 3,
    SIFD_SRC_OTN_LPM  = 4,
    SIFD_SRC_OTN_EPM  = 5,
    SIFD_SRC_DIG_M1   = 6,
    SIFD_SRC_DIG_IPME = 7,
    SIFD_LAST_SRC

} sifd_sig_src_type_t;


/*
** Constants
*/
#define SIFD_24_LANE_MASK                0xFFFFFF

#define SIFD_TX_CPB_CH_FC                0x200
#define SIFD_TX_ODUK_LINK_FC             0x100
#define SIFD_TX_ODUK_CH_FC               0x080
#define SIFD_TX_IB                       0x040
#define SIFD_TX_OOB                      0x020
#define SIFD_RX_LINK_FC                  0x010
#define SIFD_RX_CPB_CH_FC                0x008
#define SIFD_RX_ODUK_CH_FC               0x004
#define SIFD_RX_IB                       0x002
#define SIFD_RX_OOB                      0x001

#define SIFD_MAX_ILKN_LANES              24

#define SIFD_ILKN_MAX_CAL_ENTRIES        300
#define SIFD_ILKN_MIN_CAL_ENTRIES        1
#define SIFD_ILKN_NULL_CAL_ENTRY         255

#define SIFD_ILKN_META_FRAME_PERIOD      2048


/*
** Constants for valid mask comparisons
*/

#define SIFD_MASK_LANE_01 0x0000000000000001
#define SIFD_MASK_LANE_02 0x0000000000000002
#define SIFD_MASK_LANE_03 0x0000000000000004
#define SIFD_MASK_LANE_04 0x0000000000000008
#define SIFD_MASK_LANE_05 0x0000000000000010
#define SIFD_MASK_LANE_06 0x0000000000000020
#define SIFD_MASK_LANE_07 0x0000000000000040
#define SIFD_MASK_LANE_08 0x0000000000000080
#define SIFD_MASK_LANE_09 0x0000000000000100
#define SIFD_MASK_LANE_10 0x0000000000000200
#define SIFD_MASK_LANE_11 0x0000000000000400
#define SIFD_MASK_LANE_12 0x0000000000000800
#define SIFD_MASK_LANE_13 0x0000000000001000
#define SIFD_MASK_LANE_14 0x0000000000002000
#define SIFD_MASK_LANE_15 0x0000000000004000
#define SIFD_MASK_LANE_16 0x0000000000008000
#define SIFD_MASK_LANE_17 0x0000000000010000
#define SIFD_MASK_LANE_18 0x0000000000020000
#define SIFD_MASK_LANE_19 0x0000000000040000
#define SIFD_MASK_LANE_20 0x0000000000080000
#define SIFD_MASK_LANE_21 0x0000000000100000
#define SIFD_MASK_LANE_22 0x0000000000200000
#define SIFD_MASK_LANE_23 0x0000000000400000
#define SIFD_MASK_LANE_24 0x0000000000800000
#define SIFD_MASK_LANE_25 0x0000000001000000
#define SIFD_MASK_LANE_26 0x0000000002000000
#define SIFD_MASK_LANE_27 0x0000000004000000
#define SIFD_MASK_LANE_28 0x0000000008000000
#define SIFD_MASK_LANE_29 0x0000000010000000
#define SIFD_MASK_LANE_30 0x0000000020000000
#define SIFD_MASK_LANE_31 0x0000000040000000
#define SIFD_MASK_LANE_32 0x0000000080000000
#define SIFD_MASK_LANE_33 0x0000000100000000
#define SIFD_MASK_LANE_34 0x0000000200000000
#define SIFD_MASK_LANE_35 0x0000000400000000
#define SIFD_MASK_LANE_36 0x0000000800000000
#define SIFD_MASK_LANE_37 0x0000001000000000
#define SIFD_MASK_LANE_38 0x0000002000000000
#define SIFD_MASK_LANE_39 0x0000004000000000
#define SIFD_MASK_LANE_40 0x0000008000000000

#define SIFD_MASK_LOW_02_LANES (SIFD_MASK_LANE_01 | SIFD_MASK_LANE_02)
#define SIFD_MASK_LOW_04_LANES SIFD_MASK_LOW_02_LANES | \
                               (SIFD_MASK_LANE_03 | SIFD_MASK_LANE_04)
#define SIFD_MASK_LOW_06_LANES SIFD_MASK_LOW_04_LANES | \
                               (SIFD_MASK_LANE_05 | SIFD_MASK_LANE_06)
#define SIFD_MASK_LOW_08_LANES SIFD_MASK_LOW_06_LANES | \
                               (SIFD_MASK_LANE_07 | SIFD_MASK_LANE_08)
#define SIFD_MASK_LOW_10_LANES SIFD_MASK_LOW_08_LANES | \
                               (SIFD_MASK_LANE_09 | SIFD_MASK_LANE_10)
#define SIFD_MASK_LOW_12_LANES SIFD_MASK_LOW_10_LANES | \
                               (SIFD_MASK_LANE_11 | SIFD_MASK_LANE_12)
#define SIFD_MASK_LOW_14_LANES SIFD_MASK_LOW_12_LANES | \
                               (SIFD_MASK_LANE_13 | SIFD_MASK_LANE_14)
#define SIFD_MASK_LOW_16_LANES SIFD_MASK_LOW_14_LANES | \
                               (SIFD_MASK_LANE_15 | SIFD_MASK_LANE_16)
#define SIFD_MASK_LOW_18_LANES SIFD_MASK_LOW_16_LANES | \
                               (SIFD_MASK_LANE_17 | SIFD_MASK_LANE_18)
#define SIFD_MASK_LOW_20_LANES SIFD_MASK_LOW_18_LANES | \
                               (SIFD_MASK_LANE_19 | SIFD_MASK_LANE_20)
#define SIFD_MASK_LOW_22_LANES SIFD_MASK_LOW_20_LANES | \
                               (SIFD_MASK_LANE_21 | SIFD_MASK_LANE_22)
#define SIFD_MASK_LOW_24_LANES SIFD_MASK_LOW_22_LANES | \
                               (SIFD_MASK_LANE_23 | SIFD_MASK_LANE_24)
#define SIFD_MASK_LOW_26_LANES SIFD_MASK_LOW_24_LANES | \
                               (SIFD_MASK_LANE_25 | SIFD_MASK_LANE_26)
#define SIFD_MASK_LOW_28_LANES SIFD_MASK_LOW_26_LANES | \
                               (SIFD_MASK_LANE_27 | SIFD_MASK_LANE_28)
#define SIFD_MASK_LOW_30_LANES SIFD_MASK_LOW_28_LANES | \
                               (SIFD_MASK_LANE_29 | SIFD_MASK_LANE_30)
#define SIFD_MASK_LOW_32_LANES SIFD_MASK_LOW_30_LANES | \
                               (SIFD_MASK_LANE_31 | SIFD_MASK_LANE_32)
#define SIFD_MASK_LOW_34_LANES SIFD_MASK_LOW_32_LANES | \
                               (SIFD_MASK_LANE_33 | SIFD_MASK_LANE_34)
#define SIFD_MASK_LOW_36_LANES SIFD_MASK_LOW_34_LANES | \
                               (SIFD_MASK_LANE_35 | SIFD_MASK_LANE_36)
#define SIFD_MASK_LOW_38_LANES SIFD_MASK_LOW_36_LANES | \
                               (SIFD_MASK_LANE_37 | SIFD_MASK_LANE_38)
#define SIFD_MASK_LOW_40_LANES SIFD_MASK_LOW_38_LANES | \
                               (SIFD_MASK_LANE_39 | SIFD_MASK_LANE_40)

#define SIFD_MASK_HIGH_40_02_LANES (SIFD_MASK_LANE_39 | SIFD_MASK_LANE_40 )
#define SIFD_MASK_HIGH_40_04_LANES SIFD_MASK_HIGH_40_02_LANES | \
                                   (SIFD_MASK_LANE_37 | SIFD_MASK_LANE_38)
#define SIFD_MASK_HIGH_40_06_LANES SIFD_MASK_HIGH_40_04_LANES | \
                                   (SIFD_MASK_LANE_35 | SIFD_MASK_LANE_36)
#define SIFD_MASK_HIGH_40_08_LANES SIFD_MASK_HIGH_40_06_LANES | \
                                   (SIFD_MASK_LANE_33 | SIFD_MASK_LANE_34)
#define SIFD_MASK_HIGH_40_10_LANES SIFD_MASK_HIGH_40_08_LANES | \
                                   (SIFD_MASK_LANE_31 | SIFD_MASK_LANE_32)
#define SIFD_MASK_HIGH_40_12_LANES SIFD_MASK_HIGH_40_10_LANES | \
                                   (SIFD_MASK_LANE_29 | SIFD_MASK_LANE_30)
#define SIFD_MASK_HIGH_40_14_LANES SIFD_MASK_HIGH_40_12_LANES | \
                                   (SIFD_MASK_LANE_27 | SIFD_MASK_LANE_28)
#define SIFD_MASK_HIGH_40_16_LANES SIFD_MASK_HIGH_40_14_LANES | \
                                   (SIFD_MASK_LANE_25 | SIFD_MASK_LANE_26)
#define SIFD_MASK_HIGH_40_18_LANES SIFD_MASK_HIGH_40_16_LANES | \
                                   (SIFD_MASK_LANE_23 | SIFD_MASK_LANE_24)
#define SIFD_MASK_HIGH_40_20_LANES SIFD_MASK_HIGH_40_18_LANES | \
                                   (SIFD_MASK_LANE_21 | SIFD_MASK_LANE_22)


#define SIFD_MASK_HIGH_24_02_LANES (SIFD_MASK_LANE_23 | SIFD_MASK_LANE_24 )
#define SIFD_MASK_HIGH_24_04_LANES SIFD_MASK_HIGH_24_02_LANES | \
                                   (SIFD_MASK_LANE_21 | SIFD_MASK_LANE_22)
#define SIFD_MASK_HIGH_24_06_LANES SIFD_MASK_HIGH_24_04_LANES | \
                                   (SIFD_MASK_LANE_19 | SIFD_MASK_LANE_20)
#define SIFD_MASK_HIGH_24_08_LANES SIFD_MASK_HIGH_24_06_LANES | \
                                   (SIFD_MASK_LANE_17 | SIFD_MASK_LANE_18)
#define SIFD_MASK_HIGH_24_10_LANES SIFD_MASK_HIGH_24_08_LANES | \
                                   (SIFD_MASK_LANE_15 | SIFD_MASK_LANE_16)
#define SIFD_MASK_HIGH_24_12_LANES SIFD_MASK_HIGH_24_10_LANES | \
                                   (SIFD_MASK_LANE_13 | SIFD_MASK_LANE_14)
#define SIFD_MASK_HIGH_24_14_LANES SIFD_MASK_HIGH_24_12_LANES | \
                                   (SIFD_MASK_LANE_11 | SIFD_MASK_LANE_12)
#define SIFD_MASK_HIGH_24_16_LANES SIFD_MASK_HIGH_24_14_LANES | \
                                   (SIFD_MASK_LANE_09 | SIFD_MASK_LANE_10)
#define SIFD_MASK_HIGH_24_18_LANES SIFD_MASK_HIGH_24_16_LANES | \
                                   (SIFD_MASK_LANE_07 | SIFD_MASK_LANE_08)
#define SIFD_MASK_HIGH_24_20_LANES SIFD_MASK_HIGH_24_18_LANES | \
                                   (SIFD_MASK_LANE_05 | SIFD_MASK_LANE_06)
#define SIFD_MASK_HIGH_24_22_LANES SIFD_MASK_HIGH_24_20_LANES | \
                                   (SIFD_MASK_LANE_03 | SIFD_MASK_LANE_04)
#define SIFD_MASK_HIGH_24_24_LANES SIFD_MASK_HIGH_24_22_LANES | \
                                   (SIFD_MASK_LANE_01 | SIFD_MASK_LANE_02)

#define SIFD_MASK_HIGH_16_02_LANES (SIFD_MASK_LANE_15 | SIFD_MASK_LANE_16 )
#define SIFD_MASK_HIGH_16_04_LANES SIFD_MASK_HIGH_16_02_LANES | \
                                   (SIFD_MASK_LANE_13 | SIFD_MASK_LANE_14)
#define SIFD_MASK_HIGH_16_06_LANES SIFD_MASK_HIGH_16_04_LANES | \
                                   (SIFD_MASK_LANE_11 | SIFD_MASK_LANE_12)
#define SIFD_MASK_HIGH_16_08_LANES SIFD_MASK_HIGH_16_06_LANES | \
                                   (SIFD_MASK_LANE_09 | SIFD_MASK_LANE_10)
#define SIFD_MASK_HIGH_16_10_LANES SIFD_MASK_HIGH_16_08_LANES | \
                                   (SIFD_MASK_LANE_07 | SIFD_MASK_LANE_08)
#define SIFD_MASK_HIGH_16_12_LANES SIFD_MASK_HIGH_16_10_LANES | \
                                   (SIFD_MASK_LANE_05 | SIFD_MASK_LANE_06)
#define SIFD_MASK_HIGH_16_14_LANES SIFD_MASK_HIGH_16_12_LANES | \
                                   (SIFD_MASK_LANE_03 | SIFD_MASK_LANE_04)
#define SIFD_MASK_HIGH_16_16_LANES SIFD_MASK_HIGH_16_14_LANES | \
                                   (SIFD_MASK_LANE_01 | SIFD_MASK_LANE_02)

#define SIFD_CLK_DISABLE                    0
#define SIFD_CBRC_CLK_ENABLE                1
#define SIFD_OTN_CLK_ENABLE                 2
#define SIFD_ENET_CLK_ENABLE                4

/*
** Macro Definitions
*/

/* SIFD_INT_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) */
#define SIFD_INT_TABLE_DEFINE() \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, FIFO_OVER           , fifo_over_i           ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, FIFO_UNDER          , fifo_under_i          ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, OOB_FC_ERR          , oob_fc_err_i          ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, TX_STATUS_MSG_CHANGE, tx_status_msg_change_i); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, TX_FC_CNT_FILL      , tx_fc_cnt_fill_i      ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, TX_CHAN_CNT_FILL    , tx_chan_cnt_fill_i    ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, CTLBIT_ERR          , ctlbit_err_i          ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, MF_ERR              , mf_err_i              ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BADCTL_ERR          , badctl_err_i          ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, SCRAM_ERR           , scram_err_i           ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, CRC32_ERR           , crc32_err_i           ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, CDR_ERR             , cdr_err_i             ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, WRD_SYNC_CHANGE     , wrd_sync_change_i     ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, ALIGN_CHANGE        , align_change_i        ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, ALIGN_ERR           , align_err_i           ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BURST_ERR           , burst_err_i           ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BMIN_ERR            , bmin_err_i            ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BMAX_ERR            , bmax_err_i            ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, CRC24_ERR           , crc24_err_i           ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, IB_FC_ERR           , ib_fc_err_i           ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, RX_STATUS_MSG_CHANGE, rx_status_msg_change_i); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BADCTL_CNT_FILL     , badctl_cnt_fill_i     ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, CRC32_CNT_FILL      , crc32_cnt_fill_i      ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, CDR_CNT_FILL        , cdr_cnt_fill_i        ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, WRD_SYNC_CNT_FILL   , wrd_sync_cnt_fill_i   ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, ALIGN_FAIL_CNT_FILL , align_fail_cnt_fill_i ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, ALIGN_CNT_FILL      , align_cnt_fill_i      ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BURST_CNT_FILL      , burst_cnt_fill_i      ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BMIN_CNT_FILL       , bmin_cnt_fill_i       ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, BMAX_CNT_FILL       , bmax_cnt_fill_i       ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, CRC24_CNT_FILL      , crc24_cnt_fill_i      ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, RX_FC_CNT_FILL      , rx_fc_cnt_fill_i      ); \
    SIFD_INT_RANGE(sifd_handle, ilkn, field, RX_CHAN_CNT_FILL    , rx_chan_cnt_fill_i    ); \
    SIFD_INT(sifd_handle, copi, CPB_TX_BUF_UN_INT,              cpb_tx_buf_un_int_i0             ); \
    SIFD_INT(sifd_handle, copi, CPB_TX_BUF_ALIGN_INT,           cpb_tx_buf_align_int_i0          ); \
    SIFD_INT(sifd_handle, copi, ODUK_TX_BUF_UN_INT,             oduk_tx_buf_un_int_i0            ); \
    SIFD_INT(sifd_handle, copi, ODUK_TX_BUF_ALIGN_INT,          oduk_tx_buf_align_int_i0         ); \
    SIFD_INT(sifd_handle, copi, RX_BUF_UN_INT,                  rx_buf_un_int_i0                 ); \
    SIFD_INT(sifd_handle, copi, RX_BUF_ALIGN_INT,               rx_buf_align_int_i0              ); \
    SIFD_INT_ARRAY(sifd_handle, copi, SIFD_NUM_ILKN_DIR_MAX,   TX_SOB_ERR_INT,             tx_sob_err_int_i0                ); \
    SIFD_INT_ARRAY(sifd_handle, copi, SIFD_NUM_ILKN_DIR_MAX,   TX_BURST_LEN_ERR_INT,       tx_burst_len_err_int_i0          ); \
    SIFD_INT_ARRAY(sifd_handle, copi, SIFD_NUM_ILKN_DIR_MAX,   TX_BURST_DROP_ERR_INT,      tx_burst_drop_err_int_i0         ); \
    SIFD_INT_ARRAY(sifd_handle, copi, SIFD_NUM_ILKN_DIR_MAX,   TX_PKT_CH_CHANGE_ERR_INT,   tx_pkt_ch_change_err_int_i0      ); \
    SIFD_INT_ARRAY(sifd_handle, copi, SIFD_NUM_ILKN_DIR_MAX,   TX_EOB_ERR_INT,             tx_eob_err_int_i0                ); \
    SIFD_INT_ARRAY(sifd_handle, copi, SIFD_NUM_ILKN_DIR_MAX,   TX_SOP_ERR_INT,             tx_sop_err_int_i0                ); \
    SIFD_INT_ARRAY(sifd_handle, copi, SIFD_NUM_ILKN_DIR_MAX,   TX_EOP_ERR_INT,             tx_eop_err_int_i0                ); \
    SIFD_INT(sifd_handle, copi, RX_SOB_ERR_INT,                 rx_sob_err_int_i0                ); \
    SIFD_INT(sifd_handle, copi, RX_BURST_LEN_ERR_INT,           rx_burst_len_err_int_i0          ); \
    SIFD_INT(sifd_handle, copi, RX_BURST_DROP_ERR_INT,          rx_burst_drop_err_int_i0         ); \
    SIFD_INT(sifd_handle, copi, RX_PKT_CH_CHANGE_ERR_INT,       rx_pkt_ch_change_err_int_i0      ); \
    SIFD_INT(sifd_handle, copi, RX_EOB_ERR_INT,                 rx_eob_err_int_i0                ); \
    SIFD_INT(sifd_handle, copi, RX_SOP_ERR_INT,                 rx_sop_err_int_i0                ); \
    SIFD_INT(sifd_handle, copi, RX_EOP_ERR_INT,                 rx_eop_err_int_i0                ); \
    SIFD_INT(sifd_handle, copi, RX_IL_ERR_ALL_INT,              rx_il_err_all_int_i0             ); \
    SIFD_INT(sifd_handle, copi, RX_IL_ALIGN_INT,                rx_il_align_int_i0               ); \
    SIFD_INT(sifd_handle, copi, RX_ODUK_H2B_CAM_ERR_INT,        rx_oduk_h2b_cam_err_int_i0       ); \
    SIFD_INT(sifd_handle, copi, RX_ODUK_H2B_CAM_MMATCH_ERR_INT, rx_oduk_h2b_cam_mmatch_err_int_i0); \
    SIFD_INT(sifd_handle, copi, RX_ODUK_H2B_CMPR_ERR_INT,       rx_oduk_h2b_cmpr_err_int_i0      ); \
    SIFD_INT(sifd_handle, copi, RX_ODUK_H4B_CMPR_ERR_INT,       rx_oduk_h4b_cmpr_err_int_i0      ); \


#define SIFD_STATUS_TABLE_DEFINE() \
    SIFD_STATUS(sifd_handle, ilkn, RX_MF_ERR,                   rx_mf_err);                  \
    SIFD_STATUS(sifd_handle, ilkn, RX_WRD_SYNC_LANE,            rx_wrd_sync_lane);           \
    SIFD_STATUS(sifd_handle, ilkn, RX_WRD_SYNC_CNT_FILL_LANE,   rx_wrd_sync_cnt_fill_lane);  \
    SIFD_STATUS(sifd_handle, ilkn, RX_CRC32_ERR,                rx_crc32_err);               \
    SIFD_STATUS(sifd_handle, ilkn, RX_CRC32_CNT_FILL_LANE,      rc_crc32_cnt_fill_lane);     \
    SIFD_STATUS(sifd_handle, ilkn, RX_BADCTL_ERR,               rx_badctl_err);              \
    SIFD_STATUS(sifd_handle, ilkn, RX_SCRAM_ERR,                rx_scram_err);               \
    SIFD_STATUS(sifd_handle, ilkn, RX_CDR_ERR,                  rx_cdr_err);                 \
    SIFD_STATUS(sifd_handle, ilkn, RX_CRD_CNT_FILL_LANE,        rx_crd_cnt_fill_lane);       \
    SIFD_STATUS(sifd_handle, ilkn, RX_WRD_SYNC_CHG,             rx_wrd_sync_chg);            \
    SIFD_STATUS(sifd_handle, ilkn, RX_ALIGN_GOOD,               rx_align_good);              \
    SIFD_STATUS(sifd_handle, ilkn, RX_ALIGN_ERR,                rx_align_err);               \
    SIFD_STATUS(sifd_handle, ilkn, CURRENT,                     current);                    \
    SIFD_STATUS(sifd_handle, ilkn, VALID,                       valid);                      \
    SIFD_STATUS(sifd_handle, ilkn, SYNC,                        sync);                       \
    SIFD_STATUS(sifd_handle, ilkn, SIF,                         sif);                        \
    SIFD_STATUS(sifd_handle, ilkn, TX_SL,                       tx_sl);                      \
    SIFD_STATUS(sifd_handle, ilkn, RX_SL,                       rx_sl);                      \
    SIFD_STATUS(sifd_handle, ilkn, RX_STATUS_MSG_VALID_ALL,     rx_status_msg_valid_all);    \
    SIFD_STATUS(sifd_handle, ilkn, RX_STATUS_MSG_VALID_ANY,     rx_status_msg_valid_any);    \
    SIFD_STATUS(sifd_handle, ilkn, RX_STATUS_MSG_SYNC,          rx_status_msg_sync);         \
    SIFD_STATUS(sifd_handle, ilkn, SIF_ALL,                     sif_all);                    \
    SIFD_STATUS(sifd_handle, ilkn, SIF_ANY,                     sif_any);                    \



#define BASE_ADDR_TX_GP 0x00943804
#define BASE_ADDR_RX_GP 0x00943894
#define BASE_ADDR_TX_TP 0x00943854
#define BASE_ADDR_RX_TP 0x009438E4

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: sifd_ilkn_calendar_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the Interlaken flow control calendar.  There is a calendar
*   for each direction of Interlaken transmission.  The flow control calendar written
*   into the COPI registers is used for RX flow control (for TX direction data).
*
*   Each calendar can contain up to 300 entries (defined by fc_cal_length).  Each
*   entry corresponds to a flow control bit that is passed either in-band or out-of-band
*   on the Interlaken interface.
*
*   Each entry can be one of:
*
*        SIFD_ILKN_NULL_CAL_ENTRY           -- calendar entry is unused
*        SIFD_ILKN_LINK_FLOW_CONTROL        -- calendar entry is passing flow control
*                                              for the link
*        channel number                     -- CPB or ODUKSW DPI channel ID that relates to
*                                              this flow control calendar entry.
*                                              0 - 149 is the CPB DPI channel ID
*                                              150 - 253 is the ODUKSW DPI channel ID
*                                              offset by 150 (ODUKSW DPI channel ID =
*                                              BCW channel - 150)

*
*   Channels can represent arbitrary bandwidth from 1.25G to 100G.  The speed of flow
*   control response (i.e reacting to either XOFF or XON) is controlled by how the
*   calendar entries are populated.  Specifically, how many calendar entries have the
*   same channel number and the distribution of these entries over the complete calendar.
*   For example, if you were using in-band flow control with 16 flow control bits
*   per ICW/BCW and 300 calendar entries (which requires 18 ICW/BCW to transfer):
*
*        link flow control -- use 1 entry every ICW/BCW
*        100G channel      -- use 1 entry every ICW/BCW  (consumes all the bandwidth)
*        40G channel       -- use 1 entry every 3 ICW/BCW  (consumes 1/3 of the bandwidth)
*        10G channel       -- use 1 entry every 12 ICW/BCW  (consumes 1/12 of the bandwidth)
*        1.25G (ODU0) chan -- use 1 entry every 18 ICW/BCW  (consumes 1/96 of the bandwidth)
*
*   These are example usages.  In a real system LMAX delay would provide further criteria
*   for populating the calendar (e.g. how frequently the channel is reported).
*
*   There are two other modes of flow control:
*
*        -- in-band with 24 flow control bits per ICW/BCW (300 calendar entries require
*           13 ICW/BCW)
*        -- out-of-band flow control is sent as a repetitive frame (length fc_cal_length)
*           using a 100Mhz clock (300 calendar entries would be sent every 3.01 useconds)
*
*   For in-band flow control, if the calendar length is not a modulo of 16 or 24,
*   then the excess flow control bits (to complete the modulo) are unused (set to XOFF).
*
*   When the calendar length (fc_cal_length) is less than 300, the unused entries should
*   be populated with DIGI_SIFD_ILKN_NULL_CAL_ENTRY (set to XOFF).
*
* ELEMENTS:
*       entry        - array of 300 calendar entries
*
*******************************************************************************/
typedef struct sifd_ilkn_calendar_t
{
    UINT8 entry[SIFD_ILKN_MAX_CAL_ENTRIES];

} sifd_ilkn_calendar_t;


/*******************************************************************************
* STRUCTURE: sifd_blk_to_srds_msk_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure describes the lanes used across the SIFD block.  It contains
*   three bitmasks - one for the E1 lanes, one for the N1 lanes, and one for the
*   N2 lanes.  Each mask represents 24 lanes where lane 0 is the LSB.
*
* ELEMENTS:
*       blk_to_E1_enable_mask    - E1 lanes
*       blk_to_N1_enable_mask    - N1 lanes
*       blk_to_N2_enable_mask    - N2 lanes
*
*******************************************************************************/
typedef struct {

    UINT32 blk_to_E1_enable_mask;
    UINT32 blk_to_N1_enable_mask;
    UINT32 blk_to_N2_enable_mask;
} sifd_blk_to_srds_msk_cfg_t;

/*******************************************************************************
* STRUCTURE: sifd_dlos_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure describes how the DLOS signal across the SIFD block is to
*   be configured for the 24 SERDES lanes.
*
* ELEMENTS:
*       dlos_inv                 - DLOS inversion (1 to invert, 0 for no invert)
*       dlos_en                  - DLOS lane enable (1 to enable, 0 to disable)
*
*******************************************************************************/
typedef struct {

    UINT32 dlos_inv;
    UINT32 dlos_en;

} sifd_dlos_cfg_t;

/*******************************************************************************
* STRUCTURE: sifd_dsis_dlos_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   LOS context
*
* ELEMENTS:
*   dlos_base_cfg - See sifd_dlos_cfg_t
*   pin_los_inv   - Internal inversion of LOS signal from internal Serdes to DSIS
*   pin_los_en    - Enable control for LOS pin
*   los_pol_inv   - Polarity of LOS pin logic
*
*******************************************************************************/
typedef struct {

    sifd_dlos_cfg_t dlos_base_cfg;
    UINT32 pin_los_inv;
    UINT32 pin_los_en;
    UINT32 los_pol_inv;

} sifd_dsis_dlos_cfg_t;

/*******************************************************************************
* STRUCTURE: sifd_ilkn_lane_remap_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining SERDES lane remapping
*
* ELEMENTS:
*       lane                     - array containing remapped lane numbers
*
*******************************************************************************/
typedef struct {
    UINT8 lane[SIFD_MAX_ILKN_LANES];
} sifd_ilkn_lane_remap_cfg_t;

/*
 * MOVED THESE OVER FROM sifd_api.h
 */

/*******************************************************************************
* STRUCTURE: sifd_ilkn_rl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Interlaken TX rate limit
*
* ELEMENTS:
*       rate_limit               - rate limit in 1 Gb/s increments (from 0 to 100)\n
*                                    0 -- disable TX rate limit\n
*                                    1 to 100 -- enable TX rate limit to value in Gb/s
*       burst_limit              - maxiumum quantity of data that may be burst across
*                                  the Interlaken interface before invoking the rate
*                                  limiting logic (in bytes)
*       byte_granularity         - number of credits that are to be subtracted from
*                                  the token bucket when an EOP is encountered.\n
*                                    0: 1 byte\n
*                                    1: 2 bytes\n
*                                    2: 4 bytes\n
*                                    3: 8 bytes\n
*                                    4: 16 bytes\n
*                                    5: 32 bytes
*
*******************************************************************************/
typedef struct {
    UINT32 rate_limit;
    UINT32 burst_limit;
    UINT32 byte_granularity;
} sifd_ilkn_rl_t;

/*******************************************************************************
* STRUCTURE: sifd_ilkn_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining COPI/Interlaken configuration.
*
* ELEMENTS:
*       rate                     - SERDES line rate
*       lane_mask                - SERDES lanes enabled for Interlaken interface
*                                  (24 lanes where lane 0 is LSB)
*                                  1 enables lane and 0 disables lane)
*       lane_tx_order            - Interlaken SERDES TX lane remapping
*       lane_rx_order            - Interlaken SERDES RX lane remapping
*       burst                    - COPI/Interlaken burst size
*       burst_min                - Interlaken minimal burst size
*       burst_short              - Interlaken short burst size
*       metaframe                - Interlaken meta-frame period
*       tx_rate_limit            - Interlaken TX rate limit
*       oduksw_intf_en           - ODUK enable (TRUE is enabled & FALSE is disabled)
*       oduksw_tx_xfer_mode      - ODUK transfer mode in the tx direction (data
*                                  flowing from ODUKSW to SIFD). There are two possible
*                                  modes: packet or segment
*       oduksw_header            - ODUK header insertion/extraction (2B, 4B or 6B)
*       oduksw_2b_header_mode    - intrepretation of received ODUK 2B header
*       cpb_intf_en              - CPB enable (TRUE is enabled & FALSE is disabled)
*       cpb_tx_xfer_mode         - CPB transfer mode in the tx direction (data
*                                  flowing from CPB to SIFD). There are two possible
*                                  modes: packet or segment
*       rx_xfer_mode             - CPB/ODUKSW transfer mode in the rx direction (data
*                                  flowing from SIFD to CPB/ODUKSW). There are two
*                                  possible modes: packet or segment
*       fc_mode                  - flow control mode (in-band, out-of-band)
*       fc_cal_length            - flow control calendar length (up to 300 entries)
*       tx_flow_control_calendar - structure of up to 300 calendar entries
*       rx_flow_control_calendar - structure of up to 300 calendar entries
*
*******************************************************************************/
typedef struct sifd_ilkn_cfg_t
{
    sifd_ilkn_serdes_rate_t rate;
    UINT32 lane_mask;
    sifd_ilkn_lane_remap_cfg_t lane_tx_order;
    sifd_ilkn_lane_remap_cfg_t lane_rx_order;

    sifd_oc_burst_t       burst;
    sifd_burst_min_t      burst_min;
    sifd_burst_short_t    burst_short;

    sifd_ilkn_mf_t    metaframe;
    sifd_ilkn_rl_t    tx_rate_limit;

    BOOL8 oduksw_intf_en;
    sifd_mode_t  oduksw_tx_xfer_mode;
    sifd_oduk_hdr_insert_t oduksw_header;
    BOOL8 oduksw_2b_header_mode;

    BOOL8 cpb_intf_en;
    sifd_mode_t  cpb_tx_xfer_mode;
    sifd_mode_t  rx_xfer_mode;

    sifd_fc_mode_t fc_mode;
    UINT32 fc_cal_length;
    sifd_ilkn_calendar_t tx_flow_control_calendar;
    sifd_ilkn_calendar_t rx_flow_control_calendar;

} sifd_ilkn_cfg_t;

/*******************************************************************************
* STRUCTURE: sifd_ilkn_1_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Interlaken 1 configuration.
*
* ELEMENTS:
*       ilkn                     - COPI/Interlaken configuration
*       lane                     - SERDES lane assignment to E1, N1 & N2
*
*******************************************************************************/
typedef struct sifd_ilkn_1_cfg_t
{
    sifd_ilkn_cfg_t ilkn;
    sifd_blk_to_srds_msk_cfg_t lane;
} sifd_ilkn_1_cfg_t;

/*******************************************************************************
* STRUCTURE: sifd_ilkn_2_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Interlaken 2 configuration.
*
* ELEMENTS:
*       ilkn                     - COPI/Interlaken configuration
*       lane                     - SERDES lane assignment to E1, N1 & N2
*
*******************************************************************************/
typedef struct sifd_ilkn_2_cfg_t
{
    sifd_ilkn_cfg_t ilkn;
    sifd_blk_to_srds_msk_cfg_t lane;
} sifd_ilkn_2_cfg_t;


/*******************************************************************************
* STRUCTURE: sifd_los_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the LOS configuration.
*
* ELEMENTS:
*       pin_los_inv     - LOS inversion control
*       pin_los_en      - LOS enable control
*       los_pol_inv     - LOS pin polarity control
*
*******************************************************************************/
typedef struct {

    BOOL pin_los_inv;
    BOOL pin_los_en;
    BOOL los_pol_inv;

} sifd_los_cfg_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC sifd_handle_t *sifd_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void sifd_ctxt_destroy(sifd_handle_t *sifd_handle);
PUBLIC void sifd_handle_init(sifd_handle_t *sifd_handle);
PUBLIC void sifd_alloc_init(sifd_handle_t *sifd_handle);
PUBLIC PMC_ERROR sifd_handle_restart_init(sifd_handle_t *sifd_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t energy_state);

/*******************************************************************************
** Function Prototypes - SIFD Initialization Configuration                    **
*******************************************************************************/

PUBLIC PMC_ERROR sifd_mux_data_path_cfg(sifd_handle_t *h,
                                        sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                        sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr);



/*******************************************************************************
** Function Prototypes - SIFD MUX Configuration                               **
*******************************************************************************/

/*******************************************************************************
** Function Prototypes - SIFD DSIS Configuration                               **
*******************************************************************************/
PUBLIC PMC_ERROR sifd_serdes_subsystem_configure(sifd_handle_t *sifd_handle,
                                                 UINT32 serdes_rx_lane,
                                                 UINT32 serdes_tx_lane,
                                                 UINT32 subsystem_lane,
                                                 sifd_dsis_type_t subsystem_type,
                                                 BOOL single_lane_otn_cbrc,
                                                 UINT32 master_serdes_lane);

PUBLIC PMC_ERROR sifd_serdes_subsystem_disable(sifd_handle_t *sifd_handle,
                                               UINT32 serdes_lane,
                                               UINT32 subsystem_lane);

PUBLIC PMC_ERROR sifd_enet_clock_select(sifd_handle_t *sifd_handle,
                                        UINT32 subsystem_lane,
                                        BOOL enet_clk_enable);

PUBLIC PMC_ERROR sifd_serdes_c_ckctl_configure(sifd_handle_t *sifd_handle,
                                               UINT32 serdes_lane,
                                               sifd_sig_src_type_t clk_source);

PUBLIC PMC_ERROR sifd_serdes_acb_mux_configure(sifd_handle_t *sifd_handle,
                                               UINT32 serdes_lane,
                                               BOOL select_lifd_enable);

PUBLIC PMC_ERROR sifd_line_side_route_configure(sifd_handle_t *sifd_handle,
                                                UINT32 serdes_lane,
                                                BOOL select_lifd_enable);

PUBLIC PMC_ERROR sifd_subsystem_los_configure(sifd_handle_t *sifd_handle,
                                              UINT32 serdes_lane,
                                              UINT32 subsystem_lane,
                                              sifd_los_cfg_t* los_cfg,
                                              BOOL use_dlos_source);

PUBLIC PMC_ERROR sifd_serdes_los_cfg_get(sifd_handle_t *sifd_handle,
                                         UINT32 serdes_lane,
                                         sifd_los_cfg_t* los_cfg);

PUBLIC PMC_ERROR sifd_serdes_los_force(sifd_handle_t *sifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 dlos_en, BOOL8 los_inv);

PUBLIC PMC_ERROR sifd_serdes_los_force_get(sifd_handle_t *sifd_handle,
                                         UINT32 serdes_lane,
                                         BOOL8 *dlos_en, BOOL8 *los_pol_inv);

/*******************************************************************************
** Function Prototypes - SIFD COPI and ILKN Configuration                     **
*******************************************************************************/

PUBLIC PMC_ERROR sifd_ilkn_rx_chnl_cfg(sifd_handle_t *h,
                                       sifd_ilkn_inst_t ilkn_inst,
                                       UINT8 bcw_chnl_id,
                                       BOOL dest,
                                       UINT8 dpi_chnl_id,
                                       UINT32 oduk_4b_header,
                                       UINT16 oduk_2b_header);

PUBLIC PMC_ERROR sifd_ilkn_tx_chnl_cfg(sifd_handle_t *h,
                                       sifd_ilkn_inst_t ilkn_inst,
                                       BOOL source,
                                       UINT8 dpi_chnl_id,
                                       UINT8 bcw_chnl_id,
                                       UINT32 oduk_4b_header,
                                       UINT16 oduk_2b_header);

PUBLIC PMC_ERROR sifd_ilkn_fc_calendar_update(sifd_handle_t *handle,
                                              sifd_ilkn_inst_t ilkn_inst,
                                              sifd_ilkn_calendar_t *tx_flow_control_calendar_ptr,
                                              sifd_ilkn_calendar_t *rx_flow_control_calendar_ptr);

PUBLIC PMC_ERROR sifd_ilkn_enable(sifd_handle_t *handle,
                                  sifd_ilkn_inst_t ilkn_inst,
                                  sifd_ilkn_en_t enable);

PUBLIC PMC_ERROR sifd_ilkn_rx_enable(sifd_handle_t *handle,
                                     sifd_ilkn_inst_t ilkn_inst,
                                     sifd_ilkn_en_t enable);

PUBLIC PMC_ERROR sifd_loopback_cfg(sifd_handle_t *handle,
                                   sifd_ilkn_inst_t ilkn_inst,
                                   sifd_loopback_cfg_t loopback);

PUBLIC PMC_ERROR sifd_loopback_status_get(sifd_handle_t *handle,
                                   sifd_ilkn_inst_t ilkn_inst,
                                   sifd_loopback_cfg_t *loopback_ptr);

PUBLIC void sifd_flow_control_calendar_get(sifd_handle_t *handle,
                                           sifd_ilkn_inst_t ilkn_inst,
                                           UINT8 *tx_bcw_dpi_map,
                                           UINT8 *rx_bcw_dpi_map,
                                           sifd_ilkn_calendar_t *tx_flow_control_calendar_ptr,
                                           sifd_ilkn_calendar_t *rx_flow_control_calendar_ptr);
PUBLIC void sifd_flow_control_calendar_array_get(sifd_handle_t *handle,
                                                 sifd_ilkn_inst_t ilkn_inst,
                                                 sifd_ilkn_calendar_t **tx_sifd_fc_ptr,
                                                 sifd_ilkn_calendar_t **rx_sifd_fc_ptr);
PUBLIC PMC_ERROR sifd_ilkn_copi_lowpwr_cfg(sifd_handle_t *handle, 
                                           sifd_ilkn_inst_t ilkn_inst, 
                                           BOOL8 shutdown);





/*******************************************************************************
** Function Prototypes - SIFD Interlaken Status registers access              **
*******************************************************************************/
PUBLIC PMC_ERROR sifd_ilkn_chnl_cnt_cfg(sifd_handle_t *handle,
                                        sifd_ilkn_inst_t ilkn_inst,
                                        sifd_ilkn_dir_t dir,
                                        UINT32 mon_chnl);
PUBLIC PMC_ERROR sifd_ilkn_get_chnl_counts(sifd_handle_t *handle,
                                           sifd_ilkn_inst_t ilkn_inst,
                                           sifd_ilkn_dir_t dir,
                                           UINT32 mon_chnl,
                                           UINT64 *p_byte_cnt,
                                           UINT64 *p_pkt_cnt,
                                           UINT64 *p_err_cnt,
                                           UINT64 *p_b_max_cnt,
                                           UINT64 *p_b_min_cnt,
                                           UINT32 *p_byte_stat,
                                           UINT32 *p_pkt_stat,
                                           UINT32 *p_err_stat,
                                           UINT32 *p_b_max_stat,
                                           UINT32 *p_b_min_stat);

PUBLIC PMC_ERROR sifd_ilkn_get_rx_intf_counts(sifd_handle_t *handle,
                                              sifd_ilkn_inst_t ilkn_inst,
                                              UINT64 *p_ib_fc_err_cnt,
                                              UINT64 *p_crc24_cnt,
                                              UINT32 *p_ib_fc_stat,
                                              UINT32 *p_crc24_stat);

PUBLIC PMC_ERROR sifd_ilkn_get_serdes_lane_counts(sifd_handle_t *handle,
                                                  sifd_ilkn_inst_t ilkn_inst,
                                                  UINT32 lane, UINT32 counter,
                                                  UINT64 *p_cnt_val,
                                                  UINT32 *p_cnt_stat);
PUBLIC PMC_ERROR sifd_ilkn_prbs_status_get(sifd_handle_t          *handle,
                                           sifd_ilkn_inst_t        ilkn_inst,
                                           UINT32                 *status);
PUBLIC PMC_ERROR sifd_int_link_enable(sifd_handle_t    *sifd_handle,
                                      sifd_ilkn_inst_t  ilkn_inst,
                                      sifd_int_link_t  *int_table_ptr,
                                      sifd_int_link_t  *int_en_table_ptr,
                                      BOOL              enable);
PUBLIC PMC_ERROR sifd_int_link_clear(sifd_handle_t    *sifd_handle,
                                     sifd_ilkn_inst_t  ilkn_inst,
                                     sifd_int_link_t  *int_table_ptr,
                                     sifd_int_link_t  *int_en_table_ptr);
PUBLIC PMC_ERROR sifd_int_link_retrieve(sifd_handle_t    *sifd_handle,
                                        sifd_ilkn_inst_t  ilkn_inst,
                                        sifd_int_link_t  *filt_table_ptr,
                                        sifd_int_link_t  *int_table_ptr);
PUBLIC PMC_ERROR sifd_int_link_enabled_check(sifd_handle_t     *sifd_handle,
                                             sifd_ilkn_inst_t   ilkn_inst,
                                             sifd_int_link_t   *int_en_table_ptr,
                                             BOOL              *int_found_ptr);


PUBLIC void sifd_serdes_dsis_rx_sel_and_rx_en_set(sifd_handle_t *sifd_handle,
                                                  UINT32 subsystem_lane,
                                                  UINT32 internal_lane,
                                                  UINT32 clock_enable);

PUBLIC PMC_ERROR sifd_ilkn_dlos_cfg(sifd_handle_t *h,
                                    sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                    sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                    BOOL8 dlos_enable);

PUBLIC PMC_ERROR sifd_ilkn_toggle_reset(sifd_handle_t *h,
                                        sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                        sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr);

PUBLIC PMC_ERROR sifd_zone_th_cfg(sifd_handle_t *handle,
                                     sifd_ilkn_inst_t ilkn_inst,
                                     UINT32 rx_oduk_zone_hi_th,
                                     UINT32 rx_oduk_zone_lw_th,
                                     UINT32 rx_cpb_zone_hi_th,
                                     UINT32 rx_cpb_zone_lw_th);
PUBLIC void sifd_type_en_cfg(sifd_handle_t *handle,
                             sifd_ilkn_inst_t ilkn_inst,
                             UINT32 rx_type_en,
                             UINT32 tx_type_en);

PUBLIC PMC_ERROR sifd_ilkn_test_cfg(sifd_handle_t     *sifd_handle,
                                    sifd_ilkn_inst_t   ilkn_inst,
                                    sifd_ilkn_dir_t    dir,
                                    sifd_ilkn_test_control_state_t test_mode,
                                    UINT32             pattern_a,
                                    UINT32             pattern_b,
                                    UINT8              rep_a,
                                    UINT8              rep_b);

PUBLIC PMC_ERROR sifd_ilkn_test_cfg_get(sifd_handle_t     *sifd_handle, 
                                        sifd_ilkn_inst_t   ilkn_inst,
                                        sifd_ilkn_dir_t    dir,
                                        sifd_ilkn_test_control_state_t *test_mode,
                                        UINT32             *pattern_a);

PUBLIC PMC_ERROR sifd_ilkn_toggle_reset(sifd_handle_t *h,
                                        sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                        sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr);


PUBLIC PMC_ERROR sifd_los_internal_context_set(sifd_handle_t *sifd_handle,
                                               UINT32 los_pin,
                                               UINT32 los_pin_val);

PUBLIC PMC_ERROR sifd_los_internal_context_unset(sifd_handle_t *sifd_handle,
                                                 UINT32         los_pin);
PUBLIC PMC_ERROR sifd_ilkn_chnl_2b_header_update(sifd_handle_t *h,
                                                 sifd_ilkn_inst_t ilkn_inst,
                                                 UINT8 dpi_chnl_id,
                                                 BOOL   update_rx,
                                                 UINT16 oduk_cur_2b_header,
                                                 UINT16 oduk_new_2b_header);









PUBLIC PMC_ERROR sifd_ilkn_rx_reset_toggle(sifd_handle_t *handle, 
                                           sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                           sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr);

PUBLIC PMC_ERROR sifd_serdes_tx_if_sel_update(sifd_handle_t *sifd_handle, 
                                              UINT32 subsystem_lane, 
                                              sifd_dsis_type_t subsystem_type);

PUBLIC void sifd_serdes_dft_dsis_rx_lane_dis_set(sifd_handle_t *sifd_handle, 
                                                 UINT32         subsystem_lane,
                                                 UINT32         enable);

PUBLIC void sifd_tx_fc_en_set(sifd_handle_t *handle,
                              sifd_ilkn_inst_t ilkn_inst,
                              BOOL8 is_oob_type,
                              sifd_ilkn_en_t ilkn_en);
PUBLIC void sifd_tx_fc_en_get(sifd_handle_t *handle,
                              sifd_ilkn_inst_t ilkn_inst,
                              BOOL8 is_oob_type,
                              sifd_ilkn_en_t *ilkn_en_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _SIFD_H */

/*
** end of file
*/
