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

    task reset();
        run(0, 0, 0, 'b0);
    endtask


endclass
