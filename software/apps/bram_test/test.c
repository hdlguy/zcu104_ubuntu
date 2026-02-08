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


int main(int argc,char** argv)
{
    void* pcie_addr;

    uint32_t pcie_bar0_addr=BASE_ADDRESS;
    uint32_t pcie_bar0_size=PROTO_SIZE;

    pcie_addr=phy_addr_2_vir_addr(pcie_bar0_addr,pcie_bar0_size);
    if(pcie_addr==NULL)
    {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr. you must be in root.\n",pcie_bar0_addr,pcie_bar0_size);
       exit(-1);
    }
    fprintf(stdout,"phy_addr 0x%08x with size 0x%08x to viraddr 0x%p.\n",pcie_bar0_addr,pcie_bar0_size, pcie_addr);

    fprintf(stdout,"FPGA ID: 0x%08X\n",read_reg(pcie_addr,FPGA_ID));

    fprintf(stdout,"VERSION: 0x%08X\n",read_reg(pcie_addr,FPGA_VERSION));

    write_reg(pcie_addr, LED_CONTROL, 0x03);

    uint32_t write_data[TEST_BRAM_SIZE/4], read_data[TEST_BRAM_SIZE/4];

    // create test data.
    for (int i=0; i<TEST_BRAM_SIZE/4; i++) write_data[i] = rand();

    uint32_t* bram_ptr = pcie_addr + TEST_BRAM_OFFSET;
    fprintf(stdout, "bram_ptr = %p\n", bram_ptr);

    // write bram
    for (int i=0; i<TEST_BRAM_SIZE/4; i++) bram_ptr[i] = write_data[i];

    // read bram
    for (int i=0; i<TEST_BRAM_SIZE/4; i++) read_data[i] = bram_ptr[i];

    // chech bram results
    int errors = 0;
    for (int i=0; i<TEST_BRAM_SIZE/4; i++) {
        if (read_data[i] != write_data[i]) errors++;
    }
    fprintf(stdout, "errors = %d\n", errors);

    munmap(pcie_addr,pcie_bar0_size);

    return 0;
}
