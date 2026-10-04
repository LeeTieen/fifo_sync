class generator;
    transaction t;
    mailbox mbx;
    event done;

    function new(mailbox mbx);
        this.mbx = mbx;
    endfunction

    task run(
            bit                   rst_n,
            bit                   wr_en,
            bit                   rd_en,
            bit [DATA_WIDTH-1:0]  din
    );
            t = new();
            t.rst_n = rst_n;
            t.wr_en = wr_en;
            t.rd_en = rd_en;
            t.din = din;
            mbx.put(t);

            @(done);
    endtask

    task tc_01_reset();
        $display("FIFO reset");
        run(0, 0, 0, 'b0);
        run(0, 1, 0, 8'hA5);
        run(0, 0, 0, 'b0);
        run(1, 0, 1, 'b0);
    endtask

    task tc_02_write();
        $display("FIFO write");
        run(0, 0, 0, 'b0);
        run(1, 1, 0, 8'hFA);
        run(1, 0, 1, 'b0);
    endtask

    task tc_03_full();
        $display("FIFO full");
        for(int i = 0; i < DEPTH; i++) begin
            run(1, 1, 0, i);
        end
        run(1, 1, 0, 8'hFF);
    endtask
endclass
