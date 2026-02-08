#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#include "mem-io.h"
#include "utils.h"
#include "proto2_hw.h"

uint32_t check_fgpa_segment(uint8_t* pcie_addr,uint32_t segment_offset,uint32_t segment_size)
{
   #define FPGA_REG_BITS sizeof(uint32_t)*8
   uint32_t i;
   uint32_t err_cnt=0;
   printf("checking FPGA segment 0x%08X...\n",segment_offset);
   for(i=0;i<segment_size;i+=sizeof(uint32_t))
   {
     uint32_t j;
     for(j=0;j<FPGA_REG_BITS;j++)
     {
        write_reg(pcie_addr,segment_offset+i,1<<j);
        if(read_reg(pcie_addr,segment_offset+i)!=1<<j)
        {
           //printf("ERR ACCESS FPGA_VIR_ADDR[0x%lX+0x%X+0x%X]=0x%08X\n",(uint64_t)pcie_addr,segment_offset,i,read_reg(pcie_addr,segment_offset+i));
           printf("ERR ACCESS FPGA_VIR_ADDR[0x%X+0x%X+0x%X]=0x%08X\n",(uint32_t)pcie_addr,segment_offset,i,read_reg(pcie_addr,segment_offset+i));
           err_cnt++;
        }
     }

     for(j=0;j<FPGA_REG_BITS;j++)
     {
        write_reg(pcie_addr,segment_offset+i,~(1<<j));
        if(read_reg(pcie_addr,segment_offset+i)!=~(1<<j))
        {
           //printf("ERR ACCESS FPGA VIR ADDR[0x%lX+0x%X+0x%X]=0x%08X\n",(uint64_t)pcie_addr,segment_offset,i,read_reg(pcie_addr,segment_offset+i));
           printf("ERR ACCESS FPGA VIR ADDR[0x%X+0x%X+0x%X]=0x%08X\n",(uint32_t)pcie_addr,segment_offset,i,read_reg(pcie_addr,segment_offset+i));
           err_cnt++;
        }
     }
   }
   return err_cnt;
}


int main(int argc,char** argv)
{
    void* pcie_addr;

    uint32_t pcie_bar0_addr;
    uint32_t pcie_bar0_size=PROTO_SIZE;
    if(sscanf(argv[1],"%x",&pcie_bar0_addr)!=1)
    {
      fprintf(stderr,"invalid PCIE BAR0 ADDR %s\n",argv[1]);
      exit(-1);
    }

    pcie_addr=phy_addr_2_vir_addr(pcie_bar0_addr,pcie_bar0_size);
    if(pcie_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr. you must be in root.\n",pcie_bar0_addr,pcie_bar0_size);
       exit(-1);
    }

    fprintf(stdout,"FPGA ID: 0x%08X\n",read_reg(pcie_addr,FPGA_ID));
    fprintf(stdout,"VERSION: 0x%08X\n",read_reg(pcie_addr,FPGA_VERSION));

    printf("%u err for checking FPGA segment 0x%08X\n",check_fgpa_segment(pcie_addr,TEST_BRAM_OFFSET,TEST_BRAM_SIZE),TEST_BRAM_OFFSET);
/*
*/

    munmap(pcie_addr,pcie_bar0_size);

    return 0;
}
