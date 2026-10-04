class monitor;
    transaction t;
    mailbox mbx;
    virtual fifo_inf#(DATA_WIDTH) vif;
    event done;

    function new(mailbox mbx);
        this.mbx = mbx;
    endfunction

    task run();
        forever begin
            t = new();
            @(posedge vif.clk)
            t.rst_n = vif.rst_n;
            t.rd_en = vif.rd_en;
            t.wr_en = vif.wr_en;
            t.din = vif.din;
            #1ns;
            t.dout = vif.dout;
            t.full = vif.full;
            t.empty = vif.empty;
            mbx.put(t);

        end
    endtask
endclass
