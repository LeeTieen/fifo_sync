`timescale 1ns/1ps
module tb_top;
    import tb_pkg::*;
    import tc_pkg::*;
    
    environment env;
    tc_10 test10;
    

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

    //clock 200MHz
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

        //Use testcase
        test10 = new(env);
        test10.test();

        $display("TC_10 completed");
        #1ns;
        $display("TOTAL COVERAGE: %0.2f%%", env.cov.fifo_cg.get_coverage());
        $display("COVERAGE OF operation: %0.2f%%", env.cov.fifo_cg.cp_operation.get_coverage());
        $display("COVERAGE OF state: %0.2f%%", env.cov.fifo_cg.cp_state.get_coverage());
        $display("COVERAGE OF data: %0.2f%%", env.cov.fifo_cg.cp_data.get_coverage());
        $display("COVERAGE OF cross op x state: %0.2f%%", env.cov.fifo_cg.operation_state.get_coverage());
        $finish;
    end

endmodule
