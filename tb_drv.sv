class driver;
  transaction t;
  mailbox mbx;
  event done;

  virtual fifo_inf #(DATA_WIDTH) vif;

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
      @(negedge vif.clk)
        mbx.get(t);
        vif.rst_n = t.rst_n;
        vif.wr_en = t.wr_en;
        vif.rd_en = t.rd_en;
        vif.din = t.din;

      @(posedge vif.clk)
       -> done;
    end
  endtask
endclass
