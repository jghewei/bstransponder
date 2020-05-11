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

#include "emac_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

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
** Public Functions
*/
/*******************************************************************************
*  emac_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Reads the specified counter.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*   count_type    - counter type to read

* OUTPUTS:
*   *count_val_ptr - counter value
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*  count_type may be one of:
*                   0 - frames transmitted ok
*                   1 - frames rx ok
*                   2 - frame check sequence errors
*                   3 - alignment errors
*                   4 - pause mac ctrl frames transmitted
*                   5 - pause mac ctrl frames rx
*                   6 - frame too long errors
*                   7 - in range length errors
*                   8 - vlan transmitted ok
*                   9 - vlan rx ok
*                  10 - if out octets
*                  11 - if in octets
*                  12 - if in ucast pkts
*                  13 - if in multicast pkts
*                  14 - if in broadcast pkts
*                  15 - if out errors
*                  16 - if out ucast pkts
*                  17 - if out multicast pkts
*                  18 - if out broadcast pkts
*                  19 - ether stats drop events
*                  20 - ether stats octets
*                  21 - ether stats pkts
*                  22 - ether stats undersize pkts
*                  23 - ether stats pkts 64 octets
*                  24 - ether stats pkts 65 to 127 octets
*                  25 - ether stats pkts 128 to 255 octets
*                  26 - ether stats pkts 256 to 511 octets
*                  27 - ether stats pkts 512 to 1023 octets
*                  28 - ether stats pkts 1024 to 1518 octets
*                  29 - ether stats pkts 1519 to max octets
*                  30 - ether stats oversize pkts
*                  31 - ether stats jabber
*                  32 - ether stats fragments
*                  33 - if in errors
*                  34 - cbfc (class based flow control) pause frames transmitted_0
*                  35 - cbcf (class based flow control) pause frames transmitted_1
*                  36 - cbfc (class based flow control) pause frames transmitted_2
*                  37 - cbfc (class based flow control) pause frames transmitted_3
*                  38 - cbfc (class based flow control) pause frames transmitted_4
*                  39 - cbfc (class based flow control) pause frames transmitted_5
*                  40 - cbfc (class based flow control) pause frames transmitted_6
*                  41 - cbfc (class based flow control) pause frames transmitted_7
*                  42 - cbfc (class based flow control) pause frames rx_0
*                  43 - cbfc (class based flow control) pause frames rx_1
*                  44 - cbfc (class based flow control) pause frames rx_2
*                  45 - cbfc (class based flow control) pause frames rx_3
*                  46 - cbfc (class based flow control) pause frames rx_4
*                  47 - cbfc (class based flow control) pause frames rx_5
*                  48 - cbfc (class based flow control) pause frames rx_6
*                  49 - cbfc (class based flow control) pause frames rx_7
*                  50 - mac control frames transmitted
*                  51 - mac control frames rx
**
*******************************************************************************/
PUBLIC PMC_ERROR emac_count_get(emac_handle_t *hndl, UINT32 link, UINT32 count_type,
                                UINT32 *count_val_ptr)
{
    PMC_ENTRY();


    PMC_RETURN(PMC_SUCCESS);

} /* emac_count_get */


/*******************************************************************************
*  emac_ber_bip_err_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get bit error and lane BIP error counters.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to read
*
* OUTPUTS:
*   *ber_cnt_ptr          - BER non-rollover counter [21:0]
*   *lane_bip_err_cnt_ptr - Lane BIP error non-rollover counter [15:0]
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_ber_bip_err_cnt_get(emac_handle_t *hndl, UINT32 link, 
                                          UINT32 *ber_cnt_ptr, UINT32 *lane_bip_err_cnt_ptr)
{
    PMC_ENTRY();

    /* Read counters using *_get() functions from the following registers:
     * Register 0xC0B0 + 1024*A(A=0:11): EMAC_10_40_100GE:PCS(A) - BER Counter (MSB)
     * Register 0xC084 + 1024*A(A=0:11): EMAC_10_40_100GE:PCS(A) - Base-R Status (2/2)
     * Register 0xC168 + 1024*A(A=0:11) + 4*N(N=0:19): EMAC_10_40_100GE:PCS(A) - Lane BIP Error Counters
     */
    /* Does reading register 0xC084 also clear the BLOCK_ERR_CNT? Would this affect
       emac_block_err_cnt_get()? */

    PMC_RETURN(PMC_SUCCESS);

} /* emac_ber_bip_err_cnt_get */

/*******************************************************************************
*  emac_block_err_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get block error counters.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to read
*
* OUTPUTS:
*   *block_err_cnt_msb_ptr  - Bits [39:32] of the 40-bit errored block non-rollover counter
*   *block_err_cnt_lsb_ptr  - Bits [31:0] of the 40-bit errored block non-rollover counter
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_block_err_cnt_get(emac_handle_t *hndl, UINT32 link, 
                                        UINT32 *block_err_cnt_msb_ptr, UINT32 *block_err_cnt_lsb_ptr)
{
    PMC_ENTRY();

    /* Read counters using *_get() functions from the following registers:
     * Register 0xC0B8 + 1024*A(A=0:11): EMAC_10_40_100GE:PCS(A) - PMON Block Error Counter (1/3)
     * Register 0xC0BC + 1024*A(A=0:11): EMAC_10_40_100GE:PCS(A) - PMON Block Error Counter (2/3)
     * Register 0xC0C0 + 1024*A(A=0:11): EMAC_10_40_100GE:PCS(A) - PMON Block Error Counter (3/3)
     */

    PMC_RETURN(PMC_SUCCESS);

} /* emac_block_err_cnt_get */

/*******************************************************************************
*  emac_pcs_test_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get PCS test counter value.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*
* OUTPUTS:
*   *test_err_cnt      - Test Error counter value
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pcs_test_cnt_get(emac_handle_t *hndl, UINT32 link, UINT32 *test_err_cnt)
{
    PMC_ENTRY();

    /* Read counter using *_get() functions from the following register:
     * Register 0xC0AC + 1024*A(A=0:11): PCS(A) - 10/40/100G Base-R Test Error Counter
     */
    PMC_RETURN(PMC_SUCCESS);

} /* emac_pcs_test_cnt_get */

/*******************************************************************************
*  emac_prbs_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get PRBS counter values.
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*
* OUTPUTS:
*   *prbs31_err_cnt_ptr - PRBS-31 Error counter value
*   *prbs9_err_cnt_ptr - PRBS-9 Error counter value
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_prbs_cnt_get(emac_handle_t *hndl, UINT32 link, UINT32 *prbs31_err_cnt_ptr, UINT32 *prbs9_err_cnt_ptr)
{
    PMC_ENTRY();

    /* Read counters using *_get() functions from the following registers:
     * Register 0xF804 + 4*N(N=0:11): PRBS - Lane PRBS31 Error Counter
     * Register 0xF834 + 4*N(N=0:11): PRBS - Lane PRBS9 Error Counter
     */
    PMC_RETURN(PMC_SUCCESS);
} /* emac_prbs_cnt_get */

/*******************************************************************************
*  emac_pcs_stats_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get all PCS Statistic counter values
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*   data_rate     - data rate for the link
*   pmon_enable   - select which BLOCK_ERROR count register to read (MTIP or LCLK)
*
* OUTPUTS:
*   count_val_ptr - pointer to a list of the read value counters for that link
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   count_val index is:
*   [0] = BER_CNT[21:0]
*   [1] = BLK_ERR_CNT[31:0]
*   [2] = BLK_ERR_CNT[39:32]
*   [3] = TEST_PATTERN[15:0]
*   [4] = PRBS9_ERR_CNT[15:0]
*   [5] = PRBS31_ERR_CNT[15:0]
*   [25:6] = LANE_BIP_ERR_CNT[15:0] for lane #0 to #19
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_pcs_stats_cnt_get (emac_handle_t    *hndl,
                                         UINT32           link,
                                         emac_data_rate_t data_rate,
                                         BOOL8            pmon_enable, 
                                         UINT32           count_val_ptr[EMAC_PCS_STATS_SIZE])
{
  UINT32 read_value;
  UINT32 i;

  PMC_ENTRY();

  /* Initialize the list of register value returned */
  for (i = 0; i < EMAC_PCS_STATS_SIZE; i++) {
    count_val_ptr[i] = 0x00000000;
  }

  /* Check for valid link */
  if (EMAC_LINK_100G == data_rate) {
    if (EMAC_100G_LINK_0 != link) {
      PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
    }
  } else if (EMAC_LINK_40G == data_rate) {
    if ((EMAC_40G_LINK_0 != link) && (EMAC_40G_LINK_4 != link) && (EMAC_40G_LINK_8 != link)) {
      PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
    }
  } /* end if */

  /* Reading a MTIP PCS statistics register clears the counter!!! */
  /* Read the LSB of BER_CNT and BLOCK_ERR_CNT */
  read_value = emac_pcs_reg_BASE_R_STAT_2_2__read(NULL, hndl, link);
  count_val_ptr[0] = read_value & PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_MSK;
  /* block error count LSB is overwritten if PMON mode is enabled */
  count_val_ptr[1] = read_value & PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_MSK;
  
  /* Read the MSB of BER_CNT (21:6) */
  read_value = emac_pcs_reg_BER_CNT_MSB__read(NULL, hndl, link);
  count_val_ptr[0] = count_val_ptr[0] | (read_value & PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_MSK) << 6;
  
  if (pmon_enable) {
    /* Read the 40-bit PMON version of the block error count */
    /* bit[15:0] */
    read_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_1_3__read(NULL, hndl, link);
    count_val_ptr[1] = read_value & PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_MSK;
    /* bit[31:16] */
    read_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_2_3__read(NULL, hndl, link);
    count_val_ptr[1] = count_val_ptr[1] & (read_value & PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_MSK) << 16;
    /* bit[39:32] */
    read_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_3_3__read(NULL, hndl, link);
    count_val_ptr[2] = read_value & PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_MSK;
  } else {
    /* Read the MSB of BLK_ERR_CNT (22:8) */
    read_value = emac_pcs_reg_BLOCK_ERR_CNT_MSB__read(NULL, hndl, link);
    count_val_ptr[1] = count_val_ptr[0] | (read_value & PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_MSK) << 8;
    count_val_ptr[2] = 0x00000000;
  }

  /* Read the test pattern count */
  read_value = emac_pcs_reg_BASE_R_TEST_ERR_CNT_read(NULL, hndl, link);
  count_val_ptr[3] = read_value & PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_MSK;

  /* Read the PRBS9 and 31 count */
  read_value = emac_prbs_reg_LANE_PRBS9_ERR_CNT_array_read(NULL, hndl, link);
  count_val_ptr[4] = read_value & PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_MSK;
  read_value = emac_prbs_reg_LANE_PRBS31_ERR_CNT_array_read(NULL, hndl, link);
  count_val_ptr[5] = read_value & PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_MSK;

  /* Read the lane BIP error count */
  /* In 100G, all 20 lanes are valid. In 40G, only lanes 0 to 3 are valid */
  if (EMAC_LINK_100G == data_rate && EMAC_100G_LINK_0 == link) {
    for (i = 0; i <= 19; i++) {
      read_value = emac_pcs_reg_LANE_BIP_ERR_CNT_array_read(NULL, hndl, link, i);
      count_val_ptr[6+i] = read_value & PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_MSK;
    }
  } else if ((EMAC_LINK_40G  == data_rate && EMAC_40G_LINK_0  == link) ||
             (EMAC_LINK_40G  == data_rate && EMAC_40G_LINK_4  == link) ||
             (EMAC_LINK_40G  == data_rate && EMAC_40G_LINK_8  == link)) {
    for (i = 0; i <= 3; i++) {
      read_value = emac_pcs_reg_LANE_BIP_ERR_CNT_array_read(NULL, hndl, link, i);
      count_val_ptr[6+i] = read_value & PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_MSK;
    }
    /* Unused in 40G mode */
    for (i = 4; i <= 19; i++) {
      count_val_ptr[6+i] = 0x0;
    }
  } else {
    /* Unused in 10G mode */
    for (i = 0; i <= 19; i++) {
      count_val_ptr[6+i] = 0x0;
    }
  }

  PMC_RETURN(PMC_SUCCESS);

} /* emac_pcs_stats_cnt_get */

/*******************************************************************************
*  emac_mac_stats_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get all MAC Statistic counter values
*
* INPUTS:
*   hndl          - handle to the EMAC instance to be operated on
*   link          - link to configure
*   data_rate     - data rate for the link
*
* OUTPUTS:
*   count_val_ptr - pointer to a list of the read value counters for that link
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*   count_val index is based on the register address.
*   with the tx frame OK (LSB) as the base address ([0])
*
*******************************************************************************/
PUBLIC PMC_ERROR emac_mac_stats_cnt_get (emac_handle_t    *hndl,
                                         UINT32           link,
                                         emac_data_rate_t data_rate,
                                         UINT32           count_val_ptr[EMAC_MAC_STATS_SIZE])
{
  UINT32 index;
  UINT32 base_addr;
  UINT32 i;

  PMC_ENTRY();
  
  /* Initialize the list of register value returned */
  for (i = 0; i < EMAC_MAC_STATS_SIZE; i++) {
    count_val_ptr[i] = 0x00000000;
  }

  /* Check for valid link */
  if (EMAC_LINK_100G == data_rate) {
    if (EMAC_100G_LINK_0 != link) {
      PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
    }
  } else if (EMAC_LINK_40G == data_rate) {
    if ((EMAC_40G_LINK_0 != link) && (EMAC_40G_LINK_4 != link) && (EMAC_40G_LINK_8 != link)) {
      PMC_RETURN(EMAC_LOG_ERR_INVALID_PARAMETERS);
    }
  } /* end if */

  base_addr = PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_;

  index = (PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AALIGNMENTERRORS_LSB_get                     (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AALIGNMENTERRORS_MSB_get                     (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_0_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_0_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_1_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_1_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_2_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_2_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_3_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_3_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_4_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_4_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_5_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_5_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_6_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_6_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_7_LSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESRX_7_MSB_get           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_0_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_0_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBCFPAUSEFRAMESTX_1_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBCFPAUSEFRAMESTX_1_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_2_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_2_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_3_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_3_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_4_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_4_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_5_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_5_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_6_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_6_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_7_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ACBFCPAUSEFRAMESTX_7_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AFRAMECHECKSEQUENCEERRORS_LSB_get            (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AFRAMECHECKSEQUENCEERRORS_MSB_get            (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_ - base_addr) / 4;  count_val_ptr[index] = emac_mac_field_AFRAMESRXOK_LSB_get                    (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_ - base_addr) / 4;  count_val_ptr[index] = emac_mac_field_AFRAMESRXOK_MSB_get                    (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_ - base_addr) / 4;  count_val_ptr[index] = emac_mac_field_AFRAMESTXOK_LSB_get                 (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_ - base_addr) / 4;  count_val_ptr[index] = emac_mac_field_AFRAMESTXOK_MSB_get                 (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AFRAMETOOLONGERRORS_LSB_get                  (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AFRAMETOOLONGERRORS_MSB_get                  (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AINRANGELENGTHERRORS_LSB_get                 (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AINRANGELENGTHERRORS_MSB_get                 (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AMACCONTROLFRAMESRX_LSB_get         (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_AMACCONTROLFRAMESRX_MSB_get         (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_APAUSEMACCTRLFRAMESRX_LSB_get          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_APAUSEMACCTRLFRAMESRX_MSB_get          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_APAUSEMACCTRLFRAMESTX_LSB_get       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_APAUSEMACCTRLFRAMESTX_MSB_get       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSDROPEVENTS_LSB_get                 (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSDROPEVENTS_MSB_get                 (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSFRAGMENTS_LSB_get                  (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSFRAGMENTS_MSB_get                  (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSJABBER_LSB_get                     (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSJABBER_MSB_get                     (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSOCTS_LSB_get                     (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSOCTS_MSB_get                     (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSOVERSIZEPKTS_LSB_get               (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSOVERSIZEPKTS_MSB_get               (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_LSB_get       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_MSB_get       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS128TO255OCTS_LSB_get         (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS128TO255OCTS_MSB_get         (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS256TO511OCTS_LSB_get         (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS256TO511OCTS_MSB_get         (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_LSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_MSB_get        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS64OCTS_LSB_get               (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS64OCTS_MSB_get               (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS65TO127OCTS_LSB_get          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS65TO127OCTS_MSB_get          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS_LSB_get                       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSPKTS_MSB_get                       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSUNDERSIZEPKTS_LSB_get              (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_ETHERSTATSUNDERSIZEPKTS_MSB_get              (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINBROADCASTPKTS_LSB_get                    (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINBROADCASTPKTS_MSB_get                    (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINERRS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINERRORS_LSB_get                           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINERRS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINERRORS_MSB_get                           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINMULTICASTPKTS_LSB_get                    (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINMULTICASTPKTS_MSB_get                    (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINOCTS_LSB_get                           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINOCTS_MSB_get                           (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINUCASTPKTS_LSB_get                        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFINUCASTPKTS_MSB_get                        (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTBROADCASTPKTS_LSB_get                   (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTBROADCASTPKTS_MSB_get                   (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTERRORS_LSB_get                          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTERRORS_MSB_get                          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTMULTICASTPKTS_LSB_get                   (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTMULTICASTPKTS_MSB_get                   (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTOCTS_LSB_get                          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTOCTS_MSB_get                          (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTUCASTPKTS_LSB_get                       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_IFOUTUCASTPKTS_MSB_get                       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_VLANRXOK_LSB_get                       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_VLANRXOK_MSB_get                       (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_VLANTXOK_LSB_get                    (NULL, hndl, link);
  index = (PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_ - base_addr) / 4;    count_val_ptr[index] = emac_mac_field_VLANTXOK_MSB_get                    (NULL, hndl, link);

  PMC_RETURN(PMC_SUCCESS);

} /* emac_mac_stats_cnt_get */

/*
** Private Functions
*/

/*
** End of file
*/
