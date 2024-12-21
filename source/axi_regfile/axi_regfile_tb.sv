// 
module axi_regfile_tb ();

    logic [39:0]    M00_AXI_araddr;
    logic [2:0]     M00_AXI_arprot;
    logic           M00_AXI_arready;
    logic           M00_AXI_arvalid;
    logic [39:0]    M00_AXI_awaddr;
    logic [2:0]     M00_AXI_awprot;
    logic           M00_AXI_awready;
    logic           M00_AXI_awvalid;
    logic           M00_AXI_bready;
    logic [1:0]     M00_AXI_bresp;
    logic           M00_AXI_bvalid;
    logic [31:0]    M00_AXI_rdata;
    logic           M00_AXI_rready;
    logic [1:0]     M00_AXI_rresp;
    logic           M00_AXI_rvalid;
    logic [31:0]    M00_AXI_wdata;
    logic           M00_AXI_wready;
    logic [3:0]     M00_AXI_wstrb;
    logic           M00_AXI_wvalid;
    logic           axi_aresetn;

    localparam integer LOG2_NREGS = 5;
    localparam integer NREGS = 2**LOG2_NREGS;


    localparam clk_period = 10; logic axi_aclk = 0; always #(clk_period/2) axi_aclk = ~axi_aclk;
    

    logic [NREGS-1:0][31:0] slv_reg, slv_read;
    assign slv_read = slv_reg;

	axi_regfile_v1_0_S00_AXI #	(
		.C_S_AXI_DATA_WIDTH (32),
		.C_S_AXI_ADDR_WIDTH (LOG2_NREGS+2)
	) axi_regfile_inst (
        // register interface
        .slv_read       (slv_read), 
        .slv_reg        (slv_reg),  
        // axi interface
		.S_AXI_ACLK    (axi_aclk),
		.S_AXI_ARESETN (axi_aresetn),
        //
		.S_AXI_ARADDR  (M00_AXI_araddr ),
		.S_AXI_ARPROT  (M00_AXI_arprot ),
		.S_AXI_ARREADY (M00_AXI_arready),
		.S_AXI_ARVALID (M00_AXI_arvalid),
		.S_AXI_AWADDR  (M00_AXI_awaddr ),
		.S_AXI_AWPROT  (M00_AXI_awprot ),
		.S_AXI_AWREADY (M00_AXI_awready),
		.S_AXI_AWVALID (M00_AXI_awvalid),
		.S_AXI_BREADY  (M00_AXI_bready ),
		.S_AXI_BRESP   (M00_AXI_bresp  ),
		.S_AXI_BVALID  (M00_AXI_bvalid ),
		.S_AXI_RDATA   (M00_AXI_rdata  ),
		.S_AXI_RREADY  (M00_AXI_rready ),
		.S_AXI_RRESP   (M00_AXI_rresp  ),
		.S_AXI_RVALID  (M00_AXI_rvalid ),
		.S_AXI_WDATA   (M00_AXI_wdata  ),
		.S_AXI_WREADY  (M00_AXI_wready ),
		.S_AXI_WSTRB   (M00_AXI_wstrb  ),
		.S_AXI_WVALID  (M00_AXI_wvalid )
	);

    logic axi_init, error, axi_done;
    initial begin
        axi_init = 0;
        axi_aresetn = 0;
        #(clk_period*10);
        axi_aresetn = 1;
        #(clk_period*10);
        axi_init = 1;
        #(clk_period*1);
        axi_init = 0;
    end

    localparam integer C_M_TRANSACTIONS_NUM = 2**LOG2_NREGS;
	axi_master_v1_0_M00_AXI # (
		.C_M_START_DATA_VALUE       (32'hAA000000),
		.C_M_TARGET_SLAVE_BASE_ADDR (32'h40000000),
		.C_M_AXI_ADDR_WIDTH         (32),
		.C_M_AXI_DATA_WIDTH         (32),
		.C_M_TRANSACTIONS_NUM       (NREGS)
	) axi_master_inst (
		.INIT_AXI_TXN  (axi_init),
		.ERROR         (error),
		.TXN_DONE      (axi_done),
		.M_AXI_ACLK    (axi_aclk),
		.M_AXI_ARESETN (axi_aresetn),
        //
		.M_AXI_ARADDR  (M00_AXI_araddr ),
		.M_AXI_ARPROT  (M00_AXI_arprot ),
		.M_AXI_ARREADY (M00_AXI_arready),
		.M_AXI_ARVALID (M00_AXI_arvalid),
		.M_AXI_AWADDR  (M00_AXI_awaddr ),
		.M_AXI_AWPROT  (M00_AXI_awprot ),
		.M_AXI_AWREADY (M00_AXI_awready),
		.M_AXI_AWVALID (M00_AXI_awvalid),
		.M_AXI_BREADY  (M00_AXI_bready ),
		.M_AXI_BRESP   (M00_AXI_bresp  ),
		.M_AXI_BVALID  (M00_AXI_bvalid ),
		.M_AXI_RDATA   (M00_AXI_rdata  ),
		.M_AXI_RREADY  (M00_AXI_rready ),
		.M_AXI_RRESP   (M00_AXI_rresp  ),
		.M_AXI_RVALID  (M00_AXI_rvalid ),
		.M_AXI_WDATA   (M00_AXI_wdata  ),
		.M_AXI_WREADY  (M00_AXI_wready ),
		.M_AXI_WSTRB   (M00_AXI_wstrb  ),
		.M_AXI_WVALID  (M00_AXI_wvalid )
	);

endmodule

