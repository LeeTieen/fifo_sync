class transaction;
    bit                        clk;
    bit                        rst_n;
    rand bit                   wr_en;
    rand bit                   rd_en;
    rand bit [DATA_WIDTH-1:0]  din;

    logic [DATA_WIDTH-1:0]   dout;
    logic                    full;
    logic                    empty;    
endclass