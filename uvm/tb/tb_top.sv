`timescale 1ns/1ps
module tb_top;
    import uvm_pkg::*;
    import tb_pkg::*;
    
    environment env;
    
    // tc_00_idle          test00;
    // tc_01_reset         test01;
    // tc_02_write         test02;
    // tc_03_full          test03;
    // tc_04_write_full    test04;
    // tc_05_read          test05;
    // tc_06_read_empty    test06;
    // tc_07_read_write    test07;
    // tc_08               test08;
    // tc_09               test09;
    // tc_10               test10;
    

    fifo_inf #(.DATA_WIDTH(DATA_WIDTH)) vif();
    fifo_sync #(.DATA_WIDTH(DATA_WIDTH),
                .DEPTH(DEPTH)) 
        dut(
            .clk(vif.clk),
            .rst_n(vif.rst_n),
            .wr_en(vif.wr_en),
            .rd_en(vif.rd_en),
            .din(vif.din),
            .dout(vif.dout),
            .full(vif.full),
            .empty(vif.empty)
    );

    initial vif.clk = 0;

    //clock 200MHz
    always #5ns vif.clk = ~vif.clk;

    initial begin
        vif.rst_n = 0;
        vif.wr_en = 0;
        vif.rd_en = 0;
        vif.din = 0;

        uvm_config_db #(
            virtual fifo_inf #(DATA_WIDTH)
        )::set(
            null,
            "*",
            "vif",
            vif
        );

        run_test("tb_test");

    end

endmodule
