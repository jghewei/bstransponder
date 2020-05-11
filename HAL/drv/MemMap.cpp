#include <fcntl.h>  
#include <sys/mman.h>  
#include "MemMap.h"

static int mem_fd = -1;  

MemAddr MemMap(uint32_t addr, uint32_t size)
{
    if (mem_fd < 0)
    {
        mem_fd = open("/dev/mem", O_RDWR | O_NDELAY);
        if (mem_fd < 0)
        {
            APP_LOG("mem fd open failed");
            exit(-1);
        }
    }

    if ((addr & 0xfffff000) != addr)
    {
        APP_LOG("mem map failed for not align to 4K size, addr = 0x%08x, size = 0x%08x", addr, size);
        exit(-1);
    }

    return (MemAddr)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, addr);
}

void MemWrite(MemAddr addr, uint32_t data)
{
#ifdef __TARGET__
    *((MemAddr32)addr) = data;
#else
    APP_LOG("%s %p 0x%08x", __FUNCTION__, addr, data);
#endif
}

void MemWriteMask(MemAddr addr, uint32_t data, uint32_t mask)
{
#ifdef __TARGET__
    uint32_t val = *((MemAddr32)addr);
    val &= ~mask;
    val |= data;
    *((MemAddr32)addr) = val;
#else
    APP_LOG("%s %p 0x%08x 0x%08x", __FUNCTION__, addr, data, mask);
#endif
}

uint32_t MemRead(MemAddr addr)
{
#ifdef __TARGET__
    //APP_LOG("%s %p", __FUNCTION__, addr);
    return *((MemAddr32)addr);
#else
    APP_LOG("%s %p", __FUNCTION__, addr);
    return 0;
#endif
}

