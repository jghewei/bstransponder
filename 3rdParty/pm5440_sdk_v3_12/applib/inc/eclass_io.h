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
 *     contains prototypes for the field accessor functions
 *     and register accessor functions for the eclass block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing eclass io functions is:
 | {
 |     eclass_buffer_t b_ptr[1];
 |     eclass_buffer_init( b_ptr, h_ptr );
 |     value = eclass_field_<FIELD>_get( b_ptr, h_ptr );
 |     eclass_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     eclass_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = eclass_field_<FIELD>_get( NULL, h_ptr );
 | eclass_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------------+
 | | Register 0x0000001c ECLASS120_LINK_EN |
 | +---------------------------------------+
 | | bit  11 R/W  ECLASS120_LINK11_EN      |
 | | bit  10 R/W  ECLASS120_LINK10_EN      |
 | | bit  9  R/W  ECLASS120_LINK9_EN       |
 | | bit  8  R/W  ECLASS120_LINK8_EN       |
 | | bit  7  R/W  ECLASS120_LINK7_EN       |
 | | bit  6  R/W  ECLASS120_LINK6_EN       |
 | | bit  5  R/W  ECLASS120_LINK5_EN       |
 | | bit  4  R/W  ECLASS120_LINK4_EN       |
 | | bit  3  R/W  ECLASS120_LINK3_EN       |
 | | bit  2  R/W  ECLASS120_LINK2_EN       |
 | | bit  1  R/W  ECLASS120_LINK1_EN       |
 | | bit  0  R/W  ECLASS120_LINK0_EN       |
 | +---------------------------------------+
 | 
 | void   eclass_field_ECLASS120_LINK11_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK11_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK10_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK10_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK9_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK9_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK8_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK8_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK7_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK7_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK6_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK6_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK5_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK5_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK4_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK4_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK3_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK3_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK2_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK2_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK1_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK1_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_ECLASS120_LINK0_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_ECLASS120_LINK0_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000024 + (N) * 512) GEN_CFG_VLAN_MPLS |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  30    R/W  MPLS_ETYPE_CUSTOM_EN                |
 | | bit  29    R/W  MPLS_ETYPE_8848_EN                  |
 | | bit  28    R/W  MPLS_ETYPE_8847_EN                  |
 | | bit  27:26 R/W  MPLS_NB_LEVELS                      |
 | | bit  25    R/W  MPLS_OVERFLOW_CFG                   |
 | | bit  24:23 R/W  MPLS_MESS                           |
 | | bit  22    R/W  VLAN_TPID_CUSTOM_OUTER_EN           |
 | | bit  21    R/W  VLAN_TPID_CUSTOM_INNER_EN           |
 | | bit  20    R/W  VLAN_TPID_88A8_EN                   |
 | | bit  19    R/W  VLAN_TPID_9100_EN                   |
 | | bit  18    R/W  VLAN_TPID_8100_EN                   |
 | | bit  17    R/W  VLAN_OUTER_EN                       |
 | | bit  16    R/W  VLAN_INNER_EN                       |
 | | bit  10:9  R/W  VLAN_MPLS_STATE_EN                  |
 | | bit  7     R/W  VLAN_MPLS_PRIO_EN                   |
 | | bit  6:4   R/W  VLAN_MPLS_PRIO_DEFAULT              |
 | | bit  3:2   R/W  VLAN_MPLS_PRIO_LEVEL                |
 | | bit  1:0   R/W  VLAN_MPLS_PRIO_TYPE                 |
 | +-----------------------------------------------------+
 | 
 | void   eclass_field_MPLS_ETYPE_CUSTOM_EN_set         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MPLS_ETYPE_CUSTOM_EN_get         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_MPLS_ETYPE_8848_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MPLS_ETYPE_8848_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_MPLS_ETYPE_8847_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MPLS_ETYPE_8847_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_MPLS_NB_LEVELS_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MPLS_NB_LEVELS_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_MPLS_NB_LEVELS_set         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_MPLS_NB_LEVELS_get         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_MPLS_OVERFLOW_CFG_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MPLS_OVERFLOW_CFG_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_MPLS_MESS_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MPLS_MESS_get                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_MPLS_MESS_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_MPLS_MESS_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_VLAN_TPID_CUSTOM_OUTER_EN_set    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_TPID_CUSTOM_OUTER_EN_get    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_TPID_CUSTOM_INNER_EN_set    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_TPID_CUSTOM_INNER_EN_get    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_TPID_88A8_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_TPID_88A8_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_TPID_9100_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_TPID_9100_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_TPID_8100_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_TPID_8100_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_OUTER_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_OUTER_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_INNER_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_INNER_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_MPLS_STATE_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_MPLS_STATE_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_VLAN_MPLS_STATE_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_VLAN_MPLS_STATE_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_VLAN_MPLS_PRIO_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_MPLS_PRIO_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_VLAN_MPLS_PRIO_DEFAULT_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_MPLS_PRIO_DEFAULT_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_VLAN_MPLS_PRIO_DEFAULT_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_VLAN_MPLS_PRIO_DEFAULT_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_VLAN_MPLS_PRIO_LEVEL_set         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_MPLS_PRIO_LEVEL_get         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_VLAN_MPLS_PRIO_LEVEL_set   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_VLAN_MPLS_PRIO_LEVEL_get   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_VLAN_MPLS_PRIO_TYPE_set          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_MPLS_PRIO_TYPE_get          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_VLAN_MPLS_PRIO_TYPE_set    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_VLAN_MPLS_PRIO_TYPE_get    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x00000028 + (N) * 512) VLAN_TPID |
 | |     N[0..11]                                |
 | +---------------------------------------------+
 | | bit  31:16 R/W  VLAN_CUSTOM_TPID_OUTER      |
 | | bit  15:0  R/W  VLAN_CUSTOM_TPID_INNER      |
 | +---------------------------------------------+
 | 
 | void   eclass_field_VLAN_CUSTOM_TPID_OUTER_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_CUSTOM_TPID_OUTER_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_VLAN_CUSTOM_TPID_OUTER_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_VLAN_CUSTOM_TPID_OUTER_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_VLAN_CUSTOM_TPID_INNER_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_VLAN_CUSTOM_TPID_INNER_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_VLAN_CUSTOM_TPID_INNER_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_VLAN_CUSTOM_TPID_INNER_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000030 + (N) * 512) MPLS_ETHERTYPE |
 | |     N[0..11]                                     |
 | +--------------------------------------------------+
 | | bit  15:0 R/W  MPLS_CUSTOM_ETHERTYPE             |
 | +--------------------------------------------------+
 | 
 | void   eclass_field_MPLS_CUSTOM_ETHERTYPE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MPLS_CUSTOM_ETHERTYPE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_MPLS_CUSTOM_ETHERTYPE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_MPLS_CUSTOM_ETHERTYPE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000038 + (N) * 512) VLANMPLS_FILT20_TAG0_1 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  19:0 R/W  FILT20_TAG0_VALUE                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG0_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG0_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG0_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG0_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x0000003c + (N) * 512) VLANMPLS_FILT20_TAG0_2 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  31:28 R/W  FILT20_TAG0_TYPE                         |
 | | bit  24    R/W  FILT20_TAG0_INV                          |
 | | bit  22:20 R/W  FILT20_TAG0_ADDR                         |
 | | bit  19:0  R/W  FILT20_TAG0_MASK                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG0_TYPE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG0_TYPE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG0_TYPE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG0_TYPE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG0_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG0_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FILT20_TAG0_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG0_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG0_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG0_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG0_MASK_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG0_MASK_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG0_MASK_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG0_MASK_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000040 + (N) * 512) VLANMPLS_FILT20_TAG1_1 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  19:0 R/W  FILT20_TAG1_VALUE                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG1_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG1_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG1_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG1_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000044 + (N) * 512) VLANMPLS_FILT20_TAG1_2 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  31:28 R/W  FILT20_TAG1_TYPE                         |
 | | bit  24    R/W  FILT20_TAG1_INV                          |
 | | bit  22:20 R/W  FILT20_TAG1_ADDR                         |
 | | bit  19:0  R/W  FILT20_TAG1_MASK                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG1_TYPE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG1_TYPE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG1_TYPE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG1_TYPE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG1_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG1_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FILT20_TAG1_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG1_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG1_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG1_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG1_MASK_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG1_MASK_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG1_MASK_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG1_MASK_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000048 + (N) * 512) VLANMPLS_FILT20_TAG2_1 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  19:0 R/W  FILT20_TAG2_VALUE                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG2_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG2_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG2_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG2_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x0000004c + (N) * 512) VLANMPLS_FILT20_TAG2_2 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  31:28 R/W  FILT20_TAG2_TYPE                         |
 | | bit  24    R/W  FILT20_TAG2_INV                          |
 | | bit  22:20 R/W  FILT20_TAG2_ADDR                         |
 | | bit  19:0  R/W  FILT20_TAG2_MASK                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG2_TYPE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG2_TYPE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG2_TYPE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG2_TYPE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG2_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG2_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FILT20_TAG2_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG2_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG2_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG2_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG2_MASK_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG2_MASK_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG2_MASK_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG2_MASK_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000050 + (N) * 512) VLANMPLS_FILT20_TAG3_1 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  19:0 R/W  FILT20_TAG3_VALUE                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG3_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG3_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG3_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG3_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000054 + (N) * 512) VLANMPLS_FILT20_TAG3_2 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  31:28 R/W  FILT20_TAG3_TYPE                         |
 | | bit  24    R/W  FILT20_TAG3_INV                          |
 | | bit  22:20 R/W  FILT20_TAG3_ADDR                         |
 | | bit  19:0  R/W  FILT20_TAG3_MASK                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG3_TYPE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG3_TYPE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG3_TYPE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG3_TYPE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG3_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG3_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FILT20_TAG3_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG3_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG3_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG3_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG3_MASK_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG3_MASK_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG3_MASK_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG3_MASK_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000058 + (N) * 512) VLANMPLS_FILT20_TAG4_1 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  19:0 R/W  FILT20_TAG4_VALUE                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG4_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG4_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG4_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG4_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x0000005c + (N) * 512) VLANMPLS_FILT20_TAG4_2 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  31:28 R/W  FILT20_TAG4_TYPE                         |
 | | bit  24    R/W  FILT20_TAG4_INV                          |
 | | bit  22:20 R/W  FILT20_TAG4_ADDR                         |
 | | bit  19:0  R/W  FILT20_TAG4_MASK                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG4_TYPE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG4_TYPE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG4_TYPE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG4_TYPE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG4_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG4_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FILT20_TAG4_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG4_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG4_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG4_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG4_MASK_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG4_MASK_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG4_MASK_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG4_MASK_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000060 + (N) * 512) VLANMPLS_FILT20_TAG5_1 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  19:0 R/W  FILT20_TAG5_VALUE                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG5_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG5_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG5_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG5_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000064 + (N) * 512) VLANMPLS_FILT20_TAG5_2 |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  31:28 R/W  FILT20_TAG5_TYPE                         |
 | | bit  24    R/W  FILT20_TAG5_INV                          |
 | | bit  22:20 R/W  FILT20_TAG5_ADDR                         |
 | | bit  19:0  R/W  FILT20_TAG5_MASK                         |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_FILT20_TAG5_TYPE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG5_TYPE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG5_TYPE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG5_TYPE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG5_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG5_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FILT20_TAG5_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG5_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG5_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG5_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FILT20_TAG5_MASK_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FILT20_TAG5_MASK_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FILT20_TAG5_MASK_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FILT20_TAG5_MASK_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000070 + (N) * 512) MAN_VLAN_CFG |
 | |     N[0..11]                                   |
 | +------------------------------------------------+
 | | bit  27:16 R/W  MANAGEMENT_VLAN_TCI_VLANID     |
 | | bit  15:0  R/W  MANAGEMENT_VLAN_TPID           |
 | +------------------------------------------------+
 | 
 | void   eclass_field_MANAGEMENT_VLAN_TCI_VLANID_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MANAGEMENT_VLAN_TCI_VLANID_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_MANAGEMENT_VLAN_TCI_VLANID_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_MANAGEMENT_VLAN_TCI_VLANID_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_MANAGEMENT_VLAN_TPID_set             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_MANAGEMENT_VLAN_TPID_get             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_MANAGEMENT_VLAN_TPID_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_MANAGEMENT_VLAN_TPID_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00000074 + (N) * 512) UNICAST_DA_ETH_LSB |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  ADDR_LSB                              |
 | +------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_LSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_LSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_LSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_LSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00000078 + (N) * 512) UNICAST_DA_ETH_MSB |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  16   R/W  ADDR_EN                               |
 | | bit  15:0 R/W  ADDR_MSB                              |
 | +------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ADDR_MSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_MSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_MSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_MSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x0000007c + (N) * 512) UNICAST_DA_ETH_P2P_LSB |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  ADDR_P2P_LSB                              |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_P2P_LSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_P2P_LSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_P2P_LSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_P2P_LSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000080 + (N) * 512) UNICAST_DA_ETH_P2P_MSB |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  16   R/W  ADDR_P2P_EN                               |
 | | bit  15:0 R/W  ADDR_P2P_MSB                              |
 | +----------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_P2P_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_P2P_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ADDR_P2P_MSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_P2P_MSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_P2P_MSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_P2P_MSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000084 + (N) * 512) UNICAST_DEST_PORT_UDP |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:16 R/W  UDP_PORT_IPV6                           |
 | | bit  15:0  R/W  UDP_PORT_IPV4                           |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_UDP_PORT_IPV6_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_UDP_PORT_IPV6_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_UDP_PORT_IPV6_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_UDP_PORT_IPV6_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_UDP_PORT_IPV4_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_UDP_PORT_IPV4_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_UDP_PORT_IPV4_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_UDP_PORT_IPV4_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000088 + (N) * 512) UNICAST_DA_IPV4 |
 | |     N[0..11]                                      |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV4                          |
 | +---------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV4_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV4_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV4_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV4_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x0000008c + (N) * 512) UNICAST_DA_IPV4_P2P |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV4_P2P                          |
 | +-------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV4_P2P_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV4_P2P_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV4_P2P_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV4_P2P_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000090 + (N) * 512) UNICAST_DA_IPV6_0 |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_0                          |
 | +-----------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_0_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_0_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_0_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_0_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000094 + (N) * 512) UNICAST_DA_IPV6_1 |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_1                          |
 | +-----------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_1_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_1_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_1_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_1_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000098 + (N) * 512) UNICAST_DA_IPV6_2 |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_2                          |
 | +-----------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_2_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_2_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_2_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_2_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x0000009c + (N) * 512) UNICAST_DA_IPV6_3 |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_3                          |
 | +-----------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_3_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_3_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_3_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_3_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000a4 + (N) * 512) UNICAST_DA_IPV6_P2P_0 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_P2P_0                          |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_P2P_0_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_P2P_0_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_P2P_0_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_P2P_0_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000a8 + (N) * 512) UNICAST_DA_IPV6_P2P_1 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_P2P_1                          |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_P2P_1_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_P2P_1_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_P2P_1_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_P2P_1_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000ac + (N) * 512) UNICAST_DA_IPV6_P2P_2 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_P2P_2                          |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_P2P_2_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_P2P_2_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_P2P_2_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_P2P_2_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000b0 + (N) * 512) UNICAST_DA_IPV6_P2P_3 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  ADDR_IPV6_P2P_3                          |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ADDR_IPV6_P2P_3_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_P2P_3_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ADDR_IPV6_P2P_3_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ADDR_IPV6_P2P_3_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000d0 + (N) * 512) GEN_FILTER_CFG_FAND_0 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:29 R    FAND_0_UNUSEDZEROS                      |
 | | bit  16    R/W  FAND_0_OR_CFG                           |
 | | bit  15:0  R/W  FAND_0_F16BITS_EN                       |
 | +---------------------------------------------------------+
 | 
 | UINT32 eclass_field_FAND_0_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FAND_0_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FAND_0_OR_CFG_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_0_OR_CFG_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FAND_0_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_0_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FAND_0_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FAND_0_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000d4 + (N) * 512) GEN_FILTER_CFG_FAND_1 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:29 R    FAND_1_UNUSEDZEROS                      |
 | | bit  16    R/W  FAND_1_OR_CFG                           |
 | | bit  15:0  R/W  FAND_1_F16BITS_EN                       |
 | +---------------------------------------------------------+
 | 
 | UINT32 eclass_field_FAND_1_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FAND_1_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FAND_1_OR_CFG_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_1_OR_CFG_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FAND_1_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_1_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FAND_1_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FAND_1_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000d8 + (N) * 512) GEN_FILTER_CFG_FAND_2 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:29 R    FAND_2_UNUSEDZEROS                      |
 | | bit  16    R/W  FAND_2_OR_CFG                           |
 | | bit  15:0  R/W  FAND_2_F16BITS_EN                       |
 | +---------------------------------------------------------+
 | 
 | UINT32 eclass_field_FAND_2_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FAND_2_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FAND_2_OR_CFG_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_2_OR_CFG_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FAND_2_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_2_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FAND_2_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FAND_2_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000dc + (N) * 512) GEN_FILTER_CFG_FAND_3 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:29 R    FAND_3_UNUSEDZEROS                      |
 | | bit  16    R/W  FAND_3_OR_CFG                           |
 | | bit  15:0  R/W  FAND_3_F16BITS_EN                       |
 | +---------------------------------------------------------+
 | 
 | UINT32 eclass_field_FAND_3_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FAND_3_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FAND_3_OR_CFG_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_3_OR_CFG_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FAND_3_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FAND_3_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FAND_3_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FAND_3_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000e0 + (N) * 512) GEN_FILTER_CFG_FOR_0 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_0_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_0_INV                              |
 | | bit  19:16 R/W  FOR_0_FAND_EN                          |
 | | bit  15:0  R/W  FOR_0_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_0_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_0_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_0_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_0_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_0_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_0_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_0_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_0_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_0_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_0_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_0_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_0_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000e4 + (N) * 512) GEN_FILTER_CFG_FOR_1 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_1_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_1_INV                              |
 | | bit  19:16 R/W  FOR_1_FAND_EN                          |
 | | bit  15:0  R/W  FOR_1_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_1_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_1_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_1_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_1_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_1_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_1_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_1_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_1_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_1_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_1_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_1_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_1_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000e8 + (N) * 512) GEN_FILTER_CFG_FOR_2 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_2_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_2_INV                              |
 | | bit  19:16 R/W  FOR_2_FAND_EN                          |
 | | bit  15:0  R/W  FOR_2_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_2_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_2_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_2_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_2_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_2_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_2_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_2_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_2_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_2_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_2_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_2_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_2_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000ec + (N) * 512) GEN_FILTER_CFG_FOR_3 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_3_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_3_INV                              |
 | | bit  19:16 R/W  FOR_3_FAND_EN                          |
 | | bit  15:0  R/W  FOR_3_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_3_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_3_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_3_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_3_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_3_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_3_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_3_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_3_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_3_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_3_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_3_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_3_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000f0 + (N) * 512) GEN_FILTER_CFG_FOR_4 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_4_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_4_INV                              |
 | | bit  19:16 R/W  FOR_4_FAND_EN                          |
 | | bit  15:0  R/W  FOR_4_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_4_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_4_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_4_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_4_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_4_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_4_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_4_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_4_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_4_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_4_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_4_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_4_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000f4 + (N) * 512) GEN_FILTER_CFG_FOR_5 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_5_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_5_INV                              |
 | | bit  19:16 R/W  FOR_5_FAND_EN                          |
 | | bit  15:0  R/W  FOR_5_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_5_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_5_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_5_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_5_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_5_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_5_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_5_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_5_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_5_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_5_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_5_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_5_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000f8 + (N) * 512) GEN_FILTER_CFG_FOR_6 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_6_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_6_INV                              |
 | | bit  19:16 R/W  FOR_6_FAND_EN                          |
 | | bit  15:0  R/W  FOR_6_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_6_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_6_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_6_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_6_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_6_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_6_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_6_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_6_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_6_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_6_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_6_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_6_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000fc + (N) * 512) GEN_FILTER_CFG_FOR_7 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:29 R    FOR_7_UNUSEDZEROS                      |
 | | bit  20    R/W  FOR_7_INV                              |
 | | bit  19:16 R/W  FOR_7_FAND_EN                          |
 | | bit  15:0  R/W  FOR_7_F16BITS_EN                       |
 | +--------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOR_7_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOR_7_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_7_INV_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_7_INV_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOR_7_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_7_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_7_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_7_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOR_7_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOR_7_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOR_7_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOR_7_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000110 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_0 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_0_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_0_EN                                     |
 | | bit  27:20 R/W  FOUT_0_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_0_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_0_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_0_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_0_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_0_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_0_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_0_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_0_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_0_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_0_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_0_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_0_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_0_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_0_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_0_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_0_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_0_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_0_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000114 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_1 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_1_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_1_EN                                     |
 | | bit  27:20 R/W  FOUT_1_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_1_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_1_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_1_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_1_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_1_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_1_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_1_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_1_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_1_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_1_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_1_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_1_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_1_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_1_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_1_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_1_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_1_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_1_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000118 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_2 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_2_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_2_EN                                     |
 | | bit  27:20 R/W  FOUT_2_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_2_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_2_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_2_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_2_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_2_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_2_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_2_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_2_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_2_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_2_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_2_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_2_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_2_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_2_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_2_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_2_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_2_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_2_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x0000011c + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_3 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_3_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_3_EN                                     |
 | | bit  27:20 R/W  FOUT_3_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_3_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_3_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_3_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_3_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_3_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_3_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_3_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_3_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_3_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_3_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_3_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_3_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_3_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_3_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_3_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_3_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_3_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_3_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000120 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_4 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_4_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_4_EN                                     |
 | | bit  27:20 R/W  FOUT_4_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_4_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_4_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_4_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_4_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_4_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_4_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_4_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_4_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_4_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_4_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_4_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_4_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_4_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_4_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_4_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_4_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_4_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_4_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000124 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_5 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_5_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_5_EN                                     |
 | | bit  27:20 R/W  FOUT_5_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_5_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_5_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_5_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_5_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_5_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_5_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_5_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_5_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_5_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_5_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_5_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_5_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_5_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_5_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_5_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_5_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_5_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_5_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000128 + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_6 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_6_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_6_EN                                     |
 | | bit  27:20 R/W  FOUT_6_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_6_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_6_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_6_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_6_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_6_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_6_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_6_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_6_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_6_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_6_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_6_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_6_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_6_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_6_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_6_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_6_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_6_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_6_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x0000012c + (N) * 512) GEN_FILTER_CFG_FILTER_OUT_7 |
 | |     N[0..11]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:29 R    FOUT_7_UNUSEDZEROS                            |
 | | bit  28    R/W  FOUT_7_EN                                     |
 | | bit  27:20 R/W  FOUT_7_FOR_EN                                 |
 | | bit  19:16 R/W  FOUT_7_FAND_EN                                |
 | | bit  15:0  R/W  FOUT_7_F16BITS_EN                             |
 | +---------------------------------------------------------------+
 | 
 | UINT32 eclass_field_FOUT_7_UNUSEDZEROS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | UINT32 eclass_field_range_FOUT_7_UNUSEDZEROS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_7_EN_set                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_7_EN_get                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_FOUT_7_FOR_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_7_FOR_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_7_FOR_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_7_FOR_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_7_FAND_EN_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_7_FAND_EN_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_7_FAND_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_7_FAND_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_FOUT_7_F16BITS_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_FOUT_7_F16BITS_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_FOUT_7_F16BITS_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_FOUT_7_F16BITS_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000140 + (N) * 512) ROUTING_AND_FILTER_0 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND0_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND0_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND0_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND0_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND0_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND0_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND0_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND0_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND0_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND0_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000144 + (N) * 512) ROUTING_AND_FILTER_1 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND1_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND1_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND1_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND1_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND1_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND1_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND1_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND1_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND1_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND1_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000148 + (N) * 512) ROUTING_AND_FILTER_2 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND2_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND2_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND2_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND2_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND2_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND2_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND2_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND2_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND2_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND2_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x0000014c + (N) * 512) ROUTING_AND_FILTER_3 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND3_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND3_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND3_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND3_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND3_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND3_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND3_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND3_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND3_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND3_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000150 + (N) * 512) ROUTING_AND_FILTER_4 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND4_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND4_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND4_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND4_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND4_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND4_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND4_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND4_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND4_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND4_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000154 + (N) * 512) ROUTING_AND_FILTER_5 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND5_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND5_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND5_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND5_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND5_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND5_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND5_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND5_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND5_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND5_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000158 + (N) * 512) ROUTING_AND_FILTER_6 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND6_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND6_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND6_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND6_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND6_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND6_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND6_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND6_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND6_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND6_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x0000015c + (N) * 512) ROUTING_AND_FILTER_7 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND7_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND7_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND7_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND7_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND7_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND7_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND7_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND7_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND7_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND7_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000160 + (N) * 512) ROUTING_AND_FILTER_8 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND8_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND8_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND8_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND8_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND8_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND8_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND8_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND8_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND8_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND8_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000164 + (N) * 512) ROUTING_AND_FILTER_9 |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND9_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND9_INPUT_GEN_EN              |
 | +--------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND9_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND9_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND9_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND9_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND9_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND9_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND9_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND9_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000168 + (N) * 512) ROUTING_AND_FILTER_10 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND10_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND10_INPUT_GEN_EN              |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND10_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND10_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND10_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND10_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND10_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND10_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND10_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND10_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x0000016c + (N) * 512) ROUTING_AND_FILTER_11 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND11_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND11_INPUT_GEN_EN              |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND11_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND11_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND11_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND11_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND11_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND11_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND11_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND11_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000170 + (N) * 512) ROUTING_AND_FILTER_12 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND12_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND12_INPUT_GEN_EN              |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND12_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND12_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND12_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND12_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND12_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND12_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND12_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND12_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000174 + (N) * 512) ROUTING_AND_FILTER_13 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND13_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND13_INPUT_GEN_EN              |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND13_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND13_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND13_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND13_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND13_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND13_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND13_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND13_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000178 + (N) * 512) ROUTING_AND_FILTER_14 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND14_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND14_INPUT_GEN_EN              |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND14_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND14_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND14_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND14_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND14_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND14_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND14_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND14_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x0000017c + (N) * 512) ROUTING_AND_FILTER_15 |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_AND15_INPUT_VLANMPLS_EN         |
 | | bit  25:0  R/W  ROUTING_AND15_INPUT_GEN_EN              |
 | +---------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND15_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND15_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND15_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND15_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND15_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND15_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND15_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND15_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------------------+
 | | Register (0x0000018c + (N) * 512) ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW |
 | |     N[0..11]                                                                |
 | +-----------------------------------------------------------------------------+
 | | bit  31:16 R/W  ROUTING_AND_MPLS_OVERFLOW_EN                                |
 | | bit  15:0  R/W  ROUTING_AND_NO_MATCH_EN                                     |
 | +-----------------------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_AND_MPLS_OVERFLOW_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND_MPLS_OVERFLOW_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND_MPLS_OVERFLOW_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND_MPLS_OVERFLOW_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_AND_NO_MATCH_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_AND_NO_MATCH_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_AND_NO_MATCH_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_AND_NO_MATCH_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001a0 + (N) * 512) ROUTING_OR_FILTER_0_REG1 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_OR0_INPUT_VLANMPLS_EN              |
 | | bit  25:0  R/W  ROUTING_OR0_INPUT_GEN_EN                   |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR0_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR0_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR0_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR0_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_OR0_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR0_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR0_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR0_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001a4 + (N) * 512) ROUTING_OR_FILTER_0_REG2 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31   R/W  ROUTING_OR0_OUTPUT_EN                       |
 | | bit  28   R/W  ROUTING_OR0_OUTPUT_INV                      |
 | | bit  20   R/W  ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN          |
 | | bit  16   R/W  ROUTING_OR0_INPUT_NO_MATCH_ALL_EN           |
 | | bit  15:0 R/W  ROUTING_OR0_INPUT_ANDOUT_EN                 |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR0_OUTPUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR0_OUTPUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR0_OUTPUT_INV_set             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR0_OUTPUT_INV_get             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR0_INPUT_MPLS_OVERFLOW_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR0_INPUT_NO_MATCH_ALL_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR0_INPUT_NO_MATCH_ALL_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR0_INPUT_ANDOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR0_INPUT_ANDOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR0_INPUT_ANDOUT_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR0_INPUT_ANDOUT_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001a8 + (N) * 512) ROUTING_OR_FILTER_1_REG1 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_OR1_INPUT_VLANMPLS_EN              |
 | | bit  25:0  R/W  ROUTING_OR1_INPUT_GEN_EN                   |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR1_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR1_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR1_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR1_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_OR1_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR1_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR1_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR1_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001ac + (N) * 512) ROUTING_OR_FILTER_1_REG2 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31   R/W  ROUTING_OR1_OUTPUT_EN                       |
 | | bit  28   R/W  ROUTING_OR1_OUTPUT_INV                      |
 | | bit  20   R/W  ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN          |
 | | bit  16   R/W  ROUTING_OR1_INPUT_NO_MATCH_ALL_EN           |
 | | bit  15:0 R/W  ROUTING_OR1_INPUT_ANDOUT_EN                 |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR1_OUTPUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR1_OUTPUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR1_OUTPUT_INV_set             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR1_OUTPUT_INV_get             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR1_INPUT_MPLS_OVERFLOW_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR1_INPUT_NO_MATCH_ALL_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR1_INPUT_NO_MATCH_ALL_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR1_INPUT_ANDOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR1_INPUT_ANDOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR1_INPUT_ANDOUT_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR1_INPUT_ANDOUT_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001b0 + (N) * 512) ROUTING_OR_FILTER_2_REG1 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_OR2_INPUT_VLANMPLS_EN              |
 | | bit  25:0  R/W  ROUTING_OR2_INPUT_GEN_EN                   |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR2_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR2_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR2_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR2_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_OR2_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR2_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR2_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR2_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001b4 + (N) * 512) ROUTING_OR_FILTER_2_REG2 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31   R/W  ROUTING_OR2_OUTPUT_EN                       |
 | | bit  28   R/W  ROUTING_OR2_OUTPUT_INV                      |
 | | bit  20   R/W  ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN          |
 | | bit  16   R/W  ROUTING_OR2_INPUT_NO_MATCH_ALL_EN           |
 | | bit  15:0 R/W  ROUTING_OR2_INPUT_ANDOUT_EN                 |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR2_OUTPUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR2_OUTPUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR2_OUTPUT_INV_set             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR2_OUTPUT_INV_get             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR2_INPUT_MPLS_OVERFLOW_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR2_INPUT_NO_MATCH_ALL_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR2_INPUT_NO_MATCH_ALL_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR2_INPUT_ANDOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR2_INPUT_ANDOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR2_INPUT_ANDOUT_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR2_INPUT_ANDOUT_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001b8 + (N) * 512) ROUTING_OR_FILTER_3_REG1 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_OR3_INPUT_VLANMPLS_EN              |
 | | bit  25:0  R/W  ROUTING_OR3_INPUT_GEN_EN                   |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR3_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR3_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR3_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR3_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_OR3_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR3_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR3_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR3_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001bc + (N) * 512) ROUTING_OR_FILTER_3_REG2 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31   R/W  ROUTING_OR3_OUTPUT_EN                       |
 | | bit  28   R/W  ROUTING_OR3_OUTPUT_INV                      |
 | | bit  20   R/W  ROUTING_OR3_INPUT_MPLS_OVERFLOW_EN          |
 | | bit  16   R/W  ROUTING_OR3_INPUT_NO_MATCH_ALL_EN           |
 | | bit  15:0 R/W  ROUTING_OR3_INPUT_ANDOUT_EN                 |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR3_OUTPUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR3_OUTPUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR3_OUTPUT_INV_set             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR3_OUTPUT_INV_get             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR3_INPUT_MPLS_OVERFLOW_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR3_INPUT_MPLS_OVERFLOW_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR3_INPUT_NO_MATCH_ALL_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR3_INPUT_NO_MATCH_ALL_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR3_INPUT_ANDOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR3_INPUT_ANDOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR3_INPUT_ANDOUT_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR3_INPUT_ANDOUT_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001c0 + (N) * 512) ROUTING_OR_FILTER_4_REG1 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_OR4_INPUT_VLANMPLS_EN              |
 | | bit  25:0  R/W  ROUTING_OR4_INPUT_GEN_EN                   |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR4_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR4_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR4_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR4_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_OR4_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR4_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR4_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR4_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001c4 + (N) * 512) ROUTING_OR_FILTER_4_REG2 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31   R/W  ROUTING_OR4_OUTPUT_EN                       |
 | | bit  28   R/W  ROUTING_OR4_OUTPUT_INV                      |
 | | bit  20   R/W  ROUTING_OR4_INPUT_MPLS_OVERFLOW_EN          |
 | | bit  16   R/W  ROUTING_OR4_INPUT_NO_MATCH_ALL_EN           |
 | | bit  15:0 R/W  ROUTING_OR4_INPUT_ANDOUT_EN                 |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR4_OUTPUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR4_OUTPUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR4_OUTPUT_INV_set             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR4_OUTPUT_INV_get             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR4_INPUT_MPLS_OVERFLOW_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR4_INPUT_MPLS_OVERFLOW_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR4_INPUT_NO_MATCH_ALL_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR4_INPUT_NO_MATCH_ALL_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR4_INPUT_ANDOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR4_INPUT_ANDOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR4_INPUT_ANDOUT_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR4_INPUT_ANDOUT_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001c8 + (N) * 512) ROUTING_OR_FILTER_5_REG1 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31:26 R/W  ROUTING_OR5_INPUT_VLANMPLS_EN              |
 | | bit  25:0  R/W  ROUTING_OR5_INPUT_GEN_EN                   |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR5_INPUT_VLANMPLS_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR5_INPUT_VLANMPLS_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR5_INPUT_VLANMPLS_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR5_INPUT_VLANMPLS_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_ROUTING_OR5_INPUT_GEN_EN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR5_INPUT_GEN_EN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR5_INPUT_GEN_EN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR5_INPUT_GEN_EN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000001cc + (N) * 512) ROUTING_OR_FILTER_5_REG2 |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  31   R/W  ROUTING_OR5_OUTPUT_EN                       |
 | | bit  28   R/W  ROUTING_OR5_OUTPUT_INV                      |
 | | bit  20   R/W  ROUTING_OR5_INPUT_MPLS_OVERFLOW_EN          |
 | | bit  16   R/W  ROUTING_OR5_INPUT_NO_MATCH_ALL_EN           |
 | | bit  15:0 R/W  ROUTING_OR5_INPUT_ANDOUT_EN                 |
 | +------------------------------------------------------------+
 | 
 | void   eclass_field_ROUTING_OR5_OUTPUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR5_OUTPUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR5_OUTPUT_INV_set             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR5_OUTPUT_INV_get             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR5_INPUT_MPLS_OVERFLOW_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR5_INPUT_MPLS_OVERFLOW_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR5_INPUT_NO_MATCH_ALL_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR5_INPUT_NO_MATCH_ALL_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ROUTING_OR5_INPUT_ANDOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ROUTING_OR5_INPUT_ANDOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_ROUTING_OR5_INPUT_ANDOUT_EN_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_ROUTING_OR5_INPUT_ANDOUT_EN_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x000001f0 + (N) * 512) LINK_ECO |
 | |     N[0..11]                               |
 | +--------------------------------------------+
 | | bit  31:0 R/W  LINK_ECO                    |
 | +--------------------------------------------+
 | 
 | void   eclass_field_LINK_ECO_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_LINK_ECO_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_LINK_ECO_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_LINK_ECO_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00001b00 + (N) * 4) GEN_CFG_HWIRED_FILTER |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  31  R/W  ONE_WORD_GENCLASS_EN                    |
 | | bit  28  R/W  ADDR_IPV6_P2P_UCAST_EN                  |
 | | bit  24  R/W  ADDR_IPV6_UCAST_EN                      |
 | | bit  20  R/W  ADDR_IPV4_P2P_UCAST_EN                  |
 | | bit  16  R/W  ADDR_IPV4_UCAST_EN                      |
 | | bit  12  R/W  USE_UNICAST_NDP                         |
 | | bit  7:4 R/W  PTP_VER_VALUE                           |
 | | bit  0   R/W  PTP_VER_CHECK_EN                        |
 | +-------------------------------------------------------+
 | 
 | void   eclass_field_ONE_WORD_GENCLASS_EN_set   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ONE_WORD_GENCLASS_EN_get   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ADDR_IPV6_P2P_UCAST_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_P2P_UCAST_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ADDR_IPV6_UCAST_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV6_UCAST_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ADDR_IPV4_P2P_UCAST_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV4_P2P_UCAST_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_ADDR_IPV4_UCAST_EN_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_ADDR_IPV4_UCAST_EN_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_USE_UNICAST_NDP_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_USE_UNICAST_NDP_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_PTP_VER_VALUE_set          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_PTP_VER_VALUE_get          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_PTP_VER_VALUE_set    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_PTP_VER_VALUE_get    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_PTP_VER_CHECK_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_PTP_VER_CHECK_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002000 + (N) * 128) GEN_FILTER_CFG_FILTER16_0_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_0_MASK                                        |
 | | bit  15:0  R/W  F16_0_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_0_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_0_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_0_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_0_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_0_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_0_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_0_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_0_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002004 + (N) * 128) GEN_FILTER_CFG_FILTER16_0_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_0_INV                                           |
 | | bit  8   R/W  F16_0_ENABLE                                        |
 | | bit  4:0 R/W  F16_0_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_0_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_0_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_0_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_0_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_0_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_0_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_0_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_0_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002008 + (N) * 128) GEN_FILTER_CFG_FILTER16_1_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_1_MASK                                        |
 | | bit  15:0  R/W  F16_1_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_1_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_1_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_1_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_1_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_1_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_1_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_1_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_1_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x0000200c + (N) * 128) GEN_FILTER_CFG_FILTER16_1_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_1_INV                                           |
 | | bit  8   R/W  F16_1_ENABLE                                        |
 | | bit  4:0 R/W  F16_1_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_1_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_1_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_1_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_1_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_1_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_1_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_1_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_1_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002010 + (N) * 128) GEN_FILTER_CFG_FILTER16_2_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_2_MASK                                        |
 | | bit  15:0  R/W  F16_2_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_2_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_2_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_2_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_2_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_2_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_2_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_2_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_2_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002014 + (N) * 128) GEN_FILTER_CFG_FILTER16_2_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_2_INV                                           |
 | | bit  8   R/W  F16_2_ENABLE                                        |
 | | bit  4:0 R/W  F16_2_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_2_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_2_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_2_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_2_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_2_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_2_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_2_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_2_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002018 + (N) * 128) GEN_FILTER_CFG_FILTER16_3_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_3_MASK                                        |
 | | bit  15:0  R/W  F16_3_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_3_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_3_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_3_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_3_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_3_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_3_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_3_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_3_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x0000201c + (N) * 128) GEN_FILTER_CFG_FILTER16_3_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_3_INV                                           |
 | | bit  8   R/W  F16_3_ENABLE                                        |
 | | bit  4:0 R/W  F16_3_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_3_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_3_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_3_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_3_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_3_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_3_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_3_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_3_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002020 + (N) * 128) GEN_FILTER_CFG_FILTER16_4_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_4_MASK                                        |
 | | bit  15:0  R/W  F16_4_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_4_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_4_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_4_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_4_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_4_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_4_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_4_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_4_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002024 + (N) * 128) GEN_FILTER_CFG_FILTER16_4_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_4_INV                                           |
 | | bit  8   R/W  F16_4_ENABLE                                        |
 | | bit  4:0 R/W  F16_4_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_4_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_4_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_4_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_4_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_4_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_4_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_4_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_4_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002028 + (N) * 128) GEN_FILTER_CFG_FILTER16_5_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_5_MASK                                        |
 | | bit  15:0  R/W  F16_5_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_5_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_5_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_5_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_5_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_5_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_5_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_5_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_5_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x0000202c + (N) * 128) GEN_FILTER_CFG_FILTER16_5_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_5_INV                                           |
 | | bit  8   R/W  F16_5_ENABLE                                        |
 | | bit  4:0 R/W  F16_5_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_5_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_5_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_5_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_5_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_5_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_5_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_5_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_5_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002030 + (N) * 128) GEN_FILTER_CFG_FILTER16_6_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_6_MASK                                        |
 | | bit  15:0  R/W  F16_6_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_6_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_6_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_6_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_6_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_6_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_6_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_6_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_6_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002034 + (N) * 128) GEN_FILTER_CFG_FILTER16_6_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_6_INV                                           |
 | | bit  8   R/W  F16_6_ENABLE                                        |
 | | bit  4:0 R/W  F16_6_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_6_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_6_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_6_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_6_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_6_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_6_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_6_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_6_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002038 + (N) * 128) GEN_FILTER_CFG_FILTER16_7_REG_1 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:16 R/W  F16_7_MASK                                        |
 | | bit  15:0  R/W  F16_7_VALUE                                       |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_7_MASK_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_7_MASK_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_7_MASK_set  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_7_MASK_get  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_F16_7_VALUE_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_7_VALUE_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_7_VALUE_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_7_VALUE_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x0000203c + (N) * 128) GEN_FILTER_CFG_FILTER16_7_REG_2 |
 | |     N[0..23]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  12  R/W  F16_7_INV                                           |
 | | bit  8   R/W  F16_7_ENABLE                                        |
 | | bit  4:0 R/W  F16_7_ADDR                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   eclass_field_F16_7_INV_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_7_INV_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_7_ENABLE_set     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_7_ENABLE_get     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_F16_7_ADDR_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_F16_7_ADDR_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_F16_7_ADDR_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_F16_7_ADDR_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00002c00 + (N) * 4) FOUT_COMB_ORIN_1 |
 | |     N[0..15]                                     |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  COMB_ORIN_HWIRED_FOUT_EN_LSB      |
 | +--------------------------------------------------+
 | 
 | void   eclass_lfield_COMB_ORIN_HWIRED_FOUT_EN_LSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value[16] )
 | void   eclass_lfield_COMB_ORIN_HWIRED_FOUT_EN_LSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value[16] )
 | void   eclass_lfield_range_COMB_ORIN_HWIRED_FOUT_EN_LSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_lfield_range_COMB_ORIN_HWIRED_FOUT_EN_LSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00002c50 + (N) * 4) FOUT_COMB_ORIN_2 |
 | |     N[0..15]                                     |
 | +--------------------------------------------------+
 | | bit  31    R/W  COMB_ORIN_OUT_INV                |
 | | bit  30:23 R/W  COMB_ORIN_CFG_FOUT_EN            |
 | | bit  17:0  R/W  COMB_ORIN_HWIRED_FOUT_EN_MSB     |
 | +--------------------------------------------------+
 | 
 | void   eclass_field_COMB_ORIN_OUT_INV_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_ORIN_OUT_INV_get                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_COMB_ORIN_CFG_FOUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_ORIN_CFG_FOUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_COMB_ORIN_CFG_FOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_COMB_ORIN_CFG_FOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_ORIN_HWIRED_FOUT_EN_MSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_COMB_ORIN_HWIRED_FOUT_EN_MSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_COMB_ORIN_HWIRED_FOUT_EN_MSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00002ca0 + (N) * 4) FOUT_COMB_AND_1 |
 | |     N[0..39]                                    |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  COMB_AND_HWIRED_FOUT_EN_LSB      |
 | +-------------------------------------------------+
 | 
 | void   eclass_lfield_COMB_AND_HWIRED_FOUT_EN_LSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value[40] )
 | void   eclass_lfield_COMB_AND_HWIRED_FOUT_EN_LSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value[40] )
 | void   eclass_lfield_range_COMB_AND_HWIRED_FOUT_EN_LSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_lfield_range_COMB_AND_HWIRED_FOUT_EN_LSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00002d50 + (N) * 4) FOUT_COMB_AND_2 |
 | |     N[0..39]                                    |
 | +-------------------------------------------------+
 | | bit  31:24 R/W  COMB_AND_CFG_FOUT_EN            |
 | | bit  17:0  R/W  COMB_AND_HWIRED_FOUT_EN_MSB     |
 | +-------------------------------------------------+
 | 
 | void   eclass_field_COMB_AND_CFG_FOUT_EN_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_AND_CFG_FOUT_EN_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_COMB_AND_CFG_FOUT_EN_set        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_COMB_AND_CFG_FOUT_EN_get        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_AND_HWIRED_FOUT_EN_MSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_COMB_AND_HWIRED_FOUT_EN_MSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_COMB_AND_HWIRED_FOUT_EN_MSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00002e00 + (N) * 4) FOUT_COMB_AND_3 |
 | |     N[0..39]                                    |
 | +-------------------------------------------------+
 | | bit  31   R/W  COMB_AND_OUT_INV                 |
 | | bit  15:0 R/W  COMB_AND_FILTORIN_EN             |
 | +-------------------------------------------------+
 | 
 | void   eclass_field_COMB_AND_OUT_INV_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_AND_OUT_INV_get           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_COMB_AND_FILTORIN_EN_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_AND_FILTORIN_EN_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_COMB_AND_FILTORIN_EN_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_COMB_AND_FILTORIN_EN_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00002eb0 + (N) * 4) FOUT_COMB_OROUT_1 |
 | |     N[0..25]                                      |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  COMB_OROUT_FILTAND_EN_LSB          |
 | +---------------------------------------------------+
 | 
 | void   eclass_lfield_COMB_OROUT_FILTAND_EN_LSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value[26] )
 | void   eclass_lfield_COMB_OROUT_FILTAND_EN_LSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value[26] )
 | void   eclass_lfield_range_COMB_OROUT_FILTAND_EN_LSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_lfield_range_COMB_OROUT_FILTAND_EN_LSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00002f30 + (N) * 4) FOUT_COMB_OROUT_2 |
 | |     N[0..25]                                      |
 | +---------------------------------------------------+
 | | bit  31    R/W  COMB_OROUT_OUT_EN                 |
 | | bit  30    R/W  COMB_OROUT_OUT_INV                |
 | | bit  23:16 R/W  COMB_OROUT_FILTAND_EN_MSB         |
 | | bit  7:0   R/W  COMB_OROUT_CFG_FOUT_EN            |
 | +---------------------------------------------------+
 | 
 | void   eclass_field_COMB_OROUT_OUT_EN_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_OROUT_OUT_EN_get               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_COMB_OROUT_OUT_INV_set              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_OROUT_OUT_INV_get              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_COMB_OROUT_FILTAND_EN_MSB_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_OROUT_FILTAND_EN_MSB_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_COMB_OROUT_FILTAND_EN_MSB_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_COMB_OROUT_FILTAND_EN_MSB_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_COMB_OROUT_CFG_FOUT_EN_set          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 eclass_field_COMB_OROUT_CFG_FOUT_EN_get          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | void   eclass_field_range_COMB_OROUT_CFG_FOUT_EN_set    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_COMB_OROUT_CFG_FOUT_EN_get    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00003900 NO_MATCH_EN      |
 | +--------------------------------------+
 | | bit  31:26 R/W  NO_MATCH_EN_VLANMPLS |
 | | bit  25:0  R/W  NO_MATCH_EN_GEN      |
 | +--------------------------------------+
 | 
 | void   eclass_field_NO_MATCH_EN_VLANMPLS_set       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_NO_MATCH_EN_VLANMPLS_get       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_range_NO_MATCH_EN_VLANMPLS_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_NO_MATCH_EN_VLANMPLS_get ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   eclass_field_NO_MATCH_EN_GEN_set            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | UINT32 eclass_field_NO_MATCH_EN_GEN_get            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void   eclass_field_range_NO_MATCH_EN_GEN_set      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 eclass_field_range_NO_MATCH_EN_GEN_get      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ECLASS_IO_H
#define _ECLASS_IO_H

#include "eclass_loc.h"
#include "eclass_regs.h"
#include "eclass_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 eclass_buffer_init               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | void                 eclass_buffer_flush              ( eclass_buffer_t *b_ptr )
 | UINT32               eclass_reg_read                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 eclass_reg_write                 ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 eclass_field_set                 ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 eclass_action_on_write_field_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 eclass_burst_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 eclass_burst_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE eclass_poll                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   eclass_reg_ECLASS120_LINK_EN_write                                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | void   eclass_reg_ECLASS120_LINK_EN_field_set                               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ECLASS120_LINK_EN_read                                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | 
 | void   eclass_reg_NO_MATCH_EN_write                                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 value )
 | void   eclass_reg_NO_MATCH_EN_field_set                                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_NO_MATCH_EN_read                                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr )
 | 
 | void   eclass_reg_FOUT_COMB_ORIN_1_array_burst_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   eclass_reg_FOUT_COMB_ORIN_1_array_field_set                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   eclass_reg_FOUT_COMB_ORIN_1_array_burst_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 eclass_reg_FOUT_COMB_ORIN_1_array_read                               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 N )
 | 
 | void   eclass_reg_FOUT_COMB_OROUT_1_array_burst_write                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   eclass_reg_FOUT_COMB_OROUT_1_array_field_set                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   eclass_reg_FOUT_COMB_OROUT_1_array_burst_read                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 eclass_reg_FOUT_COMB_OROUT_1_array_read                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 N )
 | 
 | void   eclass_reg_FOUT_COMB_AND_1_array_burst_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   eclass_reg_FOUT_COMB_AND_1_array_field_set                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   eclass_reg_FOUT_COMB_AND_1_array_burst_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 eclass_reg_FOUT_COMB_AND_1_array_read                                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32 N )
 | 
 | void   eclass_reg_GEN_CFG_HWIRED_FILTER_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_CFG_HWIRED_FILTER_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_CFG_HWIRED_FILTER_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_FOUT_COMB_ORIN_2_array_write                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_FOUT_COMB_ORIN_2_array_field_set                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_FOUT_COMB_ORIN_2_array_read                               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_FOUT_COMB_OROUT_2_array_write                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_FOUT_COMB_OROUT_2_array_field_set                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_FOUT_COMB_OROUT_2_array_read                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_FOUT_COMB_AND_2_array_write                               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_FOUT_COMB_AND_2_array_field_set                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_FOUT_COMB_AND_2_array_read                                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_FOUT_COMB_AND_3_array_write                               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_FOUT_COMB_AND_3_array_field_set                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_FOUT_COMB_AND_3_array_read                                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_CFG_VLAN_MPLS_array_write                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_CFG_VLAN_MPLS_array_field_set                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_CFG_VLAN_MPLS_array_read                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLAN_TPID_array_write                                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLAN_TPID_array_field_set                                 ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLAN_TPID_array_read                                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_MPLS_ETHERTYPE_array_write                                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_MPLS_ETHERTYPE_array_field_set                            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_MPLS_ETHERTYPE_array_read                                 ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG0_1_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG0_1_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG0_1_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG0_2_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG0_2_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG0_2_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG1_1_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG1_1_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG1_1_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG1_2_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG1_2_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG1_2_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG2_1_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG2_1_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG2_1_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG2_2_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG2_2_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG2_2_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG3_1_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG3_1_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG3_1_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG3_2_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG3_2_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG3_2_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG4_1_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG4_1_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG4_1_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG4_2_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG4_2_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG4_2_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG5_1_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG5_1_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG5_1_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_VLANMPLS_FILT20_TAG5_2_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_VLANMPLS_FILT20_TAG5_2_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_VLANMPLS_FILT20_TAG5_2_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_MAN_VLAN_CFG_array_write                                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_MAN_VLAN_CFG_array_field_set                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_MAN_VLAN_CFG_array_read                                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_ETH_LSB_array_write                            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_ETH_LSB_array_field_set                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_ETH_LSB_array_read                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_ETH_MSB_array_write                            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_ETH_MSB_array_field_set                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_ETH_MSB_array_read                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_ETH_P2P_LSB_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_ETH_P2P_LSB_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_ETH_P2P_LSB_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_ETH_P2P_MSB_array_write                        ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_ETH_P2P_MSB_array_field_set                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_ETH_P2P_MSB_array_read                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DEST_PORT_UDP_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DEST_PORT_UDP_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DEST_PORT_UDP_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV4_array_write                               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV4_array_field_set                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV4_array_read                                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV4_P2P_array_write                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV4_P2P_array_field_set                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV4_P2P_array_read                            ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_0_array_write                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_0_array_field_set                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_0_array_read                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_1_array_write                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_1_array_field_set                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_1_array_read                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_2_array_write                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_2_array_field_set                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_2_array_read                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_3_array_write                             ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_3_array_field_set                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_3_array_read                              ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_0_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_0_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_P2P_0_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_1_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_1_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_P2P_1_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_2_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_2_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_P2P_2_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_3_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_UNICAST_DA_IPV6_P2P_3_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_UNICAST_DA_IPV6_P2P_3_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FAND_0_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FAND_0_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FAND_0_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FAND_1_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FAND_1_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FAND_1_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FAND_2_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FAND_2_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FAND_2_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FAND_3_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FAND_3_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FAND_3_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_0_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_0_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_0_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_1_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_1_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_1_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_2_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_2_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_2_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_3_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_3_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_3_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_4_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_4_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_4_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_5_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_5_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_5_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_6_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_6_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_6_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FOR_7_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FOR_7_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FOR_7_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_0_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_0_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_0_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_1_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_1_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_1_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_2_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_2_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_2_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_3_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_3_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_3_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_4_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_4_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_4_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_5_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_5_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_5_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_6_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_6_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_6_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_7_array_write                   ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER_OUT_7_array_field_set               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER_OUT_7_array_read                    ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_0_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_0_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_0_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_1_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_1_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_1_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_2_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_2_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_2_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_3_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_3_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_3_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_4_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_4_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_4_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_5_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_5_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_5_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_6_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_6_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_6_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_7_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_7_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_7_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_8_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_8_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_8_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_9_array_write                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_9_array_field_set                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_9_array_read                           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_10_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_10_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_10_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_11_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_11_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_11_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_12_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_12_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_12_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_13_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_13_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_13_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_14_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_14_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_14_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_15_array_write                         ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_15_array_field_set                     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_15_array_read                          ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_array_write     ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_array_field_set ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_AND_FILTER_NO_MATCH_MPLS_OVERFLOW_array_read      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_0_REG1_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_0_REG1_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_0_REG1_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_0_REG2_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_0_REG2_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_0_REG2_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_1_REG1_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_1_REG1_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_1_REG1_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_1_REG2_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_1_REG2_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_1_REG2_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_2_REG1_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_2_REG1_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_2_REG1_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_2_REG2_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_2_REG2_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_2_REG2_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_3_REG1_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_3_REG1_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_3_REG1_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_3_REG2_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_3_REG2_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_3_REG2_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_4_REG1_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_4_REG1_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_4_REG1_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_4_REG2_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_4_REG2_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_4_REG2_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_5_REG1_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_5_REG1_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_5_REG1_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_ROUTING_OR_FILTER_5_REG2_array_write                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_ROUTING_OR_FILTER_5_REG2_array_field_set                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_ROUTING_OR_FILTER_5_REG2_array_read                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_LINK_ECO_array_write                                      ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_LINK_ECO_array_field_set                                  ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_LINK_ECO_array_read                                       ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_0_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_1_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_2_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_3_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_4_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_5_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_6_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_1_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_1_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_1_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_2_array_write               ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_2_array_field_set           ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 eclass_reg_GEN_FILTER_CFG_FILTER16_7_REG_2_array_read                ( eclass_buffer_t *b_ptr, eclass_handle_t *h_ptr, UINT32  N )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ECLASS_IO_H */
