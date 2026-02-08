#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "utils.h"

void write_reg(uint8_t* vir_addr,uint32_t offset,uint32_t value)
{
//   DEBUG("before write: *%p=0x%08x\n",vir_addr+offset,*((uint32_t*)(vir_addr+offset)));
//usleep(100);

//DEBUG("write         *%p 0x%08x\n",vir_addr+offset,value);

   *((uint32_t*)(vir_addr+offset))=value;
//usleep(100);
//   DEBUG("post write:   *%p=0x%08x\n\n",vir_addr+offset,*((uint32_t*)(vir_addr+offset)));
}

uint32_t read_reg(uint8_t* vir_addr, uint32_t offset)
{
//DEBUG("read vir_addr=%x, offset=%x, %x\n",(uint32_t)vir_addr, offset, (uint32_t)(vir_addr+offset));
   return *((uint32_t*)(vir_addr+offset));
}




void* phy_addr_2_vir_addr(off_t phy_addr,size_t size)
{
   void* vir_addr=NULL;

   int fd = open("/dev/mem",O_RDWR|O_SYNC);
   if(fd < 0)
   {
       fprintf(stderr,"Can't open /dev/mem\n");
   }
   else
   {
                  //0 is not NULL
      vir_addr=mmap(0,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,phy_addr);
      if(vir_addr == NULL)
      {
          fprintf(stderr,"Can't mmap\n");
      }
      else
      {
          //DEBUG("phy_addr 0x%lX mapped to 0x%lX with size=0x%x bytes\n",phy_addr,(uint64_t)vir_addr,(uint32_t)size);
          DEBUG("phy_addr 0x%lx mapped to 0x%p with size=0x%zx bytes\n", phy_addr, vir_addr, size);
      }
   }
   return vir_addr;
}

void regs_check(const uint8_t* vir_addr, const uint8_t* phy_addr,uint32_t* regs_off,uint32_t regs_num)
{
   int i;
   for(i=0;i<regs_num;i++)
      printf("reg_val=0x%08x,"
             "(phy_addr=%p,vir_addr=%p)\n",
             *((uint32_t*)(vir_addr+regs_off[i])),
             phy_addr+regs_off[i],vir_addr+regs_off[i]);

}
