#ifndef _MEM_IO_H_
#define _MEM_IO_H_
void write_reg(uint8_t* vir_addr,uint32_t offset,uint32_t value);
uint32_t read_reg(void* vir_addr,uint32_t offset);
void* phy_addr_2_vir_addr(off_t phy_addr,size_t size);
void regs_check(const uint8_t* vir_addr, const uint8_t* phy_addr,uint32_t* regs_off,uint32_t regs_num);
#endif// _MEM_IO_H_
