class gen;
    transaction t;
    mailbox mbx;

    function new(mailbox mbx);
        this.mbx = mbx;
    endfunction

    task run(
            bit                   rst_n,
            bit                   wr_en,
            bit                   rd_en,
            bit [DATA_WIDTH-1:0]  din
    );
        forever begin
            t = new();
            t.rst_n = rst_n;
            t.wr_en = wr_en;
            t.rd_en = rd_en;
            t.din = din;
            mbx.put(t);
        end
    endtask
endclass