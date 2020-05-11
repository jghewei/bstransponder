/*******************************************************************************
*  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*  DESCRIPTION:
*     This file contains the definitions of the porting interface fucntions for
*     the PMC customer board and can be used as an example for customer
*     implementation.
*
*  NOTES:
*
*******************************************************************************/

#include "pmc_customer_os.h"
#include "pmc_sys.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define DIGI_IOCTL_MAGIC 0xbb
#define DIGI_IOCTL_READ_DMA_PHY_ADDR         _IOR(DIGI_IOCTL_MAGIC, 1, unsigned long)
 
#define GENDRV_OK   (0)
#define GENDRV_ERR  (1)

/* These numbers are used in mmap offset field to indicate mmap operation
to kernel. Userland must multiply this number with page size and pass it
as mmap offset. Note that mmap odffset is not used to its real pupose -- it
is used more like a flag here. */

/* Remap device register map into userland. */
#define GENDRV_REMAP_DEV_RESOURCES    (0)

/* Remap shared memory, allocated by kernel into userland. */
#define GENDRV_REMAP_SHMEM            (1)

/* Keep it on PAGE_SIZE boundary. 4K is most common page size, but some platforms
   have variable page size. In our case, 4K is probably good enough. Do not
   introduce PAGE_SIZE here, because userland does not know what that is.
   Do not use getpagesize(), because places using fixed size arrays will
   fail. To avoid all that trouble, keep it simple using bit of a guesswork. */
#define GENDRV_DEV_DMA_BUF_SZ   (4096)

/* Number of DMA buffers per device. It would be nice to keep it on 2^n boundary. */
#define GENDRV_DEV_NUM_DMA_BUFS (1024)

/* Size of the shared buffer storage (per device, in bytes). */
#define GENDRV_DEV_SHARED_MEM_DATA_STORE_SZ \
        (GENDRV_DEV_NUM_DMA_BUFS * GENDRV_DEV_DMA_BUF_SZ)
        
/*******************************************************************************
           gendrv_intf.h kernel interface file data end
 
 *******************************************************************************
 *******************************************************************************
*******************************************************************************/

/* Smallest buffer which could be allocated (in bytes). Note that all other
   buffers must be multiple of this buffer size. This will be done silently
   without any intervention from the caller.
*/
#define PMC_MEM_POOL_BUF_SIZE_MIN   (0x100)


/* Single buffer descriptor. It will be always allocated, but will be "freed"
   by clearing "used" bit in flags.
 */
#define PMC_MEM_POOL_USED_BIT  (1 << 0)

/* Max length of the digi device name. */
#define PMC_DEVNAME_MAX    (31)

/* Interrupt Aggregator Outbound Interrupt Line Status Register. If you change
   this register address here, you must change is in digi_loc.h, too. */
#define PCIE_OB_IRQ_STAT_REG    (0x01809364)

/* Number of itmes allowed in the event table (per DIGI device). */
#define PMC_DEV_EVENT_TABLE_NUM_ENTRIES    (1)

/* The number of digi devices used in multi-digi testing in customer*/
#define PMC_CUST_NUM_DIGI_DEVICES (1)


/*******************************************************************************
* STRUCTURE: pmc_event_table_t
*______________________________________________________________________________
*
* DESCRIPTION:
*   Event table structure.
*
* ELEMENTS:
*   See typedef
******************************************************************************/
typedef struct pmc_event_table
{
    BOOL used; /*!< Set to 1 if entry is used */
    INT8 devname[PMC_DEVNAME_MAX + 1]; /*!< device name */
    INT32 fd_A; /*!< asynchronous device file descriptor */
    pmc_event_id_t event_id; /*!< registered event */
    UINT32 resource; /*!< Resource to monitor. This can be simple register only */
    pmc_sys_handle_t *sys_handle; /*!< System handle */
    void (*cb)(void *); /*!< callback associated with this event */
} pmc_event_table_t;


/*******************************************************************************
* STRUCTURE: pmc_mem_pool_buf_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Single memory pool buffer.
*
* ELEMENTS:
*   flags  - various buffer flags. Bit 0 is buffer used bit. If set to '1', 
*            buffer is used. 
*   buf_ptr  - pointer to allocated pool buffer.
****************************************************************************/
typedef struct pmc_mem_pool_buf
{
    UINT32 flags;
    void *buf_ptr;
} pmc_mem_pool_buf_t;


/*******************************************************************************
* STRUCTURE: pmc_mem_pool_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Single memory pool descriptor.
*
* ELEMENTS:
*   flags  - various buffer flags. Bit 0 is buffer used bit. If set to '1', 
*            buffer is used.
*   sys_handle  - system handle.
*   buf_size - size of the buffer in this memory pool (in bytes)
*   num_buf - number of buffers of size buf_size in this pool.
*   mem_type - memory type. Passed via pmc_mem_pool_create.
*   alignment - byte alignment. Passed via pmc_mem_pool_create.
*   buf_array_ctrl_ptr - control structure for pool buffer array.
*
****************************************************************************/
typedef struct pmc_mem_pool
{
    UINT32 flags;
    pmc_sys_handle_t sys_handle;
    UINT32 buf_size;
    UINT32 num_buf;
    UINT16 mem_type;
    pmc_mem_align_enum alignment;
    pmc_mem_pool_buf_t *buf_array_ctrl_ptr;
} pmc_mem_pool_t;


/*******************************************************************************
* STRUCTURE: pmc_mem_pool_list_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Memory pool list.
*
* ELEMENTS:
*   initialized - Set to 1 if memory pools are initialized.
*   mem_max - Maximum memory size (in bytes), memory pool buffers could
*             occupy.
*   mem_free - Memory left (in bytes) for pool allocation.
*   first_free - Pointer to first free address which could be used for mem
*                pool.
*   buf_size_min - Minimum size of the memory pool buffer.
*   num_tbl_entries - Max number of memory pools.
*   mem_pool_tbl_ptr - Memory pool talbe pointer.
*
****************************************************************************/
typedef struct pmc_mem_pool_list
{
    BOOL initialized;
    UINT32 mem_max;
    UINT32 mem_free;
    void  *first_free;
    UINT32 buf_size_min;
    UINT32 num_tbl_entries;
    pmc_mem_pool_t *mem_pool_tbl_ptr;
} pmc_mem_pool_list_t;

/* Private Data */


/*! Static table the is used to reference the digi device name to the
   device locations */
PRIVATE const char *pmc_cust_digi_device_map[PMC_CUST_NUM_DIGI_DEVICES][2] = {
    {"DIGI_0", "/dev/digi0"},
};

/*! Global mutex used to limit access to logging related functions */
PRIVATE pthread_mutex_t pmc_global_log_mutex;
/*! Flag indicating pmc_log_init has already been called */
PRIVATE BOOL8 pmc_global_log_mutex_init = FALSE;


/* Private Function Declarations */
PRIVATE PMC_ERROR customer_munmap(void *addr_ptr, int size);
PRIVATE volatile void *customer_mmap(char *filename_ptr, int size, int action);
PRIVATE void parse_dev_name(INT8 *dest, INT8 *src, UINT16 max_len);
PRIVATE UINT32 extract_resource(char *buf);
PRIVATE const char *pmc_cust_digi_device_name_get(const char* name);
PRIVATE UINT8 pmc_cust_digi_device_name_to_idx(const char *dev_name);
PRIVATE UINT64 pmc_calc_timeval_diff(struct timeval *tv_start, struct timeval *tv_end);



/*******************************************************************************
*   customer_sys_handle_create
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Create a handle for reading and writing device registers.
*      This function is called by pmc_sys_handle_create().
*
*  INPUTS:
*      None
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*       pmc_sys_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC pmc_sys_handle_t *customer_sys_handle_create(void)
{
    customer_sys_handle_t *cust_sys;

    PMC_ENTRY();

    cust_sys = (customer_sys_handle_t*)PMC_CALLOC(sizeof(customer_sys_handle_t));

    PMC_RETURN((pmc_sys_handle_t *)cust_sys);
}

/*******************************************************************************
*  customer_sys_handle_destroy
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Free memory associated with the customer handle
*
*  INPUTS:
*      sys_handle - double pointer to sys_handle.
*
*  OUTPUTS:
*      sys_handle - set to NULL
*
*  RETURNS:
*      None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void customer_sys_handle_destroy(pmc_sys_handle_t **sys_handle)
{
    customer_sys_handle_t **cust_sys;

    PMC_ENTRY();

    cust_sys = (customer_sys_handle_t**)sys_handle;
    customer_munmap((*cust_sys)->base_addr_ptr, (*cust_sys)->mmap_size);
    customer_munmap((*cust_sys)->shmem_mmap_ptr, GENDRV_DEV_SHARED_MEM_DATA_STORE_SZ * PMC_CUST_NUM_DIGI_DEVICES);
    PMC_FREE(sys_handle);

    PMC_RETURN();
}

/*******************************************************************************
*  customer_sys_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Initialize the sys_handle by mapping the device into user space
*      using information in *name as the index to the device to access
*
*  INPUTS:
*      sys_handle - double pointer to sys_handle.
*      sys_cfg_ptr - information used to determine which device to map -
*                    not used by PMC_CUSTATION.
*      name_ptr - name of the device to map "DIGI_0", "DIGI_1", ...
*                 in the case of the PMC_CUSTATION
*
*  OUTPUTS:
*      sys_handle - initialized
*
*  RETURNS:
*      PMC_SUCCESS
*      PMC_SHARED_ERR_MMAP_FAILED - failure to mmap the device
*******************************************************************************/
PUBLIC PMC_ERROR customer_sys_init(pmc_sys_handle_t *sys_handle,
                                    void *base_addr_ptr,
                                    const char *name_ptr)
{
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;
    PMC_ERROR ret = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT((name_ptr != NULL),PMC_SHARED_ERR_CODE_NULL_PTR,0,0);
    cust_sys->dev_name_ptr = pmc_cust_digi_device_name_get(name_ptr);
    PMC_ASSERT((cust_sys->dev_name_ptr != NULL),PMC_SHARED_ERR_CODE_NULL_PTR,0,0);

    PMC_LOG_TRACE("PMC_CUSTATION.C: Device name = %s, MMAP_SIZE = 0x%x\n", cust_sys->dev_name_ptr, DIGI_CUST_MMAP_SIZE);

    /* Example operations to capture base_addr and name of device
       This is where PCIe operations would be performed to identify the device
       based on base_addr_ptr, name_ptr and put additional information into
       customer_sys_handle_t to be used for future device access operations. */
    cust_sys->base_addr_ptr = base_addr_ptr;
    cust_sys->mmap_size = DIGI_CUST_MMAP_SIZE;

    if (NULL != name_ptr)
    {
        size_t size = strlen(name_ptr);
            
        if (size > CUST_MAX_DIGI_NAME_SIZE)
        {
            size = CUST_MAX_DIGI_NAME_SIZE;
        }
        strncpy(cust_sys->name_ptr, name_ptr, size);
        cust_sys->name_ptr[size] = '\0';
    }

    cust_sys->last_write_addr = 0xFFFFFFFF;
    PMC_RETURN(ret);
}

/*********************************************************************
* pmc_mem_init
* ____________________________________________________________________
*
* DESCRIPTION:
*   System initialization of shared memory management.  This function
*   allocates memory for the memory pool control structures in the
*   whole system.  This function must be called before any
*   memory functions can be used. This function must be called AFTER
*   call to customer_sys_init .
*
* INPUTS:
*   sys_handle - pointer to system handle.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None if successful, otherwise, assert and never return.
*
* NOTES:
*
**********************************************************************/
PUBLIC void pmc_mem_init(pmc_sys_handle_t *sys_handle)
{
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;
    pmc_mem_pool_list_t *pmc_mem_pool_ptr = NULL;
    //gendrv_msg drv_msg;
   // gendrv_msg * drv_msg_ptr = &drv_msg;
    int ret = 0;
    int fd = -1;
    PMC_PTRDIFF offset;
    int total_shared_mem;
    UINT32 phyAddr = 0x12345678;

    PMC_ENTRY();

    /* Allocate DMA buffer pool. Size must be in 32-bit words, but allocation
     * must be in bytes. 
     * Note that we're allocating enough memory for all possible DIGIs.  This is
     * because what we're actually getting a pointer to is the start of the
     * shared memory used by all the DIGIs.  We will add an offset for our DIGI
     * instance.
     */
    total_shared_mem = GENDRV_DEV_SHARED_MEM_DATA_STORE_SZ * PMC_CUST_NUM_DIGI_DEVICES;
    cust_sys->shmem_dma_pool_size = GENDRV_DEV_SHARED_MEM_DATA_STORE_SZ/sizeof(UINT32);
    cust_sys->shmem_mmap_ptr = (void *)customer_mmap((char *)cust_sys->dev_name_ptr,
                                                          total_shared_mem,
                                                          GENDRV_REMAP_SHMEM);

    PMC_ASSERT((NULL != cust_sys->shmem_mmap_ptr),
               PMC_SHARED_ERR_MMAP_FAILED, 0, 0);

    /* Add the offset for this particular DIGI instance */
    //offset = pmc_emul_digi_device_name_to_idx(cust_sys->dev_name_ptr) * GENDRV_DEV_SHARED_MEM_DATA_STORE_SZ;
    offset = 0 * GENDRV_DEV_SHARED_MEM_DATA_STORE_SZ;
    cust_sys->shmem_dma_pool_ptr = (void *)((PMC_PTRDIFF)cust_sys->shmem_mmap_ptr + offset);


    /* Ask kernel for physical address of the dma memory pool. */
    fd = open(cust_sys->dev_name_ptr, O_RDWR|O_SYNC);
    PMC_ASSERT((fd >= 0), PMC_SHARED_ERR_DEVOPEN_FAILED, 0, 0);
    /*PMC_MEMSET(drv_msg_ptr, 0, sizeof(drv_msg));
    drv_msg.msg_type = GENDRV_MSG_TYPE_NORMAL;
    drv_msg.oper = GENDRV_MSG_OPER_GET_SHM_PHYS_ADDR;*/
    //ret = ioctl(fd, DIGI_IOCTL_READ_DMA_PHY_ADDR, &phyAddr);
    ret = 1;phyAddr = 0x1e000000;   // change by xxu1
    if(ret >= 0)
    {
        /* Physical address of the DMA memory pool. */
        //cust_sys->shmem_dma_pool_phys = drv_msg.payload.num + offset; 
        cust_sys->shmem_dma_pool_phys = phyAddr + offset;  

        /* Allocate control structure for memory pools. */
        pmc_mem_pool_ptr = (pmc_mem_pool_list_t*)PMC_MALLOC(sizeof(pmc_mem_pool_list_t));
        pmc_mem_pool_ptr->initialized = FALSE;
        pmc_mem_pool_ptr->mem_max = 0;
        pmc_mem_pool_ptr->mem_free = 0;
        pmc_mem_pool_ptr->first_free = NULL;    
        pmc_mem_pool_ptr->buf_size_min = PMC_MEM_POOL_BUF_SIZE_MIN;
        pmc_mem_pool_ptr->num_tbl_entries = 0;
        pmc_mem_pool_ptr->mem_pool_tbl_ptr = NULL;

        cust_sys->pmc_mem_pool_ptr = (void *)pmc_mem_pool_ptr;
        printf("shmem_dma_pool_ptr:%p shmem_dma_pool_phys:0x%llx pmc_mem_pool_ptr:%p\n",cust_sys->shmem_dma_pool_ptr, cust_sys->shmem_dma_pool_phys, cust_sys->pmc_mem_pool_ptr);
    }
    else
    {   
        PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_DEVIOCTL_FAILED, 0, 0, "Shared DMA memory pool ioctl failed.\n");
        customer_munmap(cust_sys->shmem_mmap_ptr, total_shared_mem);
        cust_sys->shmem_dma_pool_ptr = NULL;
        cust_sys->pmc_mem_pool_ptr = NULL;
    }

    close(fd);

    PMC_RETURN();
}


/*******************************************************************************
* pmc_mem_pool_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates a buffer pool for the specified number and size of buffers in the
*   specified memory region, aligned on the specified byte boundary.
*
* INPUTS:
*   pmc_sys_handle      - Pointer to sys handle
*   buf_size            - Size of buffers in the pool. (All buffers in the
*                         same pool have the same size.)
*   num_buf             - Number of buffers in the pool.
*   mem_type            - Memory type (always 0)
*   alignment           - Requested byte alignment of allocated buffers
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   pmc_mem_pool_handle - Handle to newly created memory pool.
*
* NOTES:
*
*******************************************************************************/
PUBLIC pmc_mem_pool_handle pmc_mem_pool_create(pmc_sys_handle_t pmc_sys_handle,
                                               UINT32 buf_size,
                                               UINT32 num_buf,
                                               UINT16 mem_type,
                                               pmc_mem_align_enum alignment)
{
    UINT32 tmp_var;
    UINT32 tmp_var2;
    BOOL found;
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)pmc_sys_handle;
    pmc_mem_pool_list_t *pmc_mem_pool_ptr = (pmc_mem_pool_list_t *)(cust_sys->pmc_mem_pool_ptr);
    pmc_mem_pool_handle ret = NULL;
    PMC_PTRDIFF  padding;
    PMC_PTRDIFF alignment_size;


    PMC_ENTRY();


    PMC_ASSERT(NULL != cust_sys, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(NULL != pmc_mem_pool_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(0 != buf_size, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);
    PMC_ASSERT(0 != num_buf, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* Make sure that the buf_size is multiple of minimum buffer size. */
    tmp_var = buf_size % pmc_mem_pool_ptr->buf_size_min;
    if(tmp_var != 0)
    {
        tmp_var = buf_size/pmc_mem_pool_ptr->buf_size_min;
        tmp_var++;
        buf_size = tmp_var * pmc_mem_pool_ptr->buf_size_min;
    }

    /* Make sure everything is initialized if there was no use of the shared memory previously. */
    if(FALSE == pmc_mem_pool_ptr->initialized)
    {
        /* Make sure we are pointing to correct shared memory. */
        pmc_mem_pool_ptr->first_free = cust_sys->shmem_dma_pool_ptr;
        pmc_mem_pool_ptr->mem_max = (cust_sys->shmem_dma_pool_size) * sizeof(UINT32);
        pmc_mem_pool_ptr->mem_free = pmc_mem_pool_ptr->mem_max;
        pmc_mem_pool_ptr->num_tbl_entries = pmc_mem_pool_ptr->mem_max/PMC_MEM_POOL_BUF_SIZE_MIN;

        /* This allocation is for table control structure. */
        pmc_mem_pool_ptr->mem_pool_tbl_ptr = (pmc_mem_pool_t*)PMC_MALLOC(sizeof(pmc_mem_pool_t) * pmc_mem_pool_ptr->num_tbl_entries);

        for(tmp_var = 0; tmp_var < pmc_mem_pool_ptr->num_tbl_entries; tmp_var++)
        {
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->flags = 0;
        }

        pmc_mem_pool_ptr->initialized = TRUE;
    }

    /* Alignment
     * Note that the alignment is the alignment relative to the base shared
     * memory address.  This is done to ensure that the physical address which
     * results from subtracting the base shared memory address and adding the
     * base physical address respects the alignement.  Of course, this also
     * assumes that the base physical address respects the largest required
     * alignment.
     */
    alignment_size = (1 << (PMC_PTRDIFF)alignment);
    padding = ((PMC_PTRDIFF)pmc_mem_pool_ptr->first_free - (PMC_PTRDIFF)cust_sys->shmem_dma_pool_ptr) % alignment_size;

    if (padding != 0)
    {
        padding = alignment_size - padding;
        pmc_mem_pool_ptr->mem_free -= padding;
        pmc_mem_pool_ptr->first_free = (void *)((PMC_PTRDIFF)pmc_mem_pool_ptr->first_free + padding);
    }

    /* Not enough free memory. */
    PMC_ASSERT((buf_size * num_buf) <= pmc_mem_pool_ptr->mem_free, PMC_SHARED_ERR_NO_MEMORY, 0, 0);

    /* Grab shared memory, but find free spot in the table first. */
    found = FALSE;
    for(tmp_var = 0; (tmp_var < pmc_mem_pool_ptr->num_tbl_entries) && (FALSE == found); tmp_var++)
    {
        if(0 == (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->flags)
        {
            found = TRUE;

            /* Empty spot for requested pool is found. */
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->flags = 0;
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->flags |= PMC_MEM_POOL_USED_BIT;
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->sys_handle = pmc_sys_handle;
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_size = buf_size;
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->num_buf = num_buf;
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->mem_type = mem_type;
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->alignment = alignment;
            (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_array_ctrl_ptr = (pmc_mem_pool_buf_t*)PMC_MALLOC(sizeof(pmc_mem_pool_buf_t) * num_buf);

            /* Initialize memory pool buffers. */
            for(tmp_var2 = 0; tmp_var2 < num_buf; tmp_var2++)
            {
                ((pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_array_ctrl_ptr + tmp_var2)->flags = 0;
                /* If this is context memory, we can do something different.  By
                 * default, we'll still use the shared mem
                 */
                if (PMC_MEM_POOL_TYPE_CONTEXT == mem_type)
                {
#ifdef PMC_CTXT_IN_SHMEM
                    ((pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_array_ctrl_ptr + tmp_var2)->buf_ptr = (UINT8 *)pmc_mem_pool_ptr->first_free + buf_size * tmp_var2;
#else
                    ((pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_array_ctrl_ptr + tmp_var2)->buf_ptr = (UINT8 *)PMC_CALLOC(buf_size);
#endif
                }
                else
                {
                    ((pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_array_ctrl_ptr + tmp_var2)->buf_ptr = (UINT8 *)pmc_mem_pool_ptr->first_free + buf_size * tmp_var2;
                }
            }

            /* Point to the next free space. */
            /* Only need to do this if this shared memory. */
            if (PMC_MEM_POOL_TYPE_CONTEXT == mem_type)
            {
#ifdef PMC_CTXT_IN_SHMEM
                pmc_mem_pool_ptr->first_free = (UINT8 *)pmc_mem_pool_ptr->first_free + (buf_size * num_buf);
#endif
            }
            else
            {
                pmc_mem_pool_ptr->first_free = (UINT8 *)pmc_mem_pool_ptr->first_free + (buf_size * num_buf);
            }

            /* The code above already checked if there was enough free memory.
               This means that in worst case scenario, mem_free could become 0. */
            pmc_mem_pool_ptr->mem_free -= (buf_size * num_buf);

            /* Return table entry ptr to the caller. We can safely do this since
               caller does not know the data content. */
            ret = (void *)(pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var);
        }
    }

    PMC_RETURN(ret);
}

#ifdef PMC_CTXT_SERIALIZED_FILENAMES
UINT32 context_itr = 0;
char context_itr_str[6];
#endif
/*! this is a new suffix for the context_memory_###.bin file */
PRIVATE char pid_str[128] ;


/*******************************************************************************
* pmc_mem_ctxt_save
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Save the context. In this case it is written to a file.
*
* INPUTS:
*   *ctxt_ptr          - Reference to the context memory to be saved.
*   ctxt_size          - Size of the context being saved.
*   sys_handle         - sys_handle representing the DIGI instance that
*                        is being saved.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS                    - Memory was successfully saved.
*   PMC_SHARED_ERR_FILE_OPEN_FAILED - Error Opening the file for writing.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR pmc_mem_ctxt_save(void *ctxt_ptr,
                                   UINT32 ctxt_size,
                                   pmc_sys_handle_t *sys_handle)
{
#ifndef PMC_CTXT_DISABLE_SAVING
    PMC_ERROR result = PMC_SHARED_ERR_FILE_OPEN_FAILED;
    FILE *f_context;
    UINT16 file_size = strlen("context_memory.bin");
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;
    PMC_ENTRY();
    pid_t pid = getpid()  ;
    sprintf(pid_str,"%d",(int)pid) ;
    
    if ('\0' != cust_sys->name_ptr[0])
    {
        file_size += strlen(cust_sys->name_ptr);
    }
    else
    {
        file_size += 6;
    }

#ifdef PMC_CTXT_SERIALIZED_FILENAMES
    file_size += 6;
#endif

    if (file_size > 0)
    {
#ifndef PMC_CTXT_FILENAMING_REMOVE_PID
        char file_name[file_size + strlen(pid_str)];
#else
        char file_name[file_size+1];
#endif
        if ('\0' != cust_sys->name_ptr[0])
        {
        	
#ifdef PMC_CTXT_SERIALIZED_FILENAMES

            sprintf(context_itr_str, "%d", context_itr);
            sprintf(file_name, "context_memory%s_%s.bin", cust_sys->name_ptr, context_itr_str);
            context_itr++;
#else
#ifndef PMC_CTXT_FILENAMING_REMOVE_PID
            /* create context_memory_%s_pid#.bin string file name*/
            sprintf(file_name, "context_memory%s_%s.bin", cust_sys->name_ptr,pid_str);
#else

            sprintf(file_name, "context_memory%s.bin", cust_sys->name_ptr);
#endif
#endif
        }
        else
        {
            strcpy(file_name, "context_memoryNoName.bin");
        }
        f_context = fopen(file_name, "wb");

        if (NULL != ctxt_ptr && NULL != f_context)
        {
            UINT32 write_size;
            write_size = fwrite(ctxt_ptr, ctxt_size, 1, f_context);
            result = PMC_SUCCESS;
            PMC_LOG_TRACE("File WriteSize=%d, ContextSize=%d and Result=%d\n", 
                          write_size, ctxt_size, result);
            fclose(f_context);
        }
        else if (NULL != f_context)
        {
            fclose(f_context);
        }
    }
    PMC_RETURN(result);
#else
#ifdef PMC_CTXT_SERIALIZED_FILENAMES
    sprintf(context_itr_str, "%d", context_itr);
    context_itr++;
#endif
    PMC_RETURN(PMC_SUCCESS);
#endif
}  /* pmc_mem_ctxt_save */

/*******************************************************************************
* pmc_mem_ctxt_restore
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Restore the previously saved context.
*
* INPUTS:
*   *ctxt_ptr          - Allocated block of memory to hold the previously
*                         saved context.
*   ctxt_size           - Size of the context..
*   sys_handle          - sys_handle associated with the DIGI instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS               - Memory was successfully saved.
*   PMC_SHARED_ERR_FILE_OPEN_FAILED - Error Opening the file for writing.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR pmc_mem_ctxt_restore(void *ctxt_ptr,
                                      UINT32 ctxt_size,
                                      pmc_sys_handle_t *sys_handle)
{
    FILE *f_context;
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;
    PMC_ERROR result = PMC_SHARED_ERR_FILE_OPEN_FAILED;
    UINT16 file_size = strlen("context_memory.bin");

    PMC_ENTRY();

    if ('\0' != cust_sys->name_ptr[0])
    {
        file_size += strlen(cust_sys->name_ptr);
    }
    else
    {
        file_size += 6;
    }
#ifdef PMC_CTXT_SERIALIZED_FILENAMES
    file_size += 6;
#endif

    if (file_size > 0)
    {
#ifndef  PMC_CTXT_FILENAMING_REMOVE_PID
        char file_name[file_size+strlen(pid_str)];
#else 
		char file_name[file_size+1];
#endif        
		
        if ('\0' != cust_sys->name_ptr[0])
        {
#ifdef PMC_CTXT_SERIALIZED_FILENAMES

            sprintf(file_name, "context_memory%s_%s.bin", cust_sys->name_ptr, context_itr_str);
#else
#ifndef PMC_CTXT_FILENAMING_REMOVE_PID
            sprintf(file_name, "context_memory%s_%s.bin", cust_sys->name_ptr,pid_str);
#else            
	    	sprintf(file_name, "context_memory%s.bin", cust_sys->name_ptr);           
#endif           
#endif
        }
        else
        {
            strcpy(file_name, "context_memoryNoName.bin");
        }
        f_context = fopen(file_name, "rb");

        if (NULL != ctxt_ptr && NULL != f_context)
        {
            (void)fread(ctxt_ptr, ctxt_size, 1, f_context);
            fclose(f_context);
            result = PMC_SUCCESS;
        }
        else if (NULL != f_context)
        {
            fclose(f_context);
        }
    }

#ifdef PMC_CTXT_SERIALIZED_FILENAMES
    /* Check that the file is loaded; otherwise abort*/
    if (result != PMC_SUCCESS)
    {
        PMC_PRINT("Unable to open saved context file: context_memory%s_%s.bin \n", cust_sys->name_ptr, context_itr_str);
    }
    PMC_ASSERT(result == PMC_SUCCESS, PMC_SHARED_ERR_FILE_OPEN_FAILED, 0, 0);
#endif 

    PMC_RETURN(result);
}  /* pmc_ctxt_mem_restore */


/*****************************************************************************
* pmc_mem_pool_destroy_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroy all memory pools.
*
* INPUTS:
*   sys_handle - system handle.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*****************************************************************************/
PUBLIC void pmc_mem_pool_destroy_all(pmc_sys_handle_t **sys_handle)
{

    UINT32 tmp_var;
#ifndef PMC_CTXT_IN_SHMEM
    UINT32 tmp_var2;
#endif
    customer_sys_handle_t **cust_sys = (customer_sys_handle_t**)sys_handle;
    pmc_mem_pool_list_t *pmc_mem_pool_ptr = (pmc_mem_pool_list_t *)((*cust_sys)->pmc_mem_pool_ptr);

    PMC_ENTRY();

    if(NULL != (*cust_sys)->pmc_mem_pool_ptr)
    {
        /* Just in case before we start destroying... */
        pmc_mem_pool_ptr->initialized = FALSE;
    
        for(tmp_var = 0; (tmp_var < pmc_mem_pool_ptr->num_tbl_entries); tmp_var++)
        {
            if( PMC_MEM_POOL_USED_BIT == ((pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->flags & PMC_MEM_POOL_USED_BIT) )
            {
                /* If this is CALLOC'd memory, we need to free the memory before
                 * freeing the control buffer. */
#ifndef PMC_CTXT_IN_SHMEM
                if (PMC_MEM_POOL_TYPE_CONTEXT ==
                    (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->mem_type)
                {
                    for(tmp_var2 = 0; tmp_var2 < (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->num_buf; tmp_var2++)
                    {
                        PMC_FREE(&((pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_array_ctrl_ptr + tmp_var2)->buf_ptr);
                    }
                }
#endif
                /* Free control buffer. */
                (pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->flags = 0;
                PMC_FREE( &((pmc_mem_pool_ptr->mem_pool_tbl_ptr + tmp_var)->buf_array_ctrl_ptr) );
            }
        }
    
        PMC_FREE(&(pmc_mem_pool_ptr->mem_pool_tbl_ptr));
    
        PMC_FREE(&(pmc_mem_pool_ptr));
    }
    else
    {
        PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_FREE_NULL_PTR, 0, 0, "pmc_mem_pool_destroy_all: cust_sys->pmc_mem_pool_ptr is NULL.\n");
    }

    PMC_RETURN();
}

/*******************************************************************************
* pmc_mem_pool_alloc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocate from the specified pool a buffer of size buf_size.
*   buf_size must be <= buf_size specified in pool_create. Return
*   value is a pointer that can be used on host processor. Asserts on
*   failure allocate
*
* INPUTS:
*   pool        - Handle to previously created memory pool.
*   buf_size    - Required buffer size.
*
* RETURNS:
*   void *pointer to buffer
*   Assert and never return for all other failures to allocate buffer.
*
* NOTES:
*   There can not be two or more buffers allocated from the same buffer in the
*   pool.
*
*******************************************************************************/
PUBLIC void *pmc_mem_pool_alloc(pmc_mem_pool_handle pool, UINT32 buf_size)
{
    UINT32 cntr = 0;
    void *ret = NULL;
    pmc_mem_pool_t *mem_pool_tbl_ptr = (pmc_mem_pool_t *)pool;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mem_pool_tbl_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(0 != buf_size, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);
    PMC_ASSERT(buf_size <= mem_pool_tbl_ptr->buf_size, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* Find free buffer. */
    for(cntr = 0; (cntr < mem_pool_tbl_ptr->num_buf) && (NULL == ret); cntr++)
    {
        if( 0 == ( (mem_pool_tbl_ptr->buf_array_ctrl_ptr + cntr)->flags & PMC_MEM_POOL_USED_BIT) )
        {
            /* Allocate buffer. */
            (mem_pool_tbl_ptr->buf_array_ctrl_ptr + cntr)->flags = 0;
            (mem_pool_tbl_ptr->buf_array_ctrl_ptr + cntr)->flags |= PMC_MEM_POOL_USED_BIT;
            ret = (mem_pool_tbl_ptr->buf_array_ctrl_ptr + cntr)->buf_ptr;
        }
    }

    if(NULL == ret)
    {
        /* No free memory left. This is critical error. */        
        PMC_ASSERT(NULL != ret, PMC_SHARED_ERR_NO_MEMORY, 0, 0);
    }

    PMC_RETURN(ret);
}


/*********************************************************************
* pmc_mem_pool_free
* ____________________________________________________________________
*
* DESCRIPTION:
*   Free a memory buffer.  On successful free, the double pointer is
*   set to NULL before being returned.
*
* INPUTS:
*   pool     - Handle to previously created memory pool.
*   buf_pptr - Pointer to pointer to buffer to be freed.
*
* OUTPUTS:
*   buf_pptr - Pointer to buffer updated to NULL on successful free;
*              unchanged if buffer free fails.
*
* RETURNS:
*   None if successful; otherwise, assert and never return.
*
* NOTES:
*
**********************************************************************/
PUBLIC void pmc_mem_pool_free(pmc_mem_pool_handle pool, void **buf_pptr)
{
    void *buf_ptr = *buf_pptr;
    UINT32 cntr = 0;
    BOOL found_buf = FALSE;
    pmc_mem_pool_t *mem_pool_tbl_ptr = (pmc_mem_pool_t *)pool;

    PMC_ENTRY();

    PMC_ASSERT(mem_pool_tbl_ptr != NULL, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(buf_ptr != NULL, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);

    /* Find buffer and free it. */
    for(cntr = 0; (cntr < mem_pool_tbl_ptr->num_buf) && (FALSE == found_buf); cntr++)
    {
        if( (mem_pool_tbl_ptr->buf_array_ctrl_ptr + cntr)->buf_ptr == buf_ptr)
        {
            /* If this is CALLOC'd memory, strictly we need to memset the buffer
             * to 0 in case it gets used again */
#ifndef PMC_CTXT_IN_SHMEM
            if (PMC_MEM_POOL_TYPE_CONTEXT == mem_pool_tbl_ptr->mem_type)
            {
                PMC_MEMSET((mem_pool_tbl_ptr->buf_array_ctrl_ptr + cntr)->buf_ptr, 0, mem_pool_tbl_ptr->buf_size );
            }
#endif

            /* Free buffer. */
            (mem_pool_tbl_ptr->buf_array_ctrl_ptr + cntr)->flags = 0;
            found_buf = TRUE;
        }
    }

    PMC_ASSERT(TRUE == found_buf, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    buf_pptr = NULL;

    PMC_RETURN();
}


/*********************************************************************
* pmc_convert_mem_pool_addr_to_physical_addr
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function takes in a local_ptr to a block and returns
*   a physical address. This is primarily used to determine the base
*   addresses of the pools to pass to the device.\n
*
*   Note: this function may need to return a PMC_ADDR so that it can
*   be UINT32 or UINT64 depending on the customer system address size.
*
* INPUTS:
*   pool      - Handle to previously created memory pool.
*   local_ptr - pointer to a valid local memory address
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Physical address of the pointer; otherwise, assert and never return.
*
* NOTES:
*
**********************************************************************/
PUBLIC UINT64 pmc_convert_mem_pool_addr_to_physical_addr(pmc_mem_pool_handle     pool,
                                                         void                   *local_ptr)
{
    pmc_mem_pool_t          *mem_pool_tbl_ptr = (pmc_mem_pool_t *)pool;
    customer_sys_handle_t  *cust_sys;
   
    PMC_PTRDIFF              shmem_vir_base_addr;
    PMC_PTRDIFF              shmem_phys_addr;
    PMC_PTRDIFF              offset;
    PMC_PTRDIFF              phys_addr;

    PMC_ENTRY();

    PMC_ASSERT(mem_pool_tbl_ptr != NULL, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(local_ptr != NULL, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
   
    cust_sys = (customer_sys_handle_t*) mem_pool_tbl_ptr->sys_handle;
    PMC_ASSERT(cust_sys != NULL, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);

    shmem_phys_addr     = cust_sys->shmem_dma_pool_phys;
    shmem_vir_base_addr = (PMC_PTRDIFF) cust_sys->shmem_dma_pool_ptr;

    PMC_ASSERT(((PMC_PTRDIFF) local_ptr) >= shmem_vir_base_addr, PMC_SHARED_ERR_INVALID_PARAM, (UINT32)(((PMC_UINTPTR)local_ptr) & 0xFFFFFFFF),  (UINT32)(((PMC_UINTPTR)shmem_vir_base_addr) & 0xFFFFFFFF));

    offset = ((PMC_PTRDIFF) local_ptr) - shmem_vir_base_addr;
    phys_addr = shmem_phys_addr + offset;

    PMC_RETURN(phys_addr);
}


/*********************************************************************
* pmc_convert_local_addr_to_index
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function takes in a local_ptr to a block and returns an index
*   of where the block in the pool.\n
*
*   Note: it is not known if this fucntion will be necessary in the
*   memory pool operations.
*
* INPUTS:
*   pool      - Handle to previously created memory pool.
*   local_ptr - pointer to a valid local memory address
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Physical address of the pointer; otherwise, assert and never return.
*
* NOTES:
*
**********************************************************************/
PUBLIC UINT32 pmc_convert_local_addr_to_index(pmc_mem_pool_handle pool, void *local_ptr)
{
    PMC_ENTRY();
    PMC_RETURN(0);
}


/*******************************************************************************
* pmc_mem_pool_write
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function writes a block of memory from local memory at src_ptr to the
*   'pool' specified startin at dst_ptr. This function is a porting point and
*   as such the operations needed for a specific system can be performed.
*
*   For a memory mapped system this can simply be a memcpy.
*
* INPUTS:
*   pool    - Handle to previously created memory pool.
*   src_ptr - pinter to data in local memory
*   length  - number of bytes to write
*
* OUTPUTS:
*   dst_ptr - destination address (pointer or index) to put data in shared memory
*
* RETURNS:
*   None.
*
* NOTES:
*  Write will fail (assert) if data should span accross multiple buffers.
*
*******************************************************************************/
PUBLIC void pmc_mem_pool_write(pmc_mem_pool_handle pool,
                               void *dst_ptr,
                               void *src_ptr,
                               UINT32 length)
{
    UINT32 offset;
    UINT32 int_part;
    UINT32 rem;
    pmc_mem_pool_t *mem_pool_tbl_ptr = (pmc_mem_pool_t *)pool;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mem_pool_tbl_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(NULL != src_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(NULL != dst_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(0 != length, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    offset = (PMC_UINTPTR)dst_ptr - (PMC_UINTPTR)(mem_pool_tbl_ptr->buf_array_ctrl_ptr)->buf_ptr;
    int_part = offset/mem_pool_tbl_ptr->buf_size;
    rem = offset % mem_pool_tbl_ptr->buf_size;

    /* Data cannor span over buffer boundaries */
    PMC_ASSERT(rem + length <= mem_pool_tbl_ptr->buf_size, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* Write should not happen to the buffer beyond the buffer pool. */
    PMC_ASSERT(int_part < mem_pool_tbl_ptr->num_buf, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* Write should not happen to the buffer which is not allocated. */
    PMC_ASSERT(PMC_MEM_POOL_USED_BIT == ((mem_pool_tbl_ptr->buf_array_ctrl_ptr + offset)->flags & PMC_MEM_POOL_USED_BIT), PMC_SHARED_ERR_INVALID_PARAM, 0, 0);
    PMC_MEMCPY(dst_ptr, src_ptr, length);

    PMC_RETURN();
}


/*******************************************************************************
* pmc_mem_pool_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads a block of memory from the 'pool' specified starting
*   at src_ptr into local memory specified by dst_ptr. This function is a
*   porting point and as such the operations needed for a specific system can
*   be performed.
*
*   For a memory mapped system this can simply be a memcpy.
*
* INPUTS:
*   pool    - Handle to previously created memory pool.
*   src_ptr - source address (pointer or index) to get data from
*   length  - number of bytes to read
*
* OUTPUTS:
*   dst_ptr - destination pointer to put data in local memory
*
* RETURNS:
*   None.
*
* NOTES:
*   Read will fail (assert) if data should span accross multiple buffers.
*
*******************************************************************************/
PUBLIC void pmc_mem_pool_read(pmc_mem_pool_handle pool,
                              void *dst_ptr,
                              void *src_ptr,
                              UINT32 length)
{
    UINT32 offset;
    UINT32 int_part;
    UINT32 rem;
    pmc_mem_pool_t *mem_pool_tbl_ptr = (pmc_mem_pool_t *)pool;

    PMC_ENTRY();

    PMC_ASSERT(NULL != mem_pool_tbl_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(NULL != src_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(NULL != dst_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(0 != length, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    offset = (PMC_UINTPTR)src_ptr - (PMC_UINTPTR)(mem_pool_tbl_ptr->buf_array_ctrl_ptr)->buf_ptr;
    int_part = offset/mem_pool_tbl_ptr->buf_size;
    rem = offset % mem_pool_tbl_ptr->buf_size;

    /* Data cannot span over buffer boundaries */
    PMC_ASSERT(rem + length <= mem_pool_tbl_ptr->buf_size, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* Write should not happen to the buffer beyond the buffer pool. */
    PMC_ASSERT(int_part < mem_pool_tbl_ptr->num_buf, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* Write should not happen to the buffer which is not allocated. */
    PMC_ASSERT(PMC_MEM_POOL_USED_BIT == ((mem_pool_tbl_ptr->buf_array_ctrl_ptr + offset)->flags & PMC_MEM_POOL_USED_BIT),
               PMC_SHARED_ERR_INVALID_PARAM, 0, 0);
    PMC_MEMCPY(dst_ptr, src_ptr, length);

    PMC_RETURN();
}

/*******************************************************************************
* pmc_calc_timeval_diff
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Calculates the time elapsed between two timeval values.  It is assumed that
*   tv_start is earlier than tv_end.
*
* INPUTS:
*   tv_start- The earlier time value.
*   tv_end  - The later time value.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   The difference in microseconds.
*
* NOTES:
*   Does not check assumptions.
*
*******************************************************************************/
PRIVATE UINT64 pmc_calc_timeval_diff(struct timeval *tv_start, struct timeval *tv_end)
{
    UINT32 elapsed_seconds = tv_end->tv_sec - tv_start->tv_sec;
    INT64 elapsed_micro = tv_end->tv_usec - tv_start->tv_usec;
    if (elapsed_micro < 0)
    {
        --elapsed_seconds;
        elapsed_micro += 1000000;
    }
    elapsed_micro += elapsed_seconds * 1000000;

    return((UINT64) elapsed_micro);
}


/*******************************************************************************
* pmc_usleep
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A slightly more accurate version of usleep.  This version doesn't do a
*   sleep, it blocks until the specified number of microseconds has passed.
*
* INPUTS:
*   usec    - The number of microseconds to wait.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC void pmc_usleep(UINT64 usec)
{
    struct timeval tv_start;
    struct timeval tv_end;

    if (-1 == gettimeofday(&tv_start, NULL))
    {
        PMC_ASSERT(0, PMC_SHARED_ERR_CODE_ASSERT, 0, 0);
    }

    /* sleep for the specified period of time */
    if (usec > 1000)
    {
        usleep(usec);
    }

    if (-1 == gettimeofday(&tv_end, NULL))
    {
        PMC_ASSERT(0, PMC_SHARED_ERR_CODE_ASSERT, 0, 0);
    }

    while(pmc_calc_timeval_diff(&tv_start, &tv_end) < usec) {
        if (-1 == gettimeofday(&tv_end, NULL))
        {
            PMC_ASSERT(0, PMC_SHARED_ERR_CODE_ASSERT, 0, 0);
        }
    }
    /* note: function has embedded return's */
};

/*******************************************************************************
* pmc_assert_function
* ______________________________________________________________________________
*
* DESCRIPTION:
*   When using the PMC_ASSERT() macro the following function must be
*   instantiated somewhere in the code base.
*
* INPUTS:
*   error_code - error code for the assert
*   *file_ptr - filename string (to match with typical assert(), not usually
*               used in fielded system)
*   line - line number where assert occurred
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Should never return from this function
*
* NOTES:
*
*******************************************************************************/
PUBLIC void pmc_assert_function(PMC_ERROR error_code, const char *file_ptr, UINT32 line)
{
    INT32 exp_sec = 20; /* seconds */
    PMC_LOG_TRACE("PMC_ASSERT (%s:%d) - %s (0x%x)\n", file_ptr, line, pmc_log_string_get(error_code), error_code);
    PMC_PRINT("PMC_ASSERT (%s:%d) - %s (0x%x)\n", file_ptr, line, pmc_log_string_get(error_code), error_code);
#ifdef PMC_CUSTATION_COREDUMP_ASSERT
    {
        UINT32 *x = (UINT32*)3;
        *x = 3; /* should be a core dump... */
    }
#endif
#ifdef PMC_CUSTATION_QUICK_ASSERT
    exp_sec = 0;
#else
    do{
        PMC_LOG_TRACE("### ASSERTION WILL EXIT IN %d SECONDS ###\n", exp_sec);
        PMC_OS_USLEEP(1000000); /* Sleep for 1 second */
        exp_sec--;
    } while(exp_sec > 0);
#endif
    exit(EXIT_FAILURE);

} /* pmc_assert_function() */

/*******************************************************************************
*  customer_event_init
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Initialize digi event (interrupt/polling) subsystem.
*
*  INPUTS:
*      sys_handle - system handle.
*
*  OUTPUTS:
*      None.
*
*  RETURNS:
*     None.
*
* NOTES:
*     Polling operations are entirely managed at the datapath layer. These
*     functions are only required for interrupt operation.
*
*******************************************************************************/
PUBLIC void customer_event_init(pmc_sys_handle_t *sys_handle)
{
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != sys_handle, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);

    /* Allocate space for event table. PMC_CALLOC asserts on error, so we do not
       have to. */
    cust_sys->pmc_event_table_ptr = PMC_CALLOC((sizeof(pmc_event_table_t) * PMC_DEV_EVENT_TABLE_NUM_ENTRIES));

    PMC_RETURN();
}

/*******************************************************************************
*  customer_event_destroy
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Destroy digi event subsystem.
*
*  INPUTS:
*      sys_handle - system handle.
*
*  OUTPUTS:
*      None.
*
*  RETURNS:
*     None.
*
* NOTES:
*   Polling operations are entirely managed at the datapath layer. This
*   function is only required for interrupt operation.
*
*******************************************************************************/
PUBLIC void customer_event_destroy(pmc_sys_handle_t *sys_handle)
{    
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sys_handle, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);

    if(cust_sys->pmc_event_table_ptr != NULL)
    {
        PMC_FREE(&(cust_sys->pmc_event_table_ptr));
        cust_sys->pmc_event_table_ptr = NULL;
    }

    PMC_RETURN();
}

/*******************************************************************************
*  customer_digi_event_register
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to register a callback with an event. There is
*   only the PMC_EVENT_ID_OB_PCIE_IRQ_MON (top level event bit) that can be 
*   registered. This function should record and track the callback and any
*   additional information needed for RTOS/OS operations when pmc_monitor()
*   is called.
*
* INPUTS:
*   sys_handle  - system handle.
*   event_id    - event id.
*   event_cfg_par   - event configuration 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERR_FAIL if event could not be registered.
*
* NOTES:
*   Polling operations are entirely managed at the datapath layer. This
*   function is only required for interrupt operation.
*
*******************************************************************************/
PUBLIC PMC_ERROR customer_event_register(pmc_sys_handle_t *sys_handle, pmc_event_id_t event_id, void *event_cfg_par)
{
    pmc_sys_event_cfg_t *event_cfg = (pmc_sys_event_cfg_t *)event_cfg_par;
    PMC_ERROR rc = PMC_SUCCESS;
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;
    INT8 dev_node_name_A[PMC_DEVNAME_MAX + (sizeof("/dev/.X") - 1) + 1];
    INT8 driver_cmd[PMC_DEVNAME_MAX + (sizeof(" subscribe 0xYYYYYYYY") - 1) + 1];
    INT32 ret = 0;
    char *str_ptr = NULL;
    pmc_event_table_t *event_table_ptr = NULL;

    PMC_ENTRY();

    PMC_ASSERT(event_id > PMC_EVENT_ID_NONE, PMC_SHARED_ERR_INVALID_PARAM, event_id, 0);
    PMC_ASSERT(event_id < PMC_EVENT_ID_LAST, PMC_SHARED_ERR_INVALID_PARAM, event_id, 0);
    PMC_ASSERT(NULL != sys_handle, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    event_table_ptr = (pmc_event_table_t *)(cust_sys->pmc_event_table_ptr);
    PMC_ASSERT(NULL != event_table_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);

    switch(event_id)
    {
        case PMC_EVENT_ID_OB_PCIE_IRQ_MON:
        {
            UINT32 cntr;
            BOOL found_flg;

            /* Save monitor request. Find free space first. */
            cntr = 0;
            found_flg = FALSE;
            do
            {
                /* Found free entry in the event table. */
                if(FALSE == (event_table_ptr + cntr)->used)
                {
                    (event_table_ptr + cntr)->event_id = PMC_EVENT_ID_OB_PCIE_IRQ_MON;
                    (event_table_ptr + cntr)->resource = PCIE_OB_IRQ_STAT_REG;
                    (event_table_ptr + cntr)->sys_handle = sys_handle;
                    (event_table_ptr + cntr)->cb = event_cfg->cb;

                    /* We do have device name in sys_handle, but it is not very
                       convenient to parse it every time we need device name. */
                    parse_dev_name((event_table_ptr + cntr)->devname, (INT8 *)(cust_sys->dev_name_ptr), sizeof((event_table_ptr + cntr)->devname));

                    (event_table_ptr + cntr)->fd_A = -1;
                    (event_table_ptr + cntr)->used = TRUE;

                    /* We are done here. */
                    found_flg = TRUE;
                }
                else
                {
                    cntr++;
                }

            } while ((FALSE == found_flg) && ( cntr < PMC_DEV_EVENT_TABLE_NUM_ENTRIES));

            if(TRUE == found_flg)
            {

                /* Make sure that strcat works properly. */
                dev_node_name_A[0] = '\0';
                str_ptr = (char *)(&dev_node_name_A[0]);                

                /* Build device name string. */
                strcat(str_ptr, "/dev/");
                str_ptr += (sizeof("/dev/") - 1);
                strcat(str_ptr, (char *)((event_table_ptr + cntr)->devname));
                str_ptr += strlen((const char *)((event_table_ptr + cntr)->devname));
                strcat(str_ptr, ".A");

                /* Open DIGI asynchronous device. If it can not be opened,
                   there is nothing else we could do but to assert. */
                (event_table_ptr + cntr)->fd_A = open((const char *)dev_node_name_A, O_RDWR|O_SYNC);
                PMC_ASSERT((event_table_ptr + cntr)->fd_A >= 0, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

                /* Build 'driver' command (subscribe or arm). */
                driver_cmd[0] = '\0';
                str_ptr = (char *)(&driver_cmd[0]);                
                strcat(str_ptr, (char *)((event_table_ptr + cntr)->devname));
                str_ptr += strlen((const char *)((event_table_ptr + cntr)->devname));
                /* 
                 * For polling mode use subscribe command, for interrupt mode use arm commmand
                 */
                if (event_cfg->polling_mode) 
                {
                    strcat(str_ptr, " subscribe ");
                    str_ptr += (sizeof(" subscribe ") - 1);
                }
                else 
                {
                    strcat(str_ptr, " arm ");
                    str_ptr += (sizeof(" arm ") - 1);
                }
                sprintf(str_ptr, "0x%08x", (UINT32)(((event_table_ptr + cntr)->resource)/(sizeof(UINT32)))); /* sprintf() allowed in porting function */

                /* Write driver command. */
                ret = write((event_table_ptr + cntr)->fd_A, driver_cmd, strlen((const char *)driver_cmd) + 1);
                PMC_ASSERT(ret > 0, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);                
            }

            if(FALSE == found_flg)
            {
                rc = PMC_ERR_FAIL;
            }
        }
        break;

        default:
        {
            /* This simply can not happened unless it is programming error. */
            PMC_ASSERT(FALSE, PMC_SHARED_ERR_INVALID_PARAM, event_id, 0);
        }
        break;
    }

    PMC_RETURN(rc);
}


/*******************************************************************************
*  customer_event_monitor
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Monitors all events system currently is subscribed for.
*
* INPUTS:
*   sys_handle - system handle.
*   event_cfg_par  - event configuration structure. \n
*             The event loop will continue running while forever = TRUE. 
*             The caller can set event_cfg->forever = FALSE to stop the
*             event loop.  \n
*             This parameter should be set to TRUE if environment using this
*             method is multithreaded. This way system does not have to
*             repeatedly call this method.\n
*             If this parameter is set to FALSE, method will wait until it receives
*             first event update, it will take appropriate action and exit.
*             Setting this parameter to false is more suitable for testing
*             and in non-multithreaded environments.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Never returns.
*
* NOTES:
*   Polling operations are entirely managed at the datapath layer. This
*   function is only required for interrupt operation.
*   This function can be stopped by setting event_cfg->enable = FALSE.
*
*******************************************************************************/
PUBLIC void customer_event_monitor(pmc_sys_handle_t *sys_handle, void *event_cfg_par)
{
    pmc_sys_event_cfg_t *event_cfg = (pmc_sys_event_cfg_t *)event_cfg_par;
    UINT32 cntr;
    BOOL found_flg;
    ssize_t size = 0;
    char tmp_buf[128];
    UINT32 devname_len = 0;
    UINT32 resource;
    customer_sys_handle_t *cust_sys = (customer_sys_handle_t*)sys_handle;
    pmc_event_table_t *event_table_ptr = NULL;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sys_handle, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    event_table_ptr = (pmc_event_table_t *)(cust_sys->pmc_event_table_ptr);
    PMC_ASSERT(NULL != event_table_ptr, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);

    /* We can monitor device, only if there is at least one event registered for
       that device. */
    found_flg = FALSE;
    cntr = 0;
    do
    {
        if((event_table_ptr + cntr)->sys_handle == sys_handle)
        {
            found_flg = TRUE;
        }
        else
        {
            cntr++;
        }
    } while( (cntr < PMC_DEV_EVENT_TABLE_NUM_ENTRIES) && (FALSE == found_flg) );

    /* We have to assert, since this is invalid request and there is nothing
       else we could do. */
    PMC_ASSERT((TRUE == found_flg), PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    devname_len = strlen((char *)(event_table_ptr + cntr)->devname);    

    do
    {
        /* We block on read. */
        size = read((event_table_ptr + cntr)->fd_A, tmp_buf, sizeof(tmp_buf));
        if(size > 0)
        {
            /* Confirm that this is the resource we are looking for. */
            tmp_buf[size - 1] = '\0';
            if (event_cfg->polling_mode) 
            {
                resource = extract_resource(tmp_buf);
            }
            else
            {
                /* 
                   -interrupt mode returns INTERRUPT + time 
                    -there is only one resource.
                */
                
                resource = PCIE_OB_IRQ_STAT_REG;
            }

            if( (0 == strncmp((char *)(event_table_ptr + cntr)->devname, tmp_buf, devname_len)) &&
                (resource == (event_table_ptr + cntr)->resource) )
            {
                /* This is the device we were looking for, send the update
                   using provided callback. */
                (event_table_ptr + cntr)->cb(event_cfg->cb_parameter);
            }
            else
            {
                PMC_LOG(PMC_LOG_SEV_HIGH, PMC_SHARED_ERR_NOT_REGISTERED, 0, 0, "Device [%s] not registered for updates.\n", tmp_buf);
            }
        }
        else
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, PMC_SHARED_ERR_INVALID_PARAM, 0, 0, "Got zero size gendrv update, doing nothing.\n");            
        }
    
    } while(event_cfg->forever);

    PMC_RETURN();
}

/*******************************************************************************
*  customer_mmap
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Helper function to mmap() device internals (registers, DMA buffers, etc.).
*
*  INPUTS:
*      filename_ptr - device name to mmap()
*      size - memory size to mmap() (in bytes)
*      action - action to be performed (defined in shared kernel driver header
*               file).
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*      Base address of memory where the device is mmap()ed or NULL if failure.
*
*******************************************************************************/
PRIVATE volatile void *customer_mmap(char *filename_ptr, int size, int action)
{
    int fd = -1;
    volatile void *base_ptr = NULL;
    long pagesz = sysconf(_SC_PAGESIZE);
    BOOL cont = TRUE;

    PMC_ENTRY();

    if((fd = open(filename_ptr, O_RDWR|O_SYNC)) < 0 )
    {
        perror("open()");
        cont = FALSE;
    }

    if(TRUE == cont)
    {
        base_ptr = (volatile void *) mmap(NULL, size,
                                          PROT_READ|PROT_WRITE,
                                          MAP_SHARED, fd, (off_t)(pagesz * action));
        if(MAP_FAILED == base_ptr)
        {
            perror("mmap()");
            base_ptr = NULL;
            cont = FALSE;
        }
    }

    if(fd >= 0)
    {
        close(fd);
    }    

    PMC_RETURN(base_ptr);
}


/*******************************************************************************
*  customer_munmap
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Helper function to munmap() a device range.
*
*  INPUTS:
*      addr_ptr  - device address to munmmap()
*      size      - memory size to munmap()
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*      PMC_SUCCESS - unmapping successful
*      PMC_ERR_FAIL - unmapping not successful
*
* NOTES:
*     Size must be multiple of page size.
*
*******************************************************************************/
PRIVATE PMC_ERROR customer_munmap(void *addr_ptr, int size)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if(NULL != addr_ptr)
    {
        if(munmap(addr_ptr, size) < 0)
        {
            rc = PMC_ERR_FAIL;
        }
    }
    else
    {
        PMC_LOG(PMC_LOG_SEV_HIGHEST, PMC_SHARED_ERR_FREE_NULL_PTR, 0, 0, "customer_munmap: NULL pointer passed.\n");
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
}

/*******************************************************************************
*  parse_dev_name
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Parse device name needed for kernel subscription events.
*
*  INPUTS:
*      dest - parsed device name
*      src  - full device name
*      max_len - maximum number of characters to store into dest (including NULL
*      termination)
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*      None.
*
* NOTES:
*     dest will be NULL terminated. The use of sizeof instead of strlen is
*     intentional in order to let cpp resolve sizes and saving some CPU cycles
*     in the process.
*
*******************************************************************************/
PRIVATE void parse_dev_name(INT8 *dest, INT8 *src, UINT16 max_len)
{

    UINT32 cntr_src = 0;
    UINT32 cntr_dest = 0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != dest, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);
    PMC_ASSERT(NULL != src, PMC_SHARED_ERR_CODE_NULL_PTR, 0, 0);

    /* There must be space for at least single character + NULL termination. */
    PMC_ASSERT(max_len > 1, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* Name of the device is assumed to be /dev/<devname>.X . Make sure that
       device name '/dev/.X' is invalid. */
    PMC_ASSERT(strlen((const char *)src) > (sizeof("/dev/.X") - 1), PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    cntr_src = sizeof("/dev/") - 1;
    cntr_dest = 0;
    do
    {
        *(dest + cntr_dest++) = *(src + cntr_src++);
    } while(((UINT32)cntr_dest < (UINT32)(max_len - 1)) && ( *(src + cntr_src) != '.') );
    *(dest + cntr_dest) = '\0';

    PMC_RETURN();
}

/*******************************************************************************
*  extract_resource
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*      Extract resource from gendrv string.
*
*  INPUTS:
*      buf - buffer with the gendrv string. It must be NULL terminated.
*
*  OUTPUTS:
*      None
*
*  RETURNS:
*      Resource number.
*
* NOTES:
*  String is expected to be in the following format:
*      [device] UPDATE [address] [value]
*
*******************************************************************************/
PRIVATE UINT32 extract_resource(char *buf)
{
    UINT32 rc;
    char *buf_ptr = buf;
    char lbuf[16];
    char *lbuf_ptr = lbuf;
    char *endptr;

    PMC_ENTRY();

    /* Skip over device name. */
    while(*(buf_ptr++) != ' ');

    /* Skip over update string. */
    buf_ptr += (sizeof("UPDATE") - 1);

    /* Save resource string. */
    do    
    {
        *(lbuf_ptr++) = *(buf_ptr++);
    } while(*buf_ptr != ' ' );
    *lbuf_ptr = '\0';

    /* gendrv is working with UINT32 addresses. */
    rc = strtoul(lbuf, &endptr, 16);
    rc *= sizeof(UINT32);

    PMC_RETURN(rc);
}

/*******************************************************************************
*  pmc_cust_digi_device_name_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*
*  INPUTS:
*      name     - name of the device to map "DIGI_0", "DIGI_1", ...
*                 in the case of the PMC_CUSTATION
*
*  OUTPUTS:
*      device string - (return value)
*
*  RETURNS:
*      string the contains the device name string
*      NULL - returns NULL if the name of the device is not found
*******************************************************************************/
PRIVATE const char *pmc_cust_digi_device_name_get(const char* name)
{
    UINT8 i = 0;

    PMC_ENTRY();

    for (i = 0; i < PMC_CUST_NUM_DIGI_DEVICES; i++)
    {
        /* column 0 is the name of the device */
        if (0 == strcmp(name,pmc_cust_digi_device_map[i][0]))
        {
            /* column 1 is the device name to map*/
            PMC_RETURN(pmc_cust_digi_device_map[i][1]);
        }
    }

    PMC_RETURN(NULL);
}

/*******************************************************************************
*  pmc_cust_digi_device_name_to_idx
*  ___________________________________________________________________________
*
*  DESCRIPTION:  Looks up the name of the device file in the
*                   pmc_cust_digi_device_map table and returns the index.  
*                   Since this is an internal helper function, it helpfully 
*                   asserts if the device file name is not found.
*
*  INPUTS:
*      dev_name - name of the device file from the second column of
*                   pmc_cust_digi_device_map
*
*  OUTPUTS:
*      none.
*
*  RETURNS:
*      The index into the pmc_cust_digi_device_map table that matches.
*******************************************************************************/
PRIVATE UINT8 pmc_cust_digi_device_name_to_idx(const char *dev_name)
{
    UINT8 i = 0;

    PMC_ENTRY();

    for (i = 0; i < PMC_CUST_NUM_DIGI_DEVICES; i++)
    {
        /* column 1 is the name of the device file */
        if (0 == strcmp(dev_name, pmc_cust_digi_device_map[i][1]))
        {
            PMC_RETURN(i);
        }
    }

    PMC_ASSERT(FALSE, PMC_SHARED_ERR_INVALID_PARAM, 0, 0);

    /* We never get here, so it really doesn't matter what we return... */
    PMC_RETURN(0xFF);
}

/*******************************************************************************
*  customer_sys_file_open
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function opens a file. Content must be updated by user for their 
*    specific system requirements.
*
*  INPUTS:        
*    *path_ptr      - path to file
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:
*   FILE - pointer to stream, NULL on errror
*
*******************************************************************************/
PUBLIC FILE* customer_sys_file_open(const char* path_ptr)
{
    FILE *f = NULL;

    PMC_ENTRY();

    f = fopen(path_ptr, "rb");

    PMC_RETURN(f);
} /* customer_sys_file_open */

/*******************************************************************************
*  customer_sys_output_file_open
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function opens a file in write mode. 
*    Content must be updated by user for their specific system requirements.
*
*  INPUTS:        
*    *path_ptr      - path to file
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:
*   FILE - pointer to streem, NULL on errror
*
*******************************************************************************/
PUBLIC FILE* customer_sys_output_file_open(const char* path_ptr)
{
    FILE *f = NULL;

    PMC_ENTRY();

    f = fopen(path_ptr, "wb");

    PMC_RETURN(f);
}/* customer_sys_output_file_open */

/*******************************************************************************
*  customer_sys_file_close
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function closes a file. Content must be updated by user for their 
*    specific system requirements.
*
*  INPUTS:        
*     *stream_ptr  - file stream pointer
*
*  OUTPUTS:       
*     None.
*
*  RETURNS:
*   PMC_SUCCESS - on SUCCESS
*   PMC_ERR_FAIL - otherwise. 
*
*******************************************************************************/
PUBLIC PMC_ERROR customer_sys_file_close(FILE* stream_ptr)
{
    PMC_ERROR result = PMC_ERR_FAIL;

    PMC_ENTRY();

    if ((fclose(stream_ptr) == 0)) {

        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
} /* customer_sys_file_close */

/*******************************************************************************
*
*  customer_sys_file_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function reads rd_len_ptr bytes from the file at the specified pointer 
*    location. Content must be updated by user for their specific system 
*    requirements.

*
*  INPUTS:        
*    *f_ptr       - file stream pointer
*    *buffer_ptr  - buffer pointer
*    *offset_ptr  - offset into the buffer pointer
*    *rd_len_ptr  - Read length pointer
*
*  OUTPUTS:       
*    None.
*
*  RETURNS:        
*    PMC_SUCCESS on Success, failure otherwise.
*  NOTES:
*    This function does not require PMC_ENTRY or PMC_RETURN do to its high call
*    rate.
******************************************************************************/
PUBLIC PMC_ERROR customer_sys_file_read(FILE* f_ptr, UINT8* buffer_ptr, UINT32* offset_ptr, UINT8* rd_len_ptr)
{
    PMC_ERROR result = PMC_ERR_FAIL;
    int op_ret;


    op_ret = fseek(f_ptr, *offset_ptr, SEEK_SET);

    if (op_ret == 0) {

        *rd_len_ptr = fread(buffer_ptr, 1, *rd_len_ptr, f_ptr);
        result = PMC_SUCCESS;
    }
    return(result);
} /* customer_sys_file_read */

/*********************************************************************
* pmc_atomic_create
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function create and return a mutex
*   pthread is used for the implementation of the mutex.
*
* INPUTS:
*   None
*
* OUTPUTS:
*   None
*
* RETURNS:
*    A pointer to the created mutex
*
* NOTES:
* 
**********************************************************************/
PUBLIC void* pmc_atomic_create(void)
{
    pthread_mutex_t *pmc_atomic_mutex_ptr; 
    PMC_ENTRY();

    /* initialize the mutex */
    pmc_atomic_mutex_ptr = (pthread_mutex_t *) PMC_CALLOC(sizeof(pthread_mutex_t)); 
    PMC_ASSERT(pthread_mutex_init(pmc_atomic_mutex_ptr, NULL)==0,PMC_ERR_FAIL,0,0);     
    PMC_RETURN(  (void*) pmc_atomic_mutex_ptr);
} /* pmc_atomic_create */

/*********************************************************************
* pmc_atomic_delete
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function destroy an allocated mutex
*   pthread is used for the implementation of the mutex.
*
* INPUTS:
*   **mutex_pptr  -  An opaque pointer to a mutex object
*
* OUTPUTS:
*   None
*
* RETURNS:
*    A pointer to the created mutex
*
* NOTES:
* 
**********************************************************************/
PUBLIC void pmc_atomic_delete(void **mutex_pptr)
{
    UINT32 rc;
    PMC_ENTRY();
    rc = pthread_mutex_destroy((pthread_mutex_t*)*mutex_pptr);
    if (rc != 0)
    {
        PMC_LOG(PMC_LOG_SEV_HIGHEST, 0 , 0, 0,
                "pthread_mutex_destroy FAILED with error code = %d\n",rc);        
        PMC_ASSERT(rc==0,PMC_ERR_FAIL,0,0);    
    }

    PMC_FREE(mutex_pptr);

    PMC_RETURN();
} /* pmc_atomic_delete */

/*********************************************************************
* pmc_atomic_start
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function unlock a mutex. 
*   pthread is used for the implementation of the mutex.
*
* INPUTS:
*   *mutex_ptr  -  An opaque pointer to a mutex object
*
* OUTPUTS:
*   None
*
* RETURNS:
*    A pointer to the created mutex
*
* NOTES:
* 
**********************************************************************/
PUBLIC void pmc_atomic_start(void *mutex_ptr)
{
    PMC_ENTRY();
    PMC_ASSERT(pthread_mutex_lock((pthread_mutex_t*)(mutex_ptr))==0,PMC_ERR_FAIL,0,0);  
    PMC_RETURN();
} /* pmc_atomic_start */

/*********************************************************************
* pmc_atomic_end
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function unlock a mutex. 
*   pthread is used for the implementation of the mutex.
*
* INPUTS:
*   *mutex_ptr  -  An opaque pointer to a mutex object
*
* OUTPUTS:
*   None
*
* RETURNS:
*    A pointer to the created mutex
*
* NOTES:
* 
**********************************************************************/
PUBLIC void pmc_atomic_end(void *mutex_ptr)
{
    PMC_ENTRY();
    PMC_ASSERT(pthread_mutex_unlock((pthread_mutex_t*)(mutex_ptr))==0, 
               PMC_ERR_FAIL,0,0);    
    PMC_RETURN();
} /* pmc_atomic_end */

/*********************************************************************
* pmc_thread_id_get
* ____________________________________________________________________
*
* DESCRIPTION:
*   This function is used to get the unique thread id of the currently
*   running thread.
*   
* INPUTS:
*   sys_handle - pointer to system handle.
*
* OUTPUTS:
*   None
*
* RETURNS:
*    UINT32 - 32 bit unsigned thread id
*
* NOTES:
* 
**********************************************************************/
PUBLIC UINT32 pmc_thread_id_get(pmc_sys_handle_t *sys_handle)
{
    PMC_ENTRY();

    UINT32 thread_id;

    thread_id = (UINT32)pthread_self();

    PMC_RETURN(thread_id);
} /* pmc_thread_id_get() */

/*******************************************************************************
*  pmc_atomic_yield_is_allowed
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*    This function is used to indicate if yieldind is allowed. For example, it\n
*    could be implemented based on a thread id or a prioriy system.
*
*  INPUTS:
*   *sys_handle - system handle
*
*  OUTPUTS:
*    none
*
*  RETURNS:        
*    TRUE if yielding is allowed 
*
*  NOTES:
*
*******************************************************************************/
PUBLIC BOOL pmc_atomic_yield_is_allowed(pmc_sys_handle_t *sys_handle)
{    
    BOOL result = TRUE;
    PMC_ENTRY();       
    PMC_RETURN( result );   
} /* pmc_atomic_yield_is_allowed */

/*********************************************************************
* pmc_atoi
* ____________________________________________________________________
*
* DESCRIPTION:
*   Parses a string interpreting its content as a decimal 
*   number, which is returned as an int value.
*
* INPUTS:
*   *str  -   String to convert in decimal number
*
* OUTPUTS: 
*   None
*
* RETURNS:
*    UINT32 - converted decimal number.
*
* NOTES:
* 
**********************************************************************/
PUBLIC UINT32 pmc_atoi(char *str)
{
    PMC_ENTRY();
        
    PMC_RETURN(atoi(str));
} /* pmc_atoi */


/*********************************************************************
* pmc_is_logger_initialized
* ____________________________________________________________________
*
* DESCRIPTION:
*  Returns a boolean indicating whether or not the logger has already been
*  initialized. If the boolean is FALSE, the first time called, then 
*  initializes a pthread mutex used to synchronize access to the logger 
*  string registration mechanism used by the various sub systems and blocks.
*
* INPUTS:
*  None
*
* OUTPUTS: 
*  None
*
* RETURNS:
*  TRUE - Logger has already been initialized and pmc_log_init should not
*         be executed again
*  FALSE - This is the first time this has been called. Mutex is initialized
*          and execution of pmc_log_init should be completed.
*
* NOTES:
* 
**********************************************************************/
PUBLIC BOOL8 pmc_is_logger_initialized(void)
{
    /*
     * If not yet initialized, indicating never called, then set the flag
     * to TRUE and initialize the global log mutex.  In this case FALSE is
     * returned to allow whatever logging specific initialization to occur.
     * Otherwise return TRUE to indicate this operation has altrady occurred
     */
    if (FALSE == pmc_global_log_mutex_init)
    {
        pmc_global_log_mutex_init = TRUE;
        PMC_ASSERT(0 == pthread_mutex_init(&pmc_global_log_mutex, NULL), 
                         PMC_ERR_FAIL,0,0);
        return FALSE;
    }

    return TRUE;
     
} /* pmc_is_logger_initialized */


/*********************************************************************
* pmc_logger_mutex_set
* ____________________________________________________________________
*
* DESCRIPTION:
*  Locks or unlocks the mutex depending on the value of the lock flag passed
*  to the function. Returns if pmc_global_log_mutex_init is FALSE indicating
*  pmc_log_init and pmc_is_logger_initialized was never called.
*
* INPUTS:
*  lock - Whether mutex is to be locked or unlocked.
*
* OUTPUTS: 
*   None
*
* RETURNS:
*  None
*
* NOTES:
* 
**********************************************************************/
PUBLIC void pmc_logger_mutex_set(BOOL8 lock)
{
    PMC_ENTRY();     

    /*
     * If FALSE then pmc_log_init has not been called. As such to prevent
     * an assert simply return.  This function is called as a result of
     * pmc_log_blocks_string_register which is called in various blocks and
     * subsystems. However pmc_log_init is called from the user application
     * If not called then synchronous access to the logger is not provided.
     * NOTE if is highly recommended that pmc_log_init be called by an 
     * application SW at startup regardless of whether or not logging is to
     * be used.
     */
    if (FALSE == pmc_global_log_mutex_init)
    {
        PMC_RETURN();
    }

    if (TRUE == lock)
    {
        PMC_ASSERT(0 == pthread_mutex_lock(&pmc_global_log_mutex),
                   PMC_ERR_FAIL,0,0);  
    }
    else
    {
        PMC_ASSERT(0 == pthread_mutex_unlock(&pmc_global_log_mutex),
                   PMC_ERR_FAIL,0,0);  
    }

    PMC_RETURN();
} /* pmc_is_logger_initialized */

