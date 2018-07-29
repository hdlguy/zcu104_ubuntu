#define		BASE_ADDRESS		0x80000000
#define		PROTO_SIZE		0x80000000

#define 	AXI_REGFILE_0		0x00000000

#define 	FPGA_ID     		AXI_REGFILE_0+0x04*0  // Currently returns 0xDEADBEEF
#define 	FPGA_VERSION		AXI_REGFILE_0+0x04*1  // Returns major and minor version numbers.
#define 	LED_CONTROL		AXI_REGFILE_0+0x04*2  // Returns major and minor version numbers.

#define         TEST_BRAM_OFFSET    0x20000000
#define         TEST_BRAM_SIZE      0x00001000 // 4KB

