class monitor;
    transaction t;
    mailbox mbx;
    virtual fifo_inf#(DATA_WIDTH) vif;
    event done;

    function new(mailbox mbx);
        this.mbx = mbx;
    endfunction

    task run(
            bit rst_n,
            bit rd_en,
            bit wr_en,
            bit [DATA_WIDTH-1:0] din,

            bit [DATA_WIDTH-1:0] dout,
            bit full,
            bit temp
        );
        forever begin
            t = new();
            @(posedge vif.clk)
            t.rst_n = vif.rst_n;
            t.rd_en = vif.rd_en;
            t.wr_en = vif.wr_en;
            t.din = vif.din;
            
            t.dout = vif.dout;
            t.full = vif.full;
            t.temp = vif.temp;
            mbx.put(t);

        end
    endtask
endclass