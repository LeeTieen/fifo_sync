`timescale 1ns/1ps
module tb_top;
    import tb_pkg::*;
    
    environment env;

    fifo_inf #(.DATA_WIDTH(DATA_WIDTH)) vif();
    fifo_sync #(.DATA_WIDTH(DATA_WIDTH)) dut(
        .clk(vif.clk),
        .rst_n(vif.rst_n),
        .wr_en(vif.wr_en),
        .rd_en(vif.rd_en),
        .din(vif.din),
        .dout(vif.dout),
        .full(vif.full),
        .empty(vif.empty)
    );

    always #5ns vif.clk = ~vif.clk;
    initial begin
        vif.clk = 0;
        vif.rst_n = 0;
        vif.wr_en = 0;
        vif.rd_en = 0;
        vif.din = '0;

        env = new();
        env.vif = vif;
        env.run();

        env.gen.tc_01_reset();
        env.gen.tc_02_write();
        env.gen.tc_03_full();

        env.gen.run(1, 0, 0, '0);
    end

    initial begin
        #1000;
        $finish;
    end

endmodule
