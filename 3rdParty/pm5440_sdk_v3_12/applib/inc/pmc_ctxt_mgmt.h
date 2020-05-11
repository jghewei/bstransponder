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
#ifndef PMC_CTXT_MGMT_H
#define PMC_CTXT_MGMT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
** Include Files
*/

/*
** Enumerated Types
*/

/*
** Constants
*/
/*Define the size of memory to calloc*/
#define PMC_CTXT_DIGI_HANDLE_SIZE    (1400832L)


#define PMC_CTXT_NUM_REG_PTRS           (5200)
#define PMC_CTXT_PTR_REGISTRY_SIZE (PMC_NUM_REG_PTRS * sizeof(void *))

#define PMC_CTXT_NUM_CALLOC_CALLS (1269)

/*
** Macro Definitions
*/
#undef PMC_PRINT_CALLOC_TO_FILE    /* useful for debug */


/* only define if not building for verification, not sure if this 
 * break something for in the c/verif interaction so disable contiguous
 * context memory allocation for now.*/
#define USE_PMC_CTXT_CALLOC


/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: pmc_ctxt_mgmt_header_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the metadata used for the current context.
*
* ELEMENTS:
*   total_size          - total size of the context, including all metadata.
*   sw_version          - the software version
*   ptr_reg             - pointer to the start of the pointer registry.
*   digi_hdl            - pointer to the start of the digi_handle and the rest
*                           of the context data.
*   mem_pool            - pointer to the mem pool used to allocate context
*                           memory.  This pointer probably doesn't need to be 
*                           stored here.
*   next_calloc_ptr     - pointer to the next available slot in the digi handle
*                           memory block.
*   ctxt_calloc_cntr    - context calloc/free tracking counter
*   f                   - file handle used for tracking callocs
*   ctxt_ptr_registry   - pointer to the next available slot in the pointer
*                           regisry table.
*******************************************************************************/
typedef struct pmc_ctxt_mgmt_header_t 
{
    UINT32              total_size;
    UINT32              sw_version;
    /* use UINT8 * b/c pointer arithmetic not valid with void *
     * and any ptr math w.r.t. the context will deal with bytes */
    UINT8               **ptr_reg;
    UINT8               *digi_hdl;
    pmc_mem_pool_handle mem_pool;
    UINT8               *next_calloc_ptr; 
    INT32               ctxt_calloc_cntr;
#ifdef PMC_PRINT_CALLOC_TO_FILE
    FILE                *f;
#endif
    UINT8**             ctxt_ptr_registry;
 } pmc_ctxt_mgmt_header_t;

/* Context memory block
 *   +----------------------+  -
 *   |ctxt_mgmt_header      |  |
 *   +----------------------+  -
 *   | pointer registration |  PMC_CTXT_PTR_REGISTRY SIZE
 *   | table                |  V
 *   +----------------------+  -
 *   | digi handle          |  PMC_CTXT_DIGI_HANDLE_SIZE
 *   |                      |  |
 *   |                      |  |
 *   |                      |  V
 *   +----------------------+  -
 */
/*******************************************************************************
* STRUCTURE: pmc_ctxt_mgmt_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A struct to hold the context for the current software.
*
* ELEMENTS:
*   header              - The context metadata.
*   ptr_reg             - The pointer registry.
*   data                - The context itself.
*******************************************************************************/
typedef struct pmc_ctxt_mgmt_ctxt_t 
{
    pmc_ctxt_mgmt_header_t  header;
    UINT8*                  ptr_reg[PMC_CTXT_NUM_REG_PTRS];
    UINT8                   data[PMC_CTXT_DIGI_HANDLE_SIZE];
} pmc_ctxt_mgmt_ctxt_t;


/*******************************************************************************
* STRUCTURE: pmc_ctxt_mgmt_header_305_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure is for the prior contexts used in earlier software 
*   releases. It is used to calculate the size. Note that the context does
*   contain a total_size field in the header that is recommended for use.
*
* ELEMENTS:
*   total_size          - total size of the context, including all metadata.
*   sw_version          - the software version
*   ptr_reg             - pointer to the start of the pointer registry.
*   digi_hdl            - pointer to the start of the digi_handle and the rest
*                           of the context data.
*   mem_pool            - pointer to the mem pool used to allocate context
*                           memory.  This pointer probably doesn't need to be 
*                           stored here.
*   next_calloc_ptr     - pointer to the next available slot in the digi handle
*                           memory block.
*   ctxt_calloc_cntr    - context calloc/free tracking counter
*   f                   - file handle used for tracking callocs
*   ctxt_ptr_registry   - pointer to the next available slot in the pointer
*                           regisry table.
*******************************************************************************/
typedef struct pmc_ctxt_mgmt_header_305_t 
{
    UINT32              total_size;
    UINT32              sw_version;
    /* use UINT8 * b/c pointer arithmetic not valid with void *
     * and any ptr math w.r.t. the context will deal with bytes */
    UINT8               **ptr_reg;
    UINT8               *digi_hdl;
    pmc_mem_pool_handle mem_pool;
    UINT8               *next_calloc_ptr; 
    INT32               ctxt_calloc_cntr;
#ifdef PMC_PRINT_CALLOC_TO_FILE
    FILE                *f;
#endif
    UINT8**             ctxt_ptr_registry;
 
} pmc_ctxt_mgmt_header_305_t;

/*******************************************************************************
* STRUCTURE: pmc_ctxt_mgmt_ctxt_305_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure is for the prior contexts used in earlier software 
*   releases. It is used to calculate the size. Note that the context does
*   contain a total_size field in the header that is recommended for use.
*
* ELEMENTS:
*   header              - The context metadata.
*   ptr_reg             - The pointer registry.
*   data                - The context itself.
*******************************************************************************/
typedef struct pmc_ctxt_mgmt_ctxt_305_t 
{
    pmc_ctxt_mgmt_header_305_t  header;
    UINT8*                  ptr_reg[PMC_CTXT_NUM_REG_PTRS];
    UINT8                   data[PMC_CTXT_DIGI_HANDLE_SIZE];
} pmc_ctxt_mgmt_ctxt_305_t;

typedef pmc_ctxt_mgmt_ctxt_305_t pmc_ctxt_mgmt_ctxt_306_t; /*!< No changes 3.05 to 3.06 */
typedef pmc_ctxt_mgmt_ctxt_306_t pmc_ctxt_mgmt_ctxt_307_t; /*!< No changes 3.06 to 3.07 */
typedef pmc_ctxt_mgmt_ctxt_307_t pmc_ctxt_mgmt_ctxt_308_t; /*!< No changes 3.07 to 3.08 */
typedef pmc_ctxt_mgmt_ctxt_308_t pmc_ctxt_mgmt_ctxt_309_t; /*!< No changes 3.08 to 3.09 */
typedef pmc_ctxt_mgmt_ctxt_309_t pmc_ctxt_mgmt_ctxt_310_t; /*!< No changes 3.09 to 3.10 */
typedef pmc_ctxt_mgmt_ctxt_310_t pmc_ctxt_mgmt_ctxt_311_t; /*!< No changes 3.10 to 3.11 */
typedef pmc_ctxt_mgmt_ctxt_310_t pmc_ctxt_mgmt_ctxt_312_t; /*!< No changes 3.11 to 3.12 */

/*
** Global Variables
*/

/*
** Function Prototypes
*/
PUBLIC void *pmc_ctxt_mem_create(pmc_sys_handle_t *sys_handle,
                                 BOOL8 is_restart);
PUBLIC void pmc_ctxt_mem_init(void *ctxt_handle);
PUBLIC void pmc_ctxt_mem_destroy(void *ctxt_header);
PUBLIC void *pmc_ctxt_calloc_function(UINT32 size, void *ctxt_header, const char* func_name, UINT32 line_number);
PUBLIC void pmc_ctxt_free_function(void **ptr, void *ctxt_header, const char* func_name);
PUBLIC void *pmc_ctxt_mgmt_digi_handle_get(void *ctxt_header);
PUBLIC void pmc_ctxt_ptr_register(void* ptr, void *ctxt_header);
PUBLIC void *pmc_ctxt_look_for_registered_ptr(void* ptr, void *ctxt_header);
PUBLIC void pmc_ctxt_log_unregistered_pointers(void *ctxt_header);
PUBLIC void pmc_ctxt_update_pointers(void *ctxt_header);
PUBLIC void pmc_ctxt_adjust_context(void* ctxt_header, UINT8 *shift_point, PMC_PTRDIFF offset);
PUBLIC UINT32 pmc_ctxt_size_get(void *ctxt_header);
PUBLIC void pmc_ctxt_set_sw_version(void *ctxt_header, UINT32 sw_version);
PUBLIC PMC_ERROR pmc_ctxt_check_sanity(void *ctxt_header);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PMC_CTXT_MGMT_H */

