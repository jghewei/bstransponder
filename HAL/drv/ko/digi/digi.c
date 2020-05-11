#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <asm/uaccess.h>

// please define below two lines in your application
#define DIGI_IOCTL_MAGIC 0xbb
#define DIGI_IOCTL_READ_DMA_PHY_ADDR         _IOR(DIGI_IOCTL_MAGIC, 1, unsigned long)

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("digi kernel space device driver module");

#define DEBUG_ON

#define DIGI_DRIVER_NAME    "DIGI-PCIE"
#define DIGI_VENDOR_ID      0x11f8
#define DIGI_DEVICE_ID      0x5440
#define DIGI_DEV_NUM        1
#define DIGI_PCI_BAR_MAX    3

#define DIGI_BASE_PHYS      0x80000000
#define DIGI_REGS_SIZE      0x02000000  /* Range for BAR1, includes some gaps, but all regs */
#define DMA_RAM_SIZE        0x400000    // Digi need 4M DMA

#define DIGI_MAJOR  0  /* Force dynamic alloc */
#define DIGI_MINOR  0 

static int digi_major = 0;

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int digi_open(struct inode *inod, struct file *filpe);
static int digi_release(struct inode *inod, struct file *filpe);
static int digi_init(void);
static int digi_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
static void digi_exit(void);
static int digi_mmap(struct file *fp, struct vm_area_struct *vma);

static void *dma_alloc_consistent(size_t size, dma_addr_t *handle)
{
    void *ret;

    ret = (void *)__get_free_pages(GFP_DMA, get_order(size));

    if (ret != NULL)
    {
        memset(ret, 0, size);
        *handle = virt_to_phys(ret);
    }
    return ret;
}

static void dma_free_consistent(size_t size, void *vaddr, dma_addr_t handle)
{
    free_pages((unsigned long)vaddr, get_order(size));
}

struct digi_dev 
{
    struct cdev cdev;

    unsigned long   digi_phy_addr;
    void*           dma_cpu_addr;
    unsigned long   dma_phy_addr;
    size_t          dma_buf_size;
};
struct digi_dev *digiDeviceList[DIGI_DEV_NUM] = {0};

static struct file_operations digi_fops =
{
    .owner   = THIS_MODULE,
    .ioctl   = digi_ioctl, 
    .mmap    = digi_mmap,
    .open    = digi_open,
    .release = digi_release,
};

static int digi_open(struct inode *inode, struct file *filp)
{
    struct digi_dev *dev;
  
    dev = container_of(inode->i_cdev, struct digi_dev, cdev);
    if (dev != NULL)
        filp->private_data = dev; // for other methods
    else
        filp->private_data = NULL;

    return 0;
}

static int digi_release(struct inode *inode, struct file *filp)
{
    struct digi_dev *dev = filp->private_data;

    if (dev == NULL)
    {
        filp->private_data = NULL;
    }
 
    return 0;
}

static int digi_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;

    struct digi_dev *dev = filp->private_data;
    if (dev != NULL)
    {
        switch (cmd)
        {
            case DIGI_IOCTL_READ_DMA_PHY_ADDR:
                if (copy_to_user((void __user *)arg, (void *)&dev->dma_phy_addr, sizeof(unsigned long)) != 0)
                {
                    printk(KERN_ERR "ERROR: digi_ioctl: copy_to_user() failed!!\n");
                    ret = -EFAULT;
                }
                break;
            default:
                break;
        }
    }
    
    return ret;
}

static int digi_mmap(struct file *fp, struct vm_area_struct *vma)
{
    int ret;
    unsigned long membase;
    unsigned long memsize;

    struct digi_dev *dev = fp->private_data;
    if (dev == NULL)
    {
        printk(KERN_ERR "digi_mmap dev NULL!\n");
        return -1;
    }

#ifdef DEBUG_ON
    printk(KERN_INFO "digi_mmap: fp = 0x%8.8x vma = 0x%8.8x\n", (unsigned int)fp, (unsigned int)vma);
#endif

    if (vma->vm_pgoff == 0) //register map
    {
        /* register map */
        membase = (dev->digi_phy_addr) >> PAGE_SHIFT;
        memsize = vma->vm_end - vma->vm_start;
        vma->vm_page_prot |= (_PAGE_GUARDED | _PAGE_NO_CACHE);
    }
    else if (vma->vm_pgoff == 1) //dma memory map
    {
        membase = (dev->dma_phy_addr >> PAGE_SHIFT);
        memsize = vma->vm_end - vma->vm_start;
        vma->vm_page_prot |= (_PAGE_GUARDED | _PAGE_NO_CACHE);
    }
    else
    {
        printk(KERN_ERR "digi_mmap: unknown mmap offset %d, rejecting\n", (int)vma->vm_pgoff);
        return -EINVAL;
    }

#ifdef DEBUG_ON
    printk(KERN_INFO "digi_mmap: phy 0x%lx mapped to 0x%08x size %lu\n", membase, (unsigned int)vma->vm_start, memsize);
#endif

    /* Now map it to physical memory. Note that we saved addresses as PFN part only. */
    ret = io_remap_pfn_range(vma, vma->vm_start, membase, memsize, vma->vm_page_prot);
    if (ret != 0)
    {
        printk(KERN_ERR "digi_mmap: io_remap_pfn_range() failed with error code %d\n", ret);
    }

    return ret;
}
        
/*
 *  Init routine for this module 
 */
static int digi_init(void) 
{
    int result, inst = 0, devno;
    dev_t dev = 0;
    
    if (digi_major)
    {
        dev = MKDEV(digi_major, DIGI_MINOR);
        result = register_chrdev_region(dev, DIGI_DEV_NUM, "digi");
    }
    else
    {
        result = alloc_chrdev_region(&dev, DIGI_MINOR, DIGI_DEV_NUM, "digi");
        digi_major = MAJOR(dev);
    }

    if (result < 0)
    {
        printk(KERN_WARNING "digi_init(): can't get major %d\n", digi_major);
        return result;
    }
#ifdef DEBUG_ON
    else
    {
        printk(KERN_DEBUG "digi_init(): assigned major number %d\n", digi_major);
    }
#endif

    for (inst = 0; inst < DIGI_DEV_NUM; inst++)
    {
        /* Set up the char device that is used for mmap() */
        struct digi_dev *digiDevice = kmalloc(sizeof(struct digi_dev), GFP_KERNEL);
        if (!digiDevice)
        {
            result = -ENOMEM;
            //T.B.D digi_exit();
            return result;
        }

        memset(digiDevice, 0, sizeof(struct digi_dev));

        devno = MKDEV(digi_major, DIGI_MINOR + inst);
        cdev_init(&(digiDevice->cdev), &digi_fops);
        digiDevice->cdev.owner = THIS_MODULE;
        digiDevice->cdev.ops = &digi_fops;
        switch (inst)
        {
        case 0:
            digiDevice->digi_phy_addr = DIGI_BASE_PHYS;
            break;
        }

        digiDevice->dma_cpu_addr = dma_alloc_consistent(DMA_RAM_SIZE, (dma_addr_t *)&digiDevice->dma_phy_addr);
        digiDevice->dma_buf_size = DMA_RAM_SIZE;

        result = cdev_add(&(digiDevice->cdev), devno, 1);
        if (result)
        {
            printk(KERN_WARNING "digi_driver_probe(): Error %d adding digi char device", result);
            //T.B.D do some more
        }

        digiDeviceList[inst] = digiDevice;
        
#ifdef DEBUG_ON
        printk(KERN_WARNING "-----------------------------------------------------------------\n");
        printk(KERN_WARNING "digi_driver_probe(%d): BAR 0 Physical address = 0x%lx\n",inst,digiDevice->digi_phy_addr);
        printk(KERN_WARNING "                     : DMA  Physical address  = 0x%lx\n",digiDevice->dma_phy_addr);
        printk(KERN_WARNING "-----------------------------------------------------------------\n");
#endif
    }

    return 0;
}

/* 
 * Exit routine for this module 
 */
static void digi_exit(void) 
{
    int inst;

    for (inst = 0; inst < DIGI_DEV_NUM; inst++)
    {
        if (digiDeviceList[inst])
        {
            cdev_del(&(digiDeviceList[inst]->cdev));

            if (digiDeviceList[inst]->dma_cpu_addr)
            {
                dma_free_consistent(DMA_RAM_SIZE, digiDeviceList[inst]->dma_cpu_addr, (dma_addr_t)digiDeviceList[inst]->dma_phy_addr);
            }

            kfree(digiDeviceList[inst]);
            digiDeviceList[inst] = NULL;
        }
    }

    if (digi_major != 0)
    {
        unregister_chrdev_region(MKDEV(digi_major, DIGI_MINOR), DIGI_DEV_NUM);
    }
}

module_init(digi_init);
module_exit(digi_exit);

