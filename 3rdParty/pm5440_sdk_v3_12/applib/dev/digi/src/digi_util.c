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
*   DESCRIPTION : This file contains C functions for printing interrupt tables
*   and handle states for debugging purposes. 
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "digi_loc.h" /* This file here so not all digi_loc.h sub-files included */
#include "digi_util_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
#define STR1(x) #x
#define STR(x)  STR1(x)

#define DIGI_UTIL_INT_SINGLE_FIELD                                      1
#define DIGI_UTIL_INT_STR_BUF_LEN                                       100 
#define DIGI_UTIL_INT_IS_PRINT_ALL(print_type)  (DIGI_UTIL_INT_PRINT_ALL == print_type)
#define DIGI_UTIL_INT_IS_PRINT_NONE(print_type) (DIGI_UTIL_INT_PRINT_NONE == print_type)
#define DIGI_UTIL_PRINT(...) \
do {                                                                            \
    if (FALSE == DIGI_UTIL_INT_IS_PRINT_NONE(print_type)) {                     \
        printf(__VA_ARGS__);                                                    \
    }                                                                           \
} while(0)

#define DIGI_UTIL_PRINT_DIVIDER() \
    DIGI_UTIL_PRINT("=============================================================================================\n");

#define DIGI_UTIL_PRINT_SUBDIVIDER() \
    DIGI_UTIL_PRINT("----------------------------------\n");

#define DIGI_UTIL_INT_PRINT_SPACER(num_lines) \
do {                                                                            \
    int i;                                                                      \
    if (digi_util_int_cur_column != 0) {                                        \
        DIGI_UTIL_INT_PRINT_NEW_LINE();                                         \
    }                                                                           \
    for (i = 0; i < num_lines; i++) {                                           \
        DIGI_UTIL_INT_PRINT_NEW_LINE();                                         \
    }                                                                           \
} while (0)
        
#define DIGI_UTIL_INT_PRINT_NEW_LINE() \
do {                                                                            \
    DIGI_UTIL_PRINT("\n");                                                      \
    digi_util_int_cur_column = 0;                                               \
} while(0)

#define DIGI_UTIL_INT_PRINT_NEW_LINE_CHECK() \
do {                                                                            \
    digi_util_int_cur_column++;                                                 \
    if (digi_util_int_cur_column >= format_ptr->num_columns) {                  \
        DIGI_UTIL_INT_PRINT_NEW_LINE();                                         \
    } else {                                                                    \
        DIGI_UTIL_PRINT("%2s", "");                                             \
    }                                                                           \
} while(0)


#define DIGI_UTIL_INT_PRINT(handle, tsb_name, tsb_field, struct_field) \
    digi_util_int_field_print(digi_handle,                                      \
                              &int_table_ptr->struct_field,                     \
                              DIGI_UTIL_INT_SINGLE_FIELD,                       \
                              (char*) STR(struct_field),                        \
                              NULL,                                             \
                              print_type,                                       \
                              format_ptr,                                       \
                              int_found_ptr);                                     



#define DIGI_UTIL_INT_PRINT_SINGLE(handle, tsb_name, tsb_field, struct_field) \
    DIGI_UTIL_INT_PRINT(handle, tsb_name, tsb_field, struct_field);

#define DIGI_UTIL_INT_PRINT_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) \
    DIGI_UTIL_INT_PRINT(handle, tsb_name, tsb_field, struct_field);


#define DIGI_UTIL_INT_PRINT_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
    digi_util_int_field_print(digi_handle,                                      \
                              int_table_ptr->struct_field,                      \
                              array_len,                                        \
                              (char*) STR(struct_field),                        \
                              (char*) STR(array_len),                           \
                              print_type,                                       \
                              format_ptr,                                       \
                              int_found_ptr);                                     

#define DIGI_UTIL_INT_PRINT_ARRAY_RANGE(handle, tsb_name, field_type, array_len, tsb_field, struct_field) \
    DIGI_UTIL_INT_PRINT_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field)

#define DIGI_UTIL_INT_PRINT_ARRAY_SINGLE(handle, tsb_name, array_len, tsb_field, struct_field) \
    DIGI_UTIL_INT_PRINT_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field)


#define DIGI_UTIL_INT_PRINT_ODU_RFRM(handle, tsb_name, tsb_field, tsb_comb_field, struct_field) \
    DIGI_UTIL_INT_PRINT(handle, tsb_name, tsb_field, struct_field);


#define DIGI_UTIL_INT_PRINT_LINEOTN_FIELD(handle, tsb_name, array_len, tsb_field, struct_field) \
    digi_util_int_field_print(digi_handle,                                      \
                              int_table_ptr->struct_field,                      \
                              array_len,                                        \
                              (char*) STR(struct_field),                        \
                              (char*) STR(array_len),                           \
                              print_type,                                       \
                              format_ptr,                                       \
                              int_found_ptr);                                     

/*
** Global Variables
*/
PRIVATE char str_buf[DIGI_UTIL_INT_STR_BUF_LEN];  /*!< string buffer to store interrupt string */
PRIVATE UINT32 digi_util_int_cur_column = 0;  /*!< Track current column when printing interrupts */
PRIVATE unsigned long Table_CRC[256]; /*!< CRC lookup table */

/*
** Local Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: digi_util_odu_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Local structure used to output ODU info
*
* ELEMENTS:
*   See typedef
*******************************************************************************/
typedef struct digi_util_odu_info_t 
{
    char                        *name;  /*!< ODU name */
    digi_otn_odu_chnl_t        **chnl_handle; /*!< Channel info for the odu channel */
    UINT32                       max_chnl; /*!< Max channel */
    digi_otn_odu_int_chnl_t     *int_table_ptr; /*!< See digi_otn_odu_int_chnl_t */
} digi_util_odu_info_t;


/*
** Local Variables
*/

/*
** Private Functions
*/

PRIVATE void digi_debug_generic_handle_state_print(util_global_switch_data_def_t* hndl_chnl_ptr, UINT32 index, BOOL print_spacer);
PRIVATE void digi_debug_generic_handle_state_header_print();
PRIVATE void digi_debug_generic_handle_state_spacer_print();
PRIVATE void digi_debug_generic_handle_state_footer_print();
PRIVATE void digi_debug_generic_odu_struct_state_print(odu_struct_t* odu_struct_ptr, UINT32 index);
PRIVATE void digi_debug_generic_odu_struct_state_header_print();
PRIVATE void digi_debug_generic_enet_handle_state_print(digi_handle_t* digi_handle, digi_enet_client_chnl_def_t* enet_handle_ptr, UINT32 index, util_global_cpb_port_t port_type);
PRIVATE void digi_debug_generic_enet_handle_state_header_print();
PRIVATE void digi_debug_generic_mapper_handle_state_print(digi_mapper_chnl_def_t* map_handle_ptr, UINT32 index);
PRIVATE void digi_debug_generic_mapper_handle_state_header_print();
PRIVATE void digi_debug_generic_ilkn_handle_state_print(digi_ilkn_chnl_def_t* map_handle_ptr, UINT32 index);
PRIVATE void digi_debug_generic_ilkn_handle_state_header_print();
PRIVATE BOOL digi_util_int_field_is_active(digi_handle_t    *digi_handle,
                                           BOOL8            *field_val_ptr,
                                           UINT32            array_len);
PRIVATE void digi_util_int_field_print(digi_handle_t            *digi_handle,
                                       BOOL8                    *field_val_ptr,
                                       UINT32                    array_len,
                                       char                     *field_name_str_ptr,
                                       char                     *array_len_str_ptr,
                                       digi_util_int_print_t     print_type,
                                       digi_util_int_format_t   *format_ptr,
                                       BOOL                     *int_found_ptr);


PUBLIC PMC_ERROR digi_util_int_otu_frm_chnl_print(digi_handle_t             *digi_handle,
                                                  otu_frm_int_chnl_t        *int_table_ptr,
                                                  digi_util_int_print_t      print_type,
                                                  digi_util_int_format_t    *format_ptr,
                                                  BOOL                      *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_rgmp_chnl_print(digi_handle_t            *digi_handle,
                                               rgmp_int_chnl_t          *int_table_ptr,
                                               digi_util_int_print_t     print_type,
                                               digi_util_int_format_t   *format_ptr,
                                               BOOL                     *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_coreotn_framer_chnl_print(digi_handle_t                *digi_handle,
                                                         coreotn_int_framer_chnl_t    *int_table_ptr,
                                                         digi_util_int_print_t         print_type,
                                                         digi_util_int_format_t       *format_ptr,
                                                         BOOL                         *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_coreotn_chnl_print(digi_handle_t            *digi_handle,
                                                  coreotn_int_chnl_t       *int_table_ptr,
                                                  digi_util_int_print_t     print_type,
                                                  digi_util_int_format_t   *format_ptr,
                                                  BOOL                     *int_found_ptr);
PUBLIC PMC_ERROR digi_util_int_sfis_chnl_print(digi_handle_t            *digi_handle,
                                               lineotn_sfis_int_chnl_t  *int_table_ptr,
                                               digi_util_int_print_t     print_type,
                                               digi_util_int_format_t   *format_ptr,
                                               BOOL                     *int_found_ptr);
PRIVATE UINT16 digi_util_crc_16( UINT8 *aData, UINT32 aSize );
/*
** Public Functions
*/
/* LCOV_EXCL_START */

/*******************************************************************************
* digi_debug_dsi_alloc_config_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing digi handle dsi allocation fields.
*
* INPUTS:
*   digi_handle - a pointer to the specific digi handle instance.
*   config_name - configuration name to be output to screen
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
PUBLIC void digi_debug_dsi_alloc_config_print(digi_handle_t* digi_handle, const char *config_name)
{
#ifndef LOGGING_DISABLED
    UINT8 i;
    UINT8 k;

    char  bus_name[][7] = {"bus 0","bus 1"};
    char  prov_state_name[][11] = {"NO_SERDES", "UNASSIGNED", "LINEOTN",
                                   "ENET_LINE", "CBRC", "ENET_SYS", "LAST"};

    char  map_type_name[][16] = {"UNUSED", "ENET_LINE", "W/O_ENET_LINE",
                                 "WITH_ENET_LINE", "AND_ENET_LINE", "LAST"};
#endif
    PMC_ENTRY();
#ifndef LOGGING_DISABLED
    
    PMC_LOG_NOTRACE("\n---------------------- %s Start ----------------------\n",config_name);
    PMC_LOG_NOTRACE("\n=========line_port_ctxt=========|=========================xfi_line_pin_ctxt========================\n");
    PMC_LOG_NOTRACE("lane   init    port_uid   master  post mux lane    post mux port    cage id    tx_index    rx_index\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%4d   %4d    %8d     %4d  %13d    %13d    %7d    %8d    %8d\n",
                            i,
                            digi_handle->var.port_ctxt.line_port_ctxt[i].init,
                            digi_handle->var.port_ctxt.line_port_ctxt[i].port_uid,
                            digi_handle->var.port_ctxt.line_port_ctxt[i].master_lane,
                            digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].post_mux_dsi_lane,
                            digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].port_uid,
                            digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].cage_id,
                            digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_tx_index,
                            digi_handle->var.pin_bus_ctxt.xfi_line_pin_ctxt[i].module_lane_rx_index);
    }

    PMC_LOG_NOTRACE("\npost mld lane    =======post_mld_dsi_line_ctxt==========\n");                     
    PMC_LOG_NOTRACE("index            line post mld port    cage uid    prov state    map state    num ODU0s\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%13d    %18d    %8d    %10s    %9s    %9d\n",
                            i,
                            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].port_uid,
                            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].cage_uid,
                            prov_state_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].prov_state],
                            map_type_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].map_type],
                            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[i].num_odu0);
    }

    PMC_LOG_NOTRACE("\n==========sys_port_ctxt=========|==========================xfi_sys_pin_ctxt========================\n");
    PMC_LOG_NOTRACE("lane   init    port_uid   master  post mux lane    post mux port    cage id    tx_index    rx_index\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%4d   %4d    %8d     %4d  %13d    %13d    %7d    %8d    %8d\n",
                            i,
                            digi_handle->var.port_ctxt.sys_port_ctxt[i].init,
                            digi_handle->var.port_ctxt.sys_port_ctxt[i].port_uid,
                            digi_handle->var.port_ctxt.sys_port_ctxt[i].master_lane,
                            digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].post_mux_dsi_lane,
                            digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid,
                            digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].cage_id,
                            digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_tx_index,
                            digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].module_lane_rx_index);

    }

    PMC_LOG_NOTRACE("\npost mld lane    =======post_mld_dsi_line_ctxt==========\n");                     
    PMC_LOG_NOTRACE("index            line post mld port    cage uid    prov state    map state    num ODU0s\n");

    for(i=0;i<12;i++) {

        PMC_LOG_NOTRACE("%13d    %18d    %8d    %10s    %9s    %9d\n",
                            i,
                            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].port_uid,
                            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].cage_uid,
                            prov_state_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].prov_state],
                            map_type_name[digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].map_type],
                            digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].num_odu0);
    }

    PMC_LOG_NOTRACE("\n============== sfi5.1 config ==============\n");
    PMC_LOG_NOTRACE("cfg bus    init    allocated bus    sfi 5.1 post mld port\n");
    /* find the dsi lane on xfi that corresponds to sfi5.1 port id */
    for(i=0;i<2;i++)
    {
        for(k=0;k<12;k++)
            if(digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].port_uid ==
               digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[k].port_uid)
                break;

        PMC_LOG_NOTRACE("%7d    %4d    %13s    %21d\n",
                            i,
                            digi_handle->var.port_ctxt.sfi51_port_ctxt[i].init,
                            bus_name[digi_handle->var.pin_bus_ctxt.sfi51_bus_ctxt[i].post_mux_2G5_bus_allocation],
                            (k<12?digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[k].port_uid:DIGI_SERDES_UNASSIGNED));
    }

    PMC_LOG_NOTRACE("\n---------------------- %s  end ----------------------\n",config_name);
#endif
    PMC_RETURN();

} /* digi_debug_dsi_alloc_config_print */

/*******************************************************************************
* digi_debug_ilkn1_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing ilkn 1 handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_ilkn1_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX ) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[ILKN 1 HANDLE]=======================================================================================+\n");
            digi_debug_generic_ilkn_handle_state_header_print();
        }

        digi_debug_generic_ilkn_handle_state_print((digi_ilkn_chnl_def_t*)&digi_handle->handle_pool.ilkn_1_chnl[index], index);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_ilkn1_handle_state_print */

/*******************************************************************************
* digi_debug_ilkn1_handle_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the ilkn 1 handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_ilkn1_handle_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_ilkn1_handle_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_ilkn1_handle_state_all_print */

/*******************************************************************************
* digi_debug_ilkn2_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing ilkn 2 handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_ilkn2_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX ) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[ILKN 2 HANDLE]=======================================================================================+\n");
            digi_debug_generic_ilkn_handle_state_header_print();
        }

        digi_debug_generic_ilkn_handle_state_print((digi_ilkn_chnl_def_t*)&digi_handle->handle_pool.ilkn_2_chnl[index], index);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_ilkn2_handle_state_print */

/*******************************************************************************
* digi_debug_ilkn2_handle_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the ilkn 2 handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_ilkn2_handle_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_ilkn2_handle_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_ilkn2_handle_state_all_print */

/*******************************************************************************
* digi_debug_enet_line_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing enet line handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_enet_line_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_ENET_SYS_CHNL_MAX) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[ENET LINE HANDLE]====================================================================================+\n");
            digi_debug_generic_enet_handle_state_header_print();
        }

        digi_debug_generic_enet_handle_state_print(digi_handle, (digi_enet_client_chnl_def_t*)&digi_handle->handle_pool.enet_line_chnl[index], index, UTIL_GLOBAL_CPB_PORT_ENET_LINE);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_enet_line_handle_state_print */

/*******************************************************************************
* digi_debug_enet_line_handle_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the enet line handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_enet_line_handle_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();

    for (i = 0; i < DIGI_ENET_LINE_CHNL_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_ENET_LINE_CHNL_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_enet_line_handle_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_enet_line_handle_state_all_print */
    
/*******************************************************************************
* digi_debug_enet_sys_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing enet sys handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_enet_sys_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_ENET_SYS_CHNL_MAX) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[ENET SYS HANDLE]=====================================================================================+\n");
            digi_debug_generic_enet_handle_state_header_print();
        }

        digi_debug_generic_enet_handle_state_print(digi_handle, (digi_enet_client_chnl_def_t*)&digi_handle->handle_pool.enet_sys_chnl[index], index, UTIL_GLOBAL_CPB_PORT_ENET_SYS);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_enet_sys_handle_state_print */

/*******************************************************************************
* digi_debug_enet_sys_handle_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the enet sys handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_enet_sys_handle_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();


    for (i = 0; i < DIGI_ENET_SYS_CHNL_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_ENET_SYS_CHNL_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_enet_sys_handle_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_enet_sys_handle_state_all_print */

/*******************************************************************************
* digi_debug_mapper_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing mapper handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_mapper_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_MAPOTN_CHNL_MAX) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[MAPPER HANDLE]=======================================================================================+\n");
            digi_debug_generic_mapper_handle_state_header_print();
        }

        digi_debug_generic_mapper_handle_state_print((digi_mapper_chnl_def_t*)&digi_handle->handle_pool.mapper_chnl[index], index);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_mapper_handle_state_print */

/*******************************************************************************
* digi_debug_mapper_handle_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the mapper handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_mapper_handle_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();

    for (i = 0; i < DIGI_MAPOTN_CHNL_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_MAPOTN_CHNL_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_mapper_handle_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_mapper_handle_state_all_print */

/*******************************************************************************
* digi_debug_ho_odu_struct_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing ho odu stuct states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_ho_odu_struct_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_OTN_SERVER_HO_CHNL_MAX) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[HO ODU HANDLE]=======================================================================================+\n");
            digi_debug_generic_odu_struct_state_header_print();
        }

        digi_debug_generic_odu_struct_state_print(&digi_handle->handle_pool.ho_odu_struct[index], index);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_ho_odu_struct_state_print */

/*******************************************************************************
* digi_debug_ho_odu_struct_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the mapper handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_ho_odu_struct_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();

    for (i = 0; i < DIGI_OTN_SERVER_HO_CHNL_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_OTN_SERVER_HO_CHNL_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_ho_odu_struct_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_ho_odu_struct_state_all_print */

/*******************************************************************************
* digi_debug_mo_odu_struct_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing mo odu stuct states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_mo_odu_struct_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_OTN_SERVER_MO_CHNL_MAX) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[MO ODU HANDLE]=======================================================================================+\n");
            digi_debug_generic_odu_struct_state_header_print();
        }

        digi_debug_generic_odu_struct_state_print(&digi_handle->handle_pool.mo_odu_struct[index], index);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_mo_odu_struct_state_print */

/*******************************************************************************
* digi_debug_mo_odu_struct_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the mapper handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_mo_odu_struct_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();

    for (i = 0; i < DIGI_OTN_SERVER_MO_CHNL_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_OTN_SERVER_MO_CHNL_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_mo_odu_struct_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_mo_odu_struct_state_all_print */

/*******************************************************************************
* digi_debug_lo_odu_struct_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing lo odu stuct states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
*   index         -  the relative index of the handle for which to obtain the 
*                    state.
*   print_head    -  TRUE: Print the header. FALSE: Do not print.
*   print_foot    -  TRUE: Print the footer. FALSE: Do not print.
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
PUBLIC void digi_debug_lo_odu_struct_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot)
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    if (index < DIGI_OTN_SERVER_LO_CHNL_MAX) {

        if (print_head == TRUE) {
            PMC_LOG_NOTRACE("\n==========================================[LO ODU HANDLE]=======================================================================================+\n");
            digi_debug_generic_odu_struct_state_header_print();
        }

        digi_debug_generic_odu_struct_state_print(&digi_handle->handle_pool.lo_odu_struct[index], index);

        if (print_foot == TRUE) {
            digi_debug_generic_handle_state_footer_print();
        }
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_lo_odu_struct_state_print */

/*******************************************************************************
* digi_debug_lo_odu_struct_state_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the mapper handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_lo_odu_struct_state_all_print(digi_handle_t* digi_handle)
{

    UINT8 i;
    BOOL print_head = FALSE;
    BOOL print_foot = FALSE;

    PMC_ENTRY();

    for (i = 0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++) {

        /* First */
        if (i == 0) {

            print_head = TRUE;
            print_foot = FALSE;

        /* Last */
        } else if (i == DIGI_OTN_SERVER_LO_CHNL_MAX - 1) {

            print_head = FALSE;
            print_foot = TRUE;

        /* Middle */
        } else {

            print_head = FALSE;
            print_foot = FALSE;
        }

        digi_debug_lo_odu_struct_state_print(digi_handle, i, print_head, print_foot);
    }

    PMC_RETURN();
} /* digi_debug_lo_odu_struct_state_all_print */

/*******************************************************************************
* digi_debug_digi_handle_states_all_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing all the digi handle states
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_digi_handle_states_all_print(digi_handle_t* digi_handle)
{
    PMC_ENTRY();

    digi_debug_ilkn1_handle_state_all_print(digi_handle);
    digi_debug_ilkn2_handle_state_all_print(digi_handle);
    digi_debug_enet_line_handle_state_all_print(digi_handle);
    digi_debug_enet_sys_handle_state_all_print(digi_handle);
    
 
    digi_debug_mapper_handle_state_all_print(digi_handle);
    
   
    digi_debug_ho_odu_struct_state_all_print(digi_handle);
    digi_debug_mo_odu_struct_state_all_print(digi_handle);
    digi_debug_lo_odu_struct_state_all_print(digi_handle);
    

    PMC_RETURN();

} /* digi_debug_digi_handle_states_all_print */


/*******************************************************************************
* digi_debug_digi_resource_handle_print_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the entire digi_resource_handle_summary_t structure and all its
*   sub-structures. Represents the conectivity of the device.
*
* INPUTS:
*   digi_resource_handle  - Handle to the digi_reosurce_handle_summary_t
*                           structure
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
PUBLIC void digi_debug_digi_resource_handle_print_all(digi_resource_handle_summary_t *digi_resource_handle)
{
    UINT16 i=0;
    BOOL8 first = TRUE;
    PMC_ENTRY();


    for (i=0; i < DIGI_OTN_SERVER_CHNL_MAX; i++)
    {
        if (first)
        {
            digi_debug_generic_odu_struct_state_header_print();
            first = FALSE;
        }
        if (NULL != digi_resource_handle->line_otn_ho_container_cfg[i].chnl_handle_ptr)
        {
            digi_debug_generic_odu_struct_state_print((odu_struct_t *)digi_resource_handle->line_otn_ho_container_cfg[i].chnl_handle_ptr, i);
        }
    }
 
    if ( i > 0 )
    {
        digi_debug_generic_handle_state_footer_print();
    }
    first = TRUE;
    i = 0;

    for (i=0; i < DIGI_OTN_SERVER_MO_CHNL_MAX; i++)
    {
        if (first)
        {
            digi_debug_generic_odu_struct_state_header_print();
            first = FALSE;
        }
        if (NULL != digi_resource_handle->line_otn_mo_container_cfg[i].chnl_handle_ptr)
        {
            digi_debug_generic_odu_struct_state_print((odu_struct_t *)digi_resource_handle->line_otn_mo_container_cfg[i].chnl_handle_ptr, i);
        }
    }

    if (i > 0)
    {
        digi_debug_generic_handle_state_footer_print();
    }
    i = 0;
    first = TRUE;

    for (i=0; i < DIGI_OTN_SERVER_LO_CHNL_MAX; i++)
    {
         if (first)
        {
            digi_debug_generic_odu_struct_state_header_print();
            first = FALSE;
        }
        if (NULL != digi_resource_handle->line_otn_lo_container_cfg[i].chnl_handle_ptr)
        {
            digi_debug_generic_odu_struct_state_print((odu_struct_t *)digi_resource_handle->line_otn_lo_container_cfg[i].chnl_handle_ptr, i);
        }
    }

    if (i > 0)
    {
        digi_debug_generic_handle_state_footer_print();
    }
    i = 0;
    first = TRUE;


    for (i=0; i < DIGI_SIFD_ILKN_LINK_FLOW_CONTROL; i++)
    {
        if (first)
        {
            digi_debug_generic_odu_struct_state_header_print();
            first = FALSE;
        }
        if (NULL != digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_handle_ptr)
        {
            digi_debug_generic_handle_state_print((util_global_switch_data_def_t*)digi_resource_handle->ilkn_chnl_cfg[0][i].ilkn_chnl_handle_ptr, i, TRUE);
        }
    }

    if (i > 0)
    {
        digi_debug_generic_handle_state_footer_print();
    }
    i = 0;
    first = TRUE;

    for (i=0; i < DIGI_SIFD_ILKN_LINK_FLOW_CONTROL; i++)
    {
        if (first)
        {
            digi_debug_generic_odu_struct_state_header_print();
            first = FALSE;
        }
        if (NULL != digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_handle_ptr)
        {
            digi_debug_generic_handle_state_print((util_global_switch_data_def_t*)digi_resource_handle->ilkn_chnl_cfg[1][i].ilkn_chnl_handle_ptr, i, TRUE);
        }
    }

    if (i > 0)
    {
        digi_debug_generic_handle_state_footer_print();
    }

    i = 0;
    first = TRUE;

    for (i=0; i < DIGI_MAPOTN_CHNL_MAX; i++)
    {
        if (first)
        {
            digi_debug_generic_odu_struct_state_header_print();
            first = FALSE;
        }
        if (NULL != digi_resource_handle->mapper_cfg[i].mapper_handle_ptr)
        {
            digi_debug_generic_handle_state_print((util_global_switch_data_def_t*)digi_resource_handle->mapper_cfg[i].mapper_handle_ptr, i, TRUE);
        }
    }
    if (i > 0)
    {
        digi_debug_generic_handle_state_footer_print();
    }

    PMC_RETURN();
} /* digi_debug_digi_handle_states_all_print */


    
/*******************************************************************************
* digi_debug_mapotn_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing mapotn scheduler entries.
*
* INPUTS:
*   digi_handle   -  a pointer to the specific digi handle instance.
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
PUBLIC void digi_debug_mapotn_chnl_print(digi_handle_t* digi_handle)
{
#ifndef LOGGING_DISABLED
    UINT16 i = 0; /* counter */
    UINT16 j = 0; /* counter */
    UINT32 chnl[DIGI_SCHD_96_ENTRY];
    UINT32 sched_mask[][DIGI_NUM_ODU0S_PER_LANE_MAX] = DIGI_SCHED_MASK;
#endif

    PMC_ENTRY();
#ifndef LOGGING_DISABLED

    if (mapotn_db_entry_all_get(digi_handle->mapotn_handle, chnl) != PMC_SUCCESS) {

        PMC_LOG_NOTRACE("Nothing to do!\n");

    } else {

        PMC_LOG_NOTRACE("Cal Entreis: \n");
        for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i++) {
            PMC_LOG_NOTRACE("Row: %2d   ", i);
            for (j = 0; j < DIGI_NUM_ODU0S_PER_LANE_MAX; j++) {

                PMC_LOG_NOTRACE(": %2d ", chnl[i*DIGI_NUM_ODU0S_PER_LANE_MAX+j]);
            }
            PMC_LOG_NOTRACE("\n");
        }
        PMC_LOG_NOTRACE("Shed Entries: \n");
        for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i++) {
            PMC_LOG_NOTRACE("Lane: %2d   ", i);
            for (j = 0; j < DIGI_NUM_ODU0S_PER_LANE_MAX; j++) {

                PMC_LOG_NOTRACE(": %2d|%2d ", sched_mask[i][j], chnl[sched_mask[i][j]]);
            }
            PMC_LOG_NOTRACE("\n");
        }
    }
#endif
    PMC_RETURN();
} /* digi_debug_mapotn_chnl_print */




/*******************************************************************************
* digi_util_int_otn_server_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized LineOTN interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - LineOTN channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_otn_server_chnl_print(digi_handle_t            *digi_handle,
                                                     lineotn_int_chnl_t       *int_table_ptr,
                                                     digi_util_int_print_t     print_type,
                                                     digi_util_int_format_t   *format_ptr,
                                                     BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    #ifdef LINEOTN_INT
    #undef LINEOTN_INT
    #endif
    #define LINEOTN_INT DIGI_UTIL_INT_PRINT
    
    #ifdef LINEOTN_INT_LANE
    #undef LINEOTN_INT_LANE
    #endif
    #define LINEOTN_INT_LANE DIGI_UTIL_INT_PRINT_ARRAY
    
    #ifdef LINEOTN_INT_ARRAY
    #undef LINEOTN_INT_ARRAY
    #endif
    #define LINEOTN_INT_ARRAY DIGI_UTIL_INT_PRINT_ARRAY

    #ifdef LINEOTN_INT_100G
    #undef LINEOTN_INT_100G
    #endif
    #define LINEOTN_INT_100G DIGI_UTIL_INT_PRINT_SINGLE
    
    #ifdef LINEOTN_INT_100G_LANE
    #undef LINEOTN_INT_100G_LANE
    #endif
    #define LINEOTN_INT_100G_LANE DIGI_UTIL_INT_PRINT_ARRAY_SINGLE
    
    #ifdef LINEOTN_STATUS
    #undef LINEOTN_STATUS
    #endif
    #define LINEOTN_STATUS DIGI_UTIL_INT_PRINT
    
    #ifdef LINEOTN_STATUS_LANE
    #undef LINEOTN_STATUS_LANE
    #endif
    #define LINEOTN_STATUS_LANE DIGI_UTIL_INT_PRINT_ARRAY
    
    #ifdef LINEOTN_STATUS_ARRAY
    #undef LINEOTN_STATUS_ARRAY
    #endif
    #define LINEOTN_STATUS_ARRAY DIGI_UTIL_INT_PRINT_ARRAY

    #ifdef LINEOTN_STATUS_100G
    #undef LINEOTN_STATUS_100G
    #endif
    #define LINEOTN_STATUS_100G DIGI_UTIL_INT_PRINT_SINGLE
    
    #ifdef LINEOTN_STATUS_100G_LANE
    #undef LINEOTN_STATUS_100G_LANE
    #endif
    #define LINEOTN_STATUS_100G_LANE DIGI_UTIL_INT_PRINT_ARRAY_SINGLE

    
    DIGI_UTIL_INT_PRINT_SPACER(1);
    LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE();
    LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE();
    DIGI_UTIL_INT_PRINT_SPACER(1);
    LINEOTN_OPSM23_STATUS_CHNL_TABLE_DEFINE();
    LINEOTN_OPSM4_STATUS_CHNL_TABLE_DEFINE();


    if (PMC_SUCCESS == result)
    {
        result = digi_util_int_otu_frm_chnl_print(digi_handle, 
                                                  &int_table_ptr->otu_frm,
                                                  print_type,
                                                  format_ptr,
                                                  int_found_ptr);
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_util_int_sfis_chnl_print(digi_handle, 
                                               &int_table_ptr->sfis,
                                               print_type,
                                               format_ptr,
                                               int_found_ptr);
    }

    PMC_RETURN(result);
} /* digi_util_int_otn_server_chnl_print */


/*******************************************************************************
* digi_util_int_sfis_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized LineOTN SFIS interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - LineOTN channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_sfis_chnl_print(digi_handle_t            *digi_handle,
                                               lineotn_sfis_int_chnl_t  *int_table_ptr,
                                               digi_util_int_print_t     print_type,
                                               digi_util_int_format_t   *format_ptr,
                                               BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    #ifdef LINEOTN_INT
    #undef LINEOTN_INT
    #endif
    #define LINEOTN_INT DIGI_UTIL_INT_PRINT
    
    #ifdef LINEOTN_STATUS
    #undef LINEOTN_STATUS
    #endif
    #define LINEOTN_STATUS DIGI_UTIL_INT_PRINT
    
    #ifdef LINEOTN_INT_ARRAY
    #undef LINEOTN_INT_ARRAY
    #endif
    #define LINEOTN_INT_ARRAY DIGI_UTIL_INT_PRINT_ARRAY

    #ifdef LINEOTN_STATUS_ARRAY
    #undef LINEOTN_STATUS_ARRAY
    #endif
    #define LINEOTN_STATUS_ARRAY DIGI_UTIL_INT_PRINT_ARRAY

    #ifdef LINEOTN_INT_FIELD
    #undef LINEOTN_INT_FIELD
    #endif
    #define LINEOTN_INT_FIELD DIGI_UTIL_INT_PRINT_LINEOTN_FIELD

    
    DIGI_UTIL_INT_PRINT_SPACER(1);
    LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE();
    DIGI_UTIL_INT_PRINT_SPACER(1);
    LINEOTN_SFIS_STATUS_CHNL_TABLE_DEFINE();

    PMC_RETURN(result);
} /* digi_util_int_sfis_chnl_print */


/*******************************************************************************
* digi_util_int_otn_odu_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized OTN ODU interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - OTN ODU channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_otn_odu_chnl_print(digi_handle_t             *digi_handle,
                                                  digi_otn_odu_int_chnl_t   *int_table_ptr,
                                                  digi_util_int_print_t      print_type,
                                                  digi_util_int_format_t    *format_ptr,
                                                  BOOL                      *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* CoreOTN Interrupts */
    if (PMC_SUCCESS == result)
    {
        result = digi_util_int_coreotn_chnl_print(digi_handle,
                                                  &int_table_ptr->status,
                                                  print_type,
                                                  format_ptr,
                                                  int_found_ptr);
    }

    /* Stg 3b Interrupts */
    if ((PMC_SUCCESS == result) &&
        (TRUE == int_table_ptr->stg3b_valid))
    {
        DIGI_UTIL_INT_PRINT_SPACER(2);
        DIGI_UTIL_PRINT("Stage 3B:");
        result = digi_util_int_coreotn_chnl_print(digi_handle,
                                                  &int_table_ptr->stg3b,
                                                  print_type,
                                                  format_ptr,
                                                  int_found_ptr);
    }


    PMC_RETURN(result);
} /* digi_util_int_otn_odu_chnl_print */


/*******************************************************************************
* digi_util_int_coreotn_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized CoreOTN interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - CoreOTN channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_coreotn_chnl_print(digi_handle_t            *digi_handle,
                                                  coreotn_int_chnl_t       *int_table_ptr,
                                                  digi_util_int_print_t     print_type,
                                                  digi_util_int_format_t   *format_ptr,
                                                  BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i UNUSED;

    PMC_ENTRY();
   
    /* Framer */
    if (PMC_SUCCESS == result)
    {
        result = digi_util_int_coreotn_framer_chnl_print(digi_handle,
                                                         &int_table_ptr->framer_chnl,
                                                         print_type,
                                                         format_ptr,
                                                         int_found_ptr);
    }



    PMC_RETURN(result);
} /* digi_util_int_coreotn_chnl_print */


/*******************************************************************************
* digi_util_int_coreotn_framer_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized CoreOTN Framer interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - CoreOTN Framer channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_coreotn_framer_chnl_print(digi_handle_t                *digi_handle,
                                                         coreotn_int_framer_chnl_t    *int_table_ptr,
                                                         digi_util_int_print_t         print_type,
                                                         digi_util_int_format_t       *format_ptr,
                                                         BOOL                         *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (PMC_SUCCESS == result)
    {
        result = digi_util_int_odu_rfrm_chnl_print(digi_handle,
                                                   &int_table_ptr->rfrm,
                                                   print_type,
                                                   format_ptr,
                                                   int_found_ptr);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = digi_util_int_odtu_dmx_chnl_print(digi_handle,
                                                   &int_table_ptr->odtu_dmx,
                                                   print_type,
                                                   format_ptr,
                                                   int_found_ptr);
    }


    PMC_RETURN(result);
} /* digi_util_int_coreotn_framer_chnl_print */



/*******************************************************************************
* digi_util_int_odu_rfrm_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized ODU RFRM interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - ODU_RFRM channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_odu_rfrm_chnl_print(digi_handle_t            *digi_handle,
                                                   odu_rfrm_int_chnl_t      *int_table_ptr,
                                                   digi_util_int_print_t     print_type,
                                                   digi_util_int_format_t   *format_ptr,
                                                   BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT DIGI_UTIL_INT_PRINT_RANGE
    
    DIGI_UTIL_INT_PRINT_SPACER(1);
    ODU_RFRM_INT_CHNL_TABLE_DEFINE();


    /* PM Interrupts */

    #ifdef ODU_RFRM_INT_NOOP
    #undef ODU_RFRM_INT_NOOP
    #endif
    #define ODU_RFRM_INT_NOOP DIGI_UTIL_INT_PRINT

    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT DIGI_UTIL_INT_PRINT_ODU_RFRM
    
    #ifdef ODU_RFRM_STATUS_NOOP
    #undef ODU_RFRM_STATUS_NOOP
    #endif
    #define ODU_RFRM_STATUS_NOOP DIGI_UTIL_INT_PRINT

    #ifdef ODU_RFRM_STATUS
    #undef ODU_RFRM_STATUS
    #endif
    #define ODU_RFRM_STATUS DIGI_UTIL_INT_PRINT_ODU_RFRM
    
    DIGI_UTIL_INT_PRINT_SPACER(1);
    ODU_RFRM_INT_PM_TABLE_DEFINE();
    DIGI_UTIL_INT_PRINT_SPACER(1);
    ODU_RFRM_STATUS_PM_TABLE_DEFINE();


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_util_int_odu_rfrm_chnl_print */



/*******************************************************************************
* digi_util_int_odtu_dmx_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized ODTU DMX interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - ODTU DMX channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_odtu_dmx_chnl_print(digi_handle_t            *digi_handle,
                                                   odtu_dmx_int_chnl_t      *int_table_ptr,
                                                   digi_util_int_print_t     print_type,
                                                   digi_util_int_format_t   *format_ptr,
                                                   BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    #ifdef ODTU_DMX_INT
    #undef ODTU_DMX_INT
    #endif
    #define ODTU_DMX_INT DIGI_UTIL_INT_PRINT_RANGE
    
    #ifdef ODTU_DMX_INT_TRIB
    #undef ODTU_DMX_INT_TRIB
    #endif
    #define ODTU_DMX_INT_TRIB DIGI_UTIL_INT_PRINT_RANGE
    
    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE DIGI_UTIL_INT_PRINT_SINGLE
    
    
    #ifdef ODTU_DMX_STATUS
    #undef ODTU_DMX_STATUS
    #endif
    #define ODTU_DMX_STATUS DIGI_UTIL_INT_PRINT_RANGE
    
    #ifdef ODTU_DMX_STATUS_TRIB
    #undef ODTU_DMX_STATUS_TRIB 
    #endif
    #define ODTU_DMX_STATUS_TRIB DIGI_UTIL_INT_PRINT_RANGE
    
    #ifdef ODTU_DMX_STATUS_SINGLE
    #undef ODTU_DMX_STATUS_SINGLE
    #endif
    #define ODTU_DMX_STATUS_SINGLE DIGI_UTIL_INT_PRINT_SINGLE
    

    DIGI_UTIL_INT_PRINT_SPACER(1);
    ODTU_DMX_INT_TABLE_HO_DEFINE();
    ODTU_DMX_INT_TABLE_LO_DEFINE();
    DIGI_UTIL_INT_PRINT_SPACER(1);
    ODTU_DMX_STATUS_TABLE_HO_DEFINE();
    ODTU_DMX_STATUS_TABLE_LO_DEFINE();



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_util_int_odtu_dmx_chnl_print */


/*******************************************************************************
* digi_util_int_rgmp_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized RGMP interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - RGMP channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_rgmp_chnl_print(digi_handle_t            *digi_handle,
                                               rgmp_int_chnl_t          *int_table_ptr,
                                               digi_util_int_print_t     print_type,
                                               digi_util_int_format_t   *format_ptr,
                                               BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    #ifdef RGMP_INT
    #undef RGMP_INT
    #endif
    #define RGMP_INT DIGI_UTIL_INT_PRINT_RANGE
    
    
    #ifdef RGMP_STATUS
    #undef RGMP_STATUS
    #endif
    #define RGMP_STATUS DIGI_UTIL_INT_PRINT_RANGE
    

    DIGI_UTIL_INT_PRINT_SPACER(1);
    DIGI_UTIL_PRINT("RGMP:");
    DIGI_UTIL_INT_PRINT_SPACER(1);
    
    RGMP_INT_TABLE_DEFINE();
    DIGI_UTIL_INT_PRINT_SPACER(1);
    RGMP_STATUS_TABLE_DEFINE();

    PMC_RETURN(result);
} /* digi_util_int_rgmp_chnl_print */



/*******************************************************************************
* digi_util_int_otu_frm_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized OTU FRM interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - OTU FRM channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_otu_frm_chnl_print(digi_handle_t            *digi_handle,
                                                  otu_frm_int_chnl_t       *int_table_ptr,
                                                  digi_util_int_print_t     print_type,
                                                  digi_util_int_format_t   *format_ptr,
                                                  BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT8 link1;
    UINT8 link2;

    PMC_ENTRY();
    
    #ifdef OTU_FRM_INT
    #undef OTU_FRM_INT
    #endif
    #define OTU_FRM_INT DIGI_UTIL_INT_PRINT_RANGE
    
    #ifdef OTU_FRM_INT_NOOP
    #undef OTU_FRM_INT_NOOP
    #endif
    #define OTU_FRM_INT_NOOP DIGI_UTIL_INT_PRINT 

    
    
    #ifdef OTU_FRM_STATUS
    #undef OTU_FRM_STATUS
    #endif
    #define OTU_FRM_STATUS DIGI_UTIL_INT_PRINT_RANGE
    
    #ifdef OTU_FRM_STATUS_NOOP
    #undef OTU_FRM_STATUS_NOOP
    #endif
    #define OTU_FRM_STATUS_NOOP DIGI_UTIL_INT_PRINT 
    

    DIGI_UTIL_INT_PRINT_SPACER(1);
    OTU_FRM_INT_TABLE_DEFINE(0,0);
    DIGI_UTIL_INT_PRINT_SPACER(1);
    OTU_FRM_STATUS_TABLE_DEFINE(0,0);



    PMC_RETURN(result);
} /* digi_util_int_otu_frm_chnl_print */



/*******************************************************************************
* digi_util_int_mapotn_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized MAPOTN interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - MAPOTN channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_mapotn_chnl_print(digi_handle_t            *digi_handle,
                                                 mapotn_int_chnl_t        *int_table_ptr,
                                                 digi_util_int_print_t     print_type,
                                                 digi_util_int_format_t   *format_ptr,
                                                 BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    #ifdef MAPOTN_INT_CHNL
    #undef MAPOTN_INT_CHNL
    #endif
    #define MAPOTN_INT_CHNL DIGI_UTIL_INT_PRINT_RANGE
    
    #ifdef MAPOTN_INT_CHNL_ARRAY
    #undef MAPOTN_INT_CHNL_ARRAY
    #endif
    #define MAPOTN_INT_CHNL_ARRAY DIGI_UTIL_INT_PRINT_ARRAY_RANGE
    
    
    #ifdef MAPOTN_STATUS_CHNL
    #undef MAPOTN_STATUS_CHNL
    #endif
    #define MAPOTN_STATUS_CHNL DIGI_UTIL_INT_PRINT_RANGE
    

    DIGI_UTIL_INT_PRINT_SPACER(1);
    MAPOTN_INT_CHNL_TABLE_DEFINE();
    if (pmc_is_digi_rev_a_revision(&digi_handle->base) == FALSE)
    {
        MAPOTN_INT_CHNL_TABLE_REV_B_DEFINE();
    }
    DIGI_UTIL_INT_PRINT_SPACER(1);
    MAPOTN_STATUS_CHNL_TABLE_DEFINE();
    if (pmc_is_digi_rev_a_revision(&digi_handle->base) == FALSE)
    {
        MAPOTN_STATUS_CHNL_TABLE_REV_B_DEFINE();
    }

    if((PMC_SUCCESS == result) &&
       (int_table_ptr->rgmp_valid))
    {
        DIGI_UTIL_INT_PRINT_SPACER(2);
        result = digi_util_int_rgmp_chnl_print(digi_handle,
                                               &int_table_ptr->rgmp,
                                               print_type,
                                               format_ptr,
                                               int_found_ptr);
    }



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_util_int_mapotn_chnl_print */






/*******************************************************************************
* digi_util_int_mapper_chnl_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the channelized Mapper interrupt table.
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   int_table_ptr    - Mapper channelized interrupt table
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*   format_ptr       - Structure describing formatting options such as 
*                      column width and number of columns
*
* OUTPUTS:
*  int_found_ptr    - Returns if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_mapper_chnl_print(digi_handle_t            *digi_handle,
                                                 digi_mapper_int_chnl_t   *int_table_ptr,
                                                 digi_util_int_print_t     print_type,
                                                 digi_util_int_format_t   *format_ptr,
                                                 BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (PMC_SUCCESS == result)
    {
        result = digi_util_int_mapotn_chnl_print(digi_handle, 
                                                 &int_table_ptr->status,
                                                 print_type,
                                                 format_ptr,
                                                 int_found_ptr);
    }

    if (PMC_SUCCESS == result)
    {
        DIGI_UTIL_INT_PRINT_SPACER(2);
        DIGI_UTIL_PRINT("FO2:");
        result = digi_util_int_otn_odu_chnl_print(digi_handle, 
                                                  &int_table_ptr->fo2,
                                                  print_type,
                                                  format_ptr,
                                                  int_found_ptr);
    }

    PMC_RETURN(result);
} /* digi_util_int_mapper_chnl_print */





/*******************************************************************************
* digi_util_int_otn_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the aggregated OTN interrupt table.  
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   *int_table_ptr   - OTN aggregated interrupt table to print
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*
* OUTPUTS:
*  int_found_ptr     - Returns TRUE if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_otn_print(digi_handle_t         *digi_handle,
                                         digi_otn_int_t        *int_table_ptr,
                                         digi_util_int_print_t  print_type,
                                         BOOL                  *int_found_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    BOOL                    int_found = FALSE;
    BOOL                    print_chnl;
    UINT32                  i;
    digi_util_odu_info_t    odu_info[3]; /* Store HO/MO/LO ODU info */
    UINT32                  odu_level;
    digi_util_int_format_t  format;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* OTN Server format config */
    format.field_width = 40;
    format.value_width = 7;
    format.num_columns = 2;

 
    DIGI_UTIL_PRINT("OTN Server\n");
    DIGI_UTIL_PRINT_DIVIDER();
    for (i = 0; (i < DIGI_OTN_SERVER_CHNL_MAX) && (PMC_SUCCESS == result); i++)
    {
        print_chnl = FALSE;
        digi_util_int_otn_server_chnl_print(digi_handle,
                                            &int_table_ptr->otn_server_chnl[i],
                                            DIGI_UTIL_INT_PRINT_NONE, NULL,
                                            &print_chnl);

        if ((int_table_ptr->otn_server_handle[i]) && (TRUE == print_chnl))
        {
            DIGI_UTIL_PRINT("Channel Handle: %p\n", int_table_ptr->otn_server_handle[i]);
            DIGI_UTIL_PRINT("Channel ID:     %d\n", i);

            result = digi_util_int_otn_server_chnl_print(digi_handle,
                                                         &int_table_ptr->otn_server_chnl[i],
                                                         print_type,
                                                         &format,
                                                         &int_found);
            DIGI_UTIL_INT_PRINT_SPACER(3);
        }
    } 
   



    odu_info[ODU_STRUCT_LEVEL_HO_ODU].name          = (char*) "HO";
    odu_info[ODU_STRUCT_LEVEL_HO_ODU].chnl_handle   = int_table_ptr->otn_odu_ho_handle;
    odu_info[ODU_STRUCT_LEVEL_HO_ODU].max_chnl      = DIGI_OTN_SERVER_HO_CHNL_MAX;
    odu_info[ODU_STRUCT_LEVEL_HO_ODU].int_table_ptr = int_table_ptr->otn_odu_ho_chnl;

    odu_info[ODU_STRUCT_LEVEL_MO_ODU].name          = (char*) "MO";
    odu_info[ODU_STRUCT_LEVEL_MO_ODU].chnl_handle   = int_table_ptr->otn_odu_mo_handle;
    odu_info[ODU_STRUCT_LEVEL_MO_ODU].max_chnl      = DIGI_OTN_SERVER_MO_CHNL_MAX;
    odu_info[ODU_STRUCT_LEVEL_MO_ODU].int_table_ptr = int_table_ptr->otn_odu_mo_chnl;
    
    odu_info[ODU_STRUCT_LEVEL_LO_ODU].name          = (char*) "LO";
    odu_info[ODU_STRUCT_LEVEL_LO_ODU].chnl_handle   = int_table_ptr->otn_odu_lo_handle;
    odu_info[ODU_STRUCT_LEVEL_LO_ODU].max_chnl      = DIGI_OTN_SERVER_LO_CHNL_MAX;
    odu_info[ODU_STRUCT_LEVEL_LO_ODU].int_table_ptr = int_table_ptr->otn_odu_lo_chnl;


    /* OTN ODU format config */
    format.field_width = 19;
    format.value_width = 3;
    format.num_columns = 4;
   
    for (odu_level = ODU_STRUCT_LEVEL_HO_ODU;
         (odu_level <= ODU_STRUCT_LEVEL_LO_ODU) && (PMC_SUCCESS == result);
         odu_level++)
    {
        DIGI_UTIL_PRINT("%s OTN ODU\n", odu_info[odu_level].name);
        DIGI_UTIL_PRINT_DIVIDER();
        for (i = 0; (i < odu_info[odu_level].max_chnl) && (PMC_SUCCESS == result); i++)
        {
            print_chnl = FALSE;
            digi_util_int_otn_odu_chnl_print(digi_handle,
                                             &odu_info[odu_level].int_table_ptr[i],
                                             DIGI_UTIL_INT_PRINT_NONE, NULL,
                                             &print_chnl);

            if ((odu_info[odu_level].chnl_handle[i]) && (TRUE == print_chnl))
            {
                DIGI_UTIL_PRINT("Channel Handle: %p\n", odu_info[odu_level].chnl_handle[i]);
                DIGI_UTIL_PRINT("Channel ID:     %d\n", i);
                DIGI_UTIL_PRINT_SUBDIVIDER();

                result = digi_util_int_otn_odu_chnl_print(digi_handle,
                                                          &odu_info[odu_level].int_table_ptr[i],
                                                          print_type,
                                                          &format,
                                                          &int_found);
                DIGI_UTIL_INT_PRINT_SPACER(3);
            }
        } 
    } /* end for */
   

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_util_int_otn_print */



/*******************************************************************************
* digi_util_int_mapper_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Prints the aggregated Mapper interrupt table.  
*
* INPUTS:
*   digi_handle      - a pointer to the specific digi handle instance.
*   *int_table_ptr   - Mapper aggregated interrupt table to print
*   print_type       - DIGI_UTIL_INT_PRINT_ALL to print all status fields,\n
*                      DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,\n
*                      DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                      return TRUE/FALSE in int_found_ptr
*
* OUTPUTS:
*  int_found_ptr     - Returns TRUE if an active interrupt was found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_int_mapper_print(digi_handle_t         *digi_handle,
                                            digi_mapper_int_t     *int_table_ptr,
                                            digi_util_int_print_t  print_type,
                                            BOOL                  *int_found_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    BOOL                    int_found = FALSE;
    BOOL                    print_chnl;
    UINT32                  i;
    digi_util_int_format_t  format;

    PMC_ATOMIC_ENTRY(digi_handle);



    /* Mapper format config */
    format.field_width = 23;
    format.value_width = 4;
    format.num_columns = 3;

 
    DIGI_UTIL_PRINT("OTN Mapper\n");
    DIGI_UTIL_PRINT_DIVIDER();
    for (i = 0; (i < DIGI_MAPOTN_CHNL_MAX) && (PMC_SUCCESS == result); i++)
    {
        print_chnl = FALSE;
        digi_util_int_mapper_chnl_print(digi_handle,
                                        &int_table_ptr->mapper_chnl[i],
                                        DIGI_UTIL_INT_PRINT_NONE, NULL,
                                        &print_chnl);

        if ((int_table_ptr->handle[i]) && (TRUE == print_chnl))
        {
            DIGI_UTIL_PRINT("Channel Handle: %p\n", int_table_ptr->handle[i]);
            DIGI_UTIL_PRINT("Channel ID:     %d\n", i);

            result = digi_util_int_mapper_chnl_print(digi_handle,
                                                     &int_table_ptr->mapper_chnl[i],
                                                     print_type,
                                                     &format,
                                                     &int_found);
            DIGI_UTIL_INT_PRINT_SPACER(3);
        }
    } 
   
   

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_util_int_mapper_print */

/*******************************************************************************
* digi_util_cal_gfp_core_header
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculate GFP CORE HEADER.  
*
* INPUTS:
*   pli              - Payload length indicator
*
* OUTPUTS:
*  cHEC_ptr          - Pointer to Core HEC.
*  core_header_ptr   - Pointer to GFP CORE HEADER.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*   This function is not atomic protected since it use no shared data.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_cal_gfp_core_header(UINT16           pli,
                                               UINT16           *cHEC_ptr,
                                               UINT32           *core_header_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    UINT16      cHEC = 0;
    UINT8       data_array[2];
    UINT32      data_size = 0;
    UINT32      core_header = 0;
    const UINT32  xor_code = 0xB6AB31E0;

    PMC_ENTRY();

    PMC_ASSERT(0 != pli, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != cHEC_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != core_header_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    data_array[0] = (UINT8)((pli & 0xFF00) >> 8);
    data_array[1] = (UINT8)(pli & 0x00FF);
        
    if( data_array[0] != 0 )
        data_size = 2;
    else
        data_size = 1;

    cHEC = digi_util_crc_16(data_array, data_size);

    core_header = (pli << 16) + cHEC;
    core_header = core_header ^ xor_code;

    *cHEC_ptr = cHEC;
    *core_header_ptr = core_header;
    
    PMC_RETURN(result);
}/* digi_util_cal_gfp_core_header */

/*******************************************************************************
* digi_util_cal_gfp_payload
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculate GFP PAYLOAD.  
*
* INPUTS:
*   type             - GFP type
*
* OUTPUTS:
*  tHEC_ptr          - Pointer to Type HEC.
*  payload_ptr       - Pointer to GFP Payload.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*   This function is not atomic protected since it use no shared data.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_util_cal_gfp_payload(UINT16       type,
                                           UINT16       *tHEC_ptr,
                                           UINT32       *payload_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    UINT16      tHEC = 0;
    UINT8       data_array[2];
    UINT32      data_size = 0;
    UINT32      payload = 0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != tHEC_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != payload_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(0 != type, DIGI_ERR_INVALID_ARG, 0, 0);

    data_array[0] = (UINT8)((type & 0xFF00) >> 8);
    data_array[1] = (UINT8)(type & 0x00FF);
        
    if( data_array[0] != 0 )
        data_size = 2;
    else
        data_size = 1;

    tHEC = digi_util_crc_16(data_array, data_size);

    payload = (type << 16) + tHEC;

    *tHEC_ptr = tHEC;
    *payload_ptr = payload;
    
    PMC_RETURN(result);
}/* digi_util_cal_gfp_core_header */

/* 
** Private Functions  
*/

/*******************************************************************************
* digi_util_build_table_16
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Build 16 bit CRC table
*
* INPUTS:
*   aPoly - 16bit initial value  
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
PRIVATE void digi_util_build_table_16( UINT16 aPoly ) 
{ 
    UINT16 i, j;
    UINT16 nData;
    UINT16 nAccum;

    for ( i = 0; i < 256; i++ ) 
    { 
        nData = ( unsigned short )( i << 8 ); 
        nAccum = 0; 
        for ( j = 0; j < 8; j++ )
        {
            if ( ( nData ^ nAccum ) & 0x8000 ) 
            {
                nAccum = ( nAccum << 1 ) ^ aPoly;
            }
            else     
            {
                nAccum <<= 1;
            }
            
            nData <<= 1; 
        }
        
        Table_CRC[i] = ( unsigned long )nAccum;
    } 
} 

/*******************************************************************************
* digi_util_crc_16
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculate 16 bit CRC value.
*   The polynomial is G(x) = x16 + x12 + x5 + 1.
*
* INPUTS:
*   aData   - a pointer to data array
*   aSize   - array size
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT16 - 16 bit CRC value.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT16 digi_util_crc_16( UINT8 *aData, UINT32 aSize ) 
{ 
    UINT32 i;
    UINT16 nAccum = 0;
    /* CRC-CCITT = X16 + X12 + X5 + X0 */
    const unsigned short cnCRC_CCITT = 0x1021;
    
    digi_util_build_table_16( cnCRC_CCITT ); 
    
    for ( i = 0; i < aSize; i++ )
        nAccum = ( nAccum << 8 ) ^ ( UINT16 )Table_CRC[( nAccum >> 8 ) ^ *aData++];

    return nAccum; 
} /* digi_util_crc_16 */


/*******************************************************************************
* digi_debug_generic_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic handle state
*
* INPUTS:
*   hndl_chnl_ptr   - a pointer to the generic global_switch_data_def_t
*   index           - When calling multiple times, the relative index into the 
*                     array of the handle type. Note this number is only use
*                     for display purposes.
*   print_spacer    - TRUE: Print a spacer after the ino . FALSE: Do not print.
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
PRIVATE void digi_debug_generic_handle_state_print(util_global_switch_data_def_t* hndl_chnl_ptr, UINT32 index, BOOL print_spacer) 
{
#ifndef LOGGING_DISABLED

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    char  prov_state_name[][10] = {"UNCONFIG", "CONFIG", "PROV", "LAST"};
    char  map_state_name[][10] = {"NO", "YES"};
    char  active_state_name[][10] = {"INACTIVE", "ACTIVE", "LAST"};
    char  cpb_port_type_name[][10] = {"ILKN1", "ILKN2", "CBRC", "MAPOTN", 
                                      "ENET_LINE", "ENET_SYS", "NOT_USED", "LAST"};
    char  oduk_port_type_name[][10] = {"ILKN1", "ILKN2", "COREOTN", "MAPOTN", 
                                       "NOT_USED", "LAST"};

                   /* 0   1     2    3    4    5    6    7    8    9   10   11 */
    PMC_LOG_NOTRACE("%7d|%12lX|%11X|%12s|%12s|%12s|%11d|%12s|%12s|%9d|%13d|%11s\n",
               /*  0 */ index,
               /*  1 */ (long unsigned int)hndl_chnl_ptr,
               /*  2 */ hndl_chnl_ptr->header.data_key,
               /*  3 */ prov_state_name[hndl_chnl_ptr->header.prov_state],
               /*  4 */ map_state_name[hndl_chnl_ptr->cpb_data.op_state.map_state.is_input_mapped],
               /*  5 */ map_state_name[hndl_chnl_ptr->cpb_data.op_state.map_state.is_output_mapped],
               /*  6 */ hndl_chnl_ptr->cpb_data.op_state.map_state.input_map_count,
               /*  7 */ active_state_name[hndl_chnl_ptr->cpb_data.op_state.active_state],
               /*  8 */ cpb_port_type_name[hndl_chnl_ptr->cpb_data.port_type],
               /*  9 */ hndl_chnl_ptr->cpb_data.channel,
               /* 10 */ hndl_chnl_ptr->cpb_data.data_source,
               /* 11 */ "|");
                   /* 0   1    2    3    4    5    6   7    8 */
    PMC_LOG_NOTRACE("%46s%12s|%12s|%11d|%12s|%12s|%9d|%13s|%11s\n",
                /* 0 */ "|",
                /* 1 */ map_state_name[hndl_chnl_ptr->oduk_data.op_state.map_state.is_input_mapped],
                /* 2 */ map_state_name[hndl_chnl_ptr->oduk_data.op_state.map_state.is_output_mapped],
                /* 3 */ hndl_chnl_ptr->oduk_data.op_state.map_state.input_map_count,
                /* 4 */ active_state_name[hndl_chnl_ptr->oduk_data.op_state.active_state],
                /* 5 */ oduk_port_type_name[hndl_chnl_ptr->oduk_data.port_type],
                /* 6 */ hndl_chnl_ptr->oduk_data.channel,
                /* 7 */ "UNUSEDON_ODUK",
                /* 8 */ "|");

    if (print_spacer == TRUE) {

        digi_debug_generic_handle_state_spacer_print();
    }

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */
#endif
} /* digi_debug_generic_handle_state_print */

/*******************************************************************************
* digi_debug_generic_handle_state_header_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic handle state header
*
* INPUTS:
*   None.
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
PRIVATE void digi_debug_generic_handle_state_header_print() 
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    PMC_LOG_NOTRACE("=============[global_state]==================|===========================================[cpb_state]============================================|\n");
    PMC_LOG_NOTRACE("              [NOT IN USE]                   |===========================================[oduk_state]===========================================|\n");
    PMC_LOG_NOTRACE("=======|============|===========|============|==============op_state============================|==========specific data========================|\n");
    PMC_LOG_NOTRACE("index  | phys_adr   | data_key  | prov_state |  is_in_map?| is_out_map?| in_map_cnt|  active_st |   port_type|  channel|  data_srce|[NOT IN USE]|\n");
    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_generic_handle_state_header_print */

/*******************************************************************************
* digi_debug_generic_handle_state_spacer_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic handle state spacer
*
* INPUTS:
*   None.
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
PRIVATE void digi_debug_generic_handle_state_spacer_print() 
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    PMC_LOG_NOTRACE("-------|------------|-----------|------------|--------------------------------------------------|------------------------------------|----------|\n");

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_generic_handle_state_spacer_print */

/*******************************************************************************
* digi_debug_generic_handle_state_footer_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic handle state footer
*
* INPUTS:
*   None.
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
PRIVATE void digi_debug_generic_handle_state_footer_print() 
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    PMC_LOG_NOTRACE("================================================================================================================================================+\n");

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_generic_handle_state_footer_print */

/*******************************************************************************
* digi_debug_generic_odu_struct_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic handle state
*
* INPUTS:
*   odu_struct_ptr  - a pointer to the generic odu_struct_t structure.
*   index           - When calling multiple times, the relative index into the 
*                     array of the handle type. Note this number is only use
*                     for display purposes.
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
PRIVATE void digi_debug_generic_odu_struct_state_print(odu_struct_t* odu_struct_ptr, UINT32 index) 
{
#ifndef LOGGING_DISABLED

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    char  odu_level_name[][7] = {"HO_ODU", "MO_ODU", "LO_ODU", "3B_ODU", "LAST"};
    char  odu_no_yes_name[][5] = {"NO", "YES"};

    digi_debug_generic_handle_state_print((util_global_switch_data_def_t*)odu_struct_ptr, index, FALSE);

                  /* 0    1     2    3    4   */
    PMC_LOG_NOTRACE("%7d|%12lx|%11s|%12s|%86s\n",
               /*  0 */ odu_struct_ptr->mem_ptr->chnl_id,
               /*  1 */ (long unsigned int)odu_struct_ptr->mem_ptr,
               /*  2 */ odu_level_name[odu_struct_ptr->mem_ptr->odu_level],
               /*  3 */ odu_no_yes_name[(unsigned int)odu_struct_ptr->active],
               /*  4 */ "|");

    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */
#endif
} /* digi_debug_generic_odu_struct_state_print */

/*******************************************************************************
* digi_debug_generic_odu_struct_state_header_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic odu_struct state header
*
* INPUTS:
*   None.
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
PRIVATE void digi_debug_generic_odu_struct_state_header_print() 
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    digi_debug_generic_handle_state_header_print();

    PMC_LOG_NOTRACE("=============[odu_struct data]============================|                              [NOT IN USE]                                           |\n");

    digi_debug_generic_handle_state_spacer_print();
    PMC_LOG_NOTRACE("chnl_id| mem_ptr    | odu_level |  is_active?|is_shrd_mem?|                                                                                     |\n");
    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_generic_odu_struct_state_header_print */

/*******************************************************************************
* digi_debug_generic_enet_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic enet handle state
*
* INPUTS:
*   digi_handle     - a pointer to the specific digi handle instance.
*   enet_handle_ptr - a pointer to the enet_client_def_t structure.
*   index           - When calling multiple times, the relative index into the 
*                     array of the handle type. Note this number is only use
*                     for display purposes.
*   port_type       - The port type to determine weather this is a LINE or SYS
*                     instance
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
PRIVATE void digi_debug_generic_enet_handle_state_print(digi_handle_t* digi_handle, digi_enet_client_chnl_def_t* enet_handle_ptr, UINT32 index, util_global_cpb_port_t port_type) 
{
#ifndef LOGGING_DISABLED

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    char  port_mode_name[][20] = {
    "ANY_LOWPWR",
    "S16_100_GE",
    "S16_40_GE",
    "S16_10_GE",
    "S16_FC_1200",
    "S16_FC_800",
    "S16_10G_GDPS",
    "S16_5G_GDPS",
    "S16_CPRI_10G",
    "S16_CPRI_6G",
    "S16_CPRI_5G",
    "S16_OC_768",
    "S16_OC_192",

    "S16_OTU4",
    "S16_OTU3",
    "S16_OTU3E2",
    "S16_OTU3E1",
    "S16_OTU2",
    "S16_OTU2E",
    "S16_OTU1E",
    "S16_OTU2F",
    "S16_OTU1F",
    "D8_OC_768",
    "D8_OTU3",
    "D8_OTU3E2",
    "D8_OTU3E1",
    "D8_GE",
    "T8_3G125",
    "T8_6G25",
    "C8_3G125",
    "C8_6G25",
    "S16_3G125",
    "S16_6G25",
    "S16_10G3125",
    "S16_11G5",
    "S16_12G5",
    "LAST"};

    char is_enh_pmon [][4] = { "NO", "YES" };
    UINT8 is_enh_pmon_index = 0;

    digi_debug_generic_handle_state_print((util_global_switch_data_def_t*)enet_handle_ptr, index, FALSE);

    if (port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) {

        is_enh_pmon_index = (digi_handle->var.digi_enet_pmon_line_usage & (1 << index)) > 0 ? 1 : 0;
        
    } else if (port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) {

        is_enh_pmon_index = (digi_handle->var.digi_enet_pmon_sys_usage & (1 << index)) > 0 ? 1 : 0;

    } else {

        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
    }

                   /* 0    1    2   3 */
    PMC_LOG_NOTRACE("%20s|%24d|%12s|%86s\n",
               /*  0 */ port_mode_name[enet_handle_ptr->serdes_mode],
               /*  1 */ enet_handle_ptr->dsi_base_lane,
               /*  1 */ is_enh_pmon[is_enh_pmon_index],
               /*  3 */ "|");

    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */
#endif
} /* digi_debug_generic_enet_handle_state_print */

/*******************************************************************************
* digi_debug_generic_enet_handle_state_header_print 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic enet handle state header
*
* INPUTS:
*   None.
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
PRIVATE void digi_debug_generic_enet_handle_state_header_print() 
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    digi_debug_generic_handle_state_header_print();

    PMC_LOG_NOTRACE("=============[enet_handle_data]===========================|                              [NOT IN USE]                                           |\n");

    digi_debug_generic_handle_state_spacer_print();
    PMC_LOG_NOTRACE("serdes_port_mode    |       dsi_base_lane    | Enh-PMON?  |                              [NOT IN USE]                                           |\n");                                                                                                         
    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_generic_enet_handle_state_header_print */

/*******************************************************************************
* digi_debug_generic_mapper_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic mapper handle state
*
* INPUTS:
*   map_handle_ptr  - a pointer to the map_client_def_t structure.
*   index           - When calling multiple times, the relative index into the 
*                     array of the handle type. Note this number is only use
*                     for display purposes.
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
PRIVATE void digi_debug_generic_mapper_handle_state_print(digi_mapper_chnl_def_t* map_handle_ptr, UINT32 index) 
{
#ifndef LOGGING_DISABLED
    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    char  map_mode_name[][29] = {
    "ODUKP_PACKET_GFP",
    "ODU4P_100_GE_GFP",
    "ODU4P_100_GE_GFP_ENET",
    "ODU3P_40_GE_GFP",
    "ODU3P_40_GE_GFP_ENET",
    "ODU2P_CBR_10_GE_7_3_GFP",
    "ODU2P_CBR_10_GE_7_3_GFP_ENET",
    "ODU2P_PKT_10_GE_7_3_GFP",
    "ODU2P_10_GE_6_2_GFP",
    "ODU2P_10_GE_6_2_GFP_ENET",
    "ODU2EP_FC_1200_GFP",
    "ODU2P_FC_800_GMP",
    "ODU2EP_10G_GDPS_GMP",
    "ODU2P_5G_GDPS_GMP",
    "ODU2P_CPRI_5_GMP",
    "ODU2P_CPRI_10_GMP",
    "ODU2P_CPRI_6_GMP",
    "ODU3P_CBR_40_GE_GMP",
    "ODU3P_PKT_40_GE_GMP",
    "ODU4P_CBR_100_GE_GMP",
    "ODU4P_PKT_100_GE_GMP",
    "ODU3E2P_40G_CBR_GMP",
    "ODUFLEX_5G_GDPS_BMP",
    "ODUFLEX_10G_GDPS_BMP",
    "ODUFLEX_CPRI_5_BMP",
    "ODUFLEX_CPRI_10_BMP",
    "ODUFLEX_CPRI_6_BMP",
    "ODUFLEX_FC_800_BMP",
    "ODU2EP_FC_1200_BMP",
    "ODU2FP_FC_1200_BMP",
    "ODU1FP_FC_1200_BMP",
    "ODU1EP_CBR_10G3_7_2_BMP",
    "ODU2EP_CBR_10G3_7_1_BMP",
    "ODU1EP_PKT_10G3_7_2_BMP",
    "ODU2EP_PKT_10G3_7_1_BMP",
    "ODU3P_OC_768_CBR_BMP",
    "ODU2P_OC_192_CBR_BMP",
    "ODU3P_OC_768_CBR_AMP",
    "ODU2P_OC_192_CBR_AMP",
    "LAST"};

    char  pmon_type_name[][5] = {

    "NONE",
    "LINE",
    "SYS",
    "LAST"};

    digi_debug_generic_handle_state_print((util_global_switch_data_def_t*)map_handle_ptr, index, FALSE);

                   /* 0    1    2    3     4 */
    PMC_LOG_NOTRACE("%32s|%12s|%12d|%37lX|%48s\n",
               /*  0 */ map_mode_name[map_handle_ptr->mode],
               /*  1 */ pmon_type_name[map_handle_ptr->enet_pmon],
               /*  2 */ map_handle_ptr->mapotn_chnl,
               /*  3 */ (long unsigned int)map_handle_ptr->enet_pmon_data,
               /*  4 */ "|");

    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */
#endif
} /* digi_debug_generic_mapper_handle_state_print */

/*******************************************************************************
* digi_debug_generic_mapper_handle_state_header_print 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic mapper handle state header
*
* INPUTS:
*   None.
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
PRIVATE void digi_debug_generic_mapper_handle_state_header_print() 
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    digi_debug_generic_handle_state_header_print();

    PMC_LOG_NOTRACE("============================[mapper_handle_data]================================================|                   [NOT IN USE]                |\n");

    digi_debug_generic_handle_state_spacer_print();

                   /*         1         2         3         4         5         6         7         8         9         0         1         2         3         4       */
                   /*123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567*/
    PMC_LOG_NOTRACE("mode                            |  enet_pmon | mapper_chnl|                       enet_pmon_data|                                               |\n");

    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_generic_mapper_handle_state_header_print */

/*******************************************************************************
* digi_debug_generic_ilkn_handle_state_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic ilkn handle state
*
* INPUTS:
*   ilkn_handle_ptr - a pointer to the digi_ilkn_chnl_def_t structure.
*   index           - When calling multiple times, the relative index into the 
*                     array of the handle type. Note this number is only use
*                     for display purposes.
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
PRIVATE void digi_debug_generic_ilkn_handle_state_print(digi_ilkn_chnl_def_t* ilkn_handle_ptr, UINT32 index)
{
#ifndef LOGGING_DISABLED

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    char  ilkn_path_name[][7] = {
    "UNUSED",
    "ILKN_1",
    "ILKN_2",

    "LAST"};

    char  pmon_type_name[][5] = {

    "NONE",
    "LINE",
    "SYS",
    "LAST"};

    char ilkn_targ_name[][5] = {
    "CPB",
    "ODUK",
    "LAST"};

    char  use_hdr_name[][4] = {"NO", "YES"};

    digi_debug_generic_handle_state_print((util_global_switch_data_def_t*)ilkn_handle_ptr, index, FALSE);

                   /* 0   1    2    3   4   5    6    7    8    9    10  11    12 */
    PMC_LOG_NOTRACE("%7s|%12s|%11s|%6s|%5s|%12x|%12x|%11x|%12x|%12x|%9x|%13lX|%11s\n",
               /*  0 */ ilkn_path_name[ilkn_handle_ptr->sifd_path],
               /*  1 */ pmon_type_name[ilkn_handle_ptr->enet_pmon],
               /*  2 */ ilkn_targ_name[ilkn_handle_ptr->target],
               /*  3 */ use_hdr_name[ilkn_handle_ptr->tx_type_header],
               /*  4 */ use_hdr_name[ilkn_handle_ptr->rx_type_header],
               /*  5 */ ilkn_handle_ptr->tx_ilkn_info.ilkn_bcw_chnl,
               /*  6 */ ilkn_handle_ptr->tx_ilkn_info.ilkn_2b_header,
               /*  7 */ ilkn_handle_ptr->tx_ilkn_info.ilkn_4b_header,
               /*  8 */ ilkn_handle_ptr->rx_ilkn_info.ilkn_bcw_chnl,
               /*  9 */ ilkn_handle_ptr->rx_ilkn_info.ilkn_2b_header,
               /* 10 */ ilkn_handle_ptr->rx_ilkn_info.ilkn_4b_header,
               /* 11 */ (long unsigned int)ilkn_handle_ptr->enet_pmon_data,
               /* 12 */ "|");

    digi_debug_generic_handle_state_spacer_print();

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */
#endif
} /* digi_debug_generic_ilkn_handle_state_print */

/*******************************************************************************
* digi_debug_generic_ilkn_handle_state_header_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is the helper function for printing the generic ilkn handle state header
*
* INPUTS:
*   None.
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
PRIVATE void digi_debug_generic_ilkn_handle_state_header_print()
{

    /* Purposely removing this PMC_ENTRY() as it adds additional printing which 
     * interferes with formating */

    digi_debug_generic_handle_state_header_print();

    PMC_LOG_NOTRACE("=======================================================[ilkn_handle_data]=========================================================== |NOT IN USE|\n");
    digi_debug_generic_handle_state_spacer_print();

                   /*         1         2         3         4         5         6         7         8         9         0         1         2         3         4       */
                   /*123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567*/
    PMC_LOG_NOTRACE("path   |   enet_pmon|   targ    |rx_hd?|tx_hd?    tx_BCW  | tx_2B_hd   | tx_4B_hd  |   rx_BCW   |    rx_2B_hd| rx_4B_hd|enet_pmon_dat|          |\n");

    /* Purposely removing this PMC_RETURN() as it adds additional printing which 
     * interferes with formating */

} /* digi_debug_generic_ilkn_handle_state_header_print */




/*******************************************************************************
* digi_util_int_field_is_active
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines if a single field or array of fields has an active interrupt
*
* INPUTS:
*   digi_handle     - a pointer to the specific digi handle instance.
*   field_val_ptr   - Pointer to an interrupt field value (TRUE or FALSE)
*                     or array of fields
*   array_len       - Length of array, 1 if only single field value.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL            - TRUE:  Active interrupt found\n
*                     FALSE: No active interrupt found
*
* NOTES:
*
*******************************************************************************/

PRIVATE BOOL digi_util_int_field_is_active(digi_handle_t    *digi_handle,
                                           BOOL8            *field_val_ptr,
                                           UINT32            array_len)
{
    BOOL    int_found = FALSE;
    UINT32  i;

    PMC_ENTRY();


    if (1 == array_len)
    {
        if (TRUE == *field_val_ptr)
        {
            int_found = TRUE;
        }
    }
    else
    {
        /* Determine if in the array there are any active interrupts */
        for (i = 0; (i < array_len) && (FALSE == int_found); i++)
        {
            if (TRUE == field_val_ptr[i])
            {
                int_found = TRUE;
            }
        }
    }

    PMC_RETURN(int_found);
} /* digi_util_int_field_is_active */





/*******************************************************************************
* digi_util_int_field_print
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Formats and prints an interrupt table field.
*
* INPUTS:
*   digi_handle         - a pointer to the specific digi handle instance.
*   field_val_ptr       - Pointer to an interrupt field value (TRUE or FALSE)
*                         or array of fields
*   array_len           - Length of array, 1 if only single field value.
*   field_name_str_ptr  - Field name string
*   array_len_str_ptr   - Array len string
*   print_type          - DIGI_UTIL_INT_PRINT_ALL to print all status fields,
*                         DIGI_UTIL_INT_PRINT_ACTIVE to only print active fields,
*                         DIGI_UTIL_INT_PRINT_NONE to print nothing and only
*                         return TRUE/FALSE in int_found_ptr
*   format_ptr          - Structure describing formatting options such as 
*                         column width and number of columns
*
* OUTPUTS:
*   int_found_ptr       - Outputs TRUE of an active interrupt was found.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_util_int_field_print(digi_handle_t            *digi_handle,
                                       BOOL8                    *field_val_ptr,
                                       UINT32                    array_len,
                                       char                     *field_name_str_ptr,
                                       char                     *array_len_str_ptr,
                                       digi_util_int_print_t     print_type,
                                       digi_util_int_format_t   *format_ptr,
                                       BOOL                     *int_found_ptr)
{
    UINT32      i;
    UINT32      array_value = 0;                                         
    UINT32      field_name_len, bracket_len, index_len;
    UINT32      field_width, value_width, num_cols;
    BOOL        field_active = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL,         DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(array_len > 0,               DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ASSERT(field_val_ptr != NULL,       DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(field_name_str_ptr != NULL,  DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(int_found_ptr != NULL,       DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(print_type < LAST_DIGI_UTIL_INT_PRINT, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT((format_ptr != NULL) || (DIGI_UTIL_INT_IS_PRINT_NONE(print_type)), DIGI_ERR_NULL_HANDLE, 0, 0);



  

    /* Determine if interrupt is active */
    if (digi_util_int_field_is_active(digi_handle, field_val_ptr, array_len))
    {
        field_active   = TRUE;
        *int_found_ptr = TRUE;
    }
    
    /* If not printing then just exit */
    if (DIGI_UTIL_INT_IS_PRINT_NONE(print_type))
    {
        PMC_RETURN();
    }

    
    
    /* Determine if we want to print */
    if(DIGI_UTIL_INT_IS_PRINT_ALL(print_type) ||
       (TRUE == field_active))
    {
        field_width = format_ptr->field_width;
        value_width = format_ptr->value_width;
        num_cols    = format_ptr->num_columns;

        if (1 == array_len)
        {
                DIGI_UTIL_PRINT("%-*.*s %#*x",
                                field_width, field_width,
                                field_name_str_ptr,
                                value_width,
                                *field_val_ptr);
                DIGI_UTIL_INT_PRINT_NEW_LINE_CHECK();                               

        }
        else
        {
            /* Create a hex representation of the array */
            for (i = 0; i < array_len; i++)
            {                         
                array_value |= (field_val_ptr[i] << i);     
            }                                                             
            
            PMC_MEMSET(str_buf, 0, DIGI_UTIL_INT_STR_BUF_LEN);
            PMC_ASSERT(array_len_str_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

            /* Check that we do not overrun our str_buf */
            field_name_len  = PMC_STRLEN(field_name_str_ptr);
            bracket_len     = 2; 
            index_len       = PMC_STRLEN(array_len_str_ptr);
            PMC_ASSERT((field_name_len + bracket_len + index_len) < DIGI_UTIL_INT_STR_BUF_LEN, DIGI_ERR_CODE_ASSERT, 0, 0);

            /* Construct the field_name with array index */
            PMC_SPRINTF(str_buf, "%s[%d]", field_name_str_ptr, array_len);

            
            DIGI_UTIL_PRINT("%-*.*s %#*x",
                            field_width, field_width,
                            str_buf,
                            value_width,
                            array_value);                  
            DIGI_UTIL_INT_PRINT_NEW_LINE_CHECK();                               
        }
    }

    PMC_RETURN();

} /* digi_util_int_field_print */


/* 
** Private Functions END 
*/

/* LCOV_EXCL_STOP */

/*
** End of file
*/
