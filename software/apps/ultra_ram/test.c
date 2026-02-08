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
    if(pcie_addr==NULL) {
       fprintf(stderr,"can't mmap phy_addr 0x%08x with size 0x%08x to viraddr. you must be in root.\n",pcie_bar0_addr,pcie_bar0_size);
       exit(-1);
    }
    fprintf(stdout,"phy_addr 0x%08x with size 0x%08x to viraddr 0x%p.\n",pcie_bar0_addr,pcie_bar0_size, pcie_addr);

    fprintf(stdout,"FPGA ID: 0x%08X\n",read_reg(pcie_addr,FPGA_ID));
    fprintf(stdout,"VERSION: 0x%08X\n",read_reg(pcie_addr,FPGA_VERSION));

    write_reg(pcie_addr, LED_CONTROL, 0x02);

    const int Nram = ULTRA_RAM_SIZE/4;
    uint32_t write_data[Nram];
    uint32_t read_data[Nram];

    uint32_t* uram = pcie_addr + ULTRA_RAM_OFFSET;

    // Let's test the URAM
    int errors;
    errors = 0;
    const int Ntest = 10;
    for (int j=0;j<Ntest;j++){

	    // compute test data
    	for (int i=0; i<Nram; i++) write_data[i] = rand();
	
	    // copy to uram
	    memcpy( uram, write_data, sizeof(write_data) );
	
	    // read and check uram data
	    memcpy( read_data, uram, sizeof(write_data) );
    	for (int i=0; i<Nram; i++) if (read_data[i] != write_data[i]) errors++;

    }
    fprintf(stdout, "Ntest = %d, errors = %d\n", Ntest, errors);

    munmap(pcie_addr,pcie_bar0_size);

    return 0;
}


