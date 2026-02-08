#define		BASE_ADDRESS		0xa0000000
#define		PROTO_SIZE		    0x02000000

#define 	REGFILE_OFFSET		0x00000000

#define 	FPGA_ID     		REGFILE_OFFSET+0x04*0  // Currently returns 0xDEADBEEF
#define 	FPGA_VERSION		REGFILE_OFFSET+0x04*1  // Returns major and minor version numbers.
#define 	LED_CONTROL		    REGFILE_OFFSET+0x04*2  // Returns major and minor version numbers.

#define     TEST_BRAM_OFFSET    0x00010000
#define     TEST_BRAM_SIZE      0x00001000 // 4KB

#define     ULTRA_RAM_OFFSET    0x01000000 // Ultraram
#define     ULTRA_RAM_SIZE      0x00200000 // 2MB

