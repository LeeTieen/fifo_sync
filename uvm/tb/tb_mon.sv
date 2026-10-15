class monitor extends uvm_monitor;

  `uvm_component_utils(monitor)
  virtual fifo_inf #(DATA_WIDTH) vif;
  uvm_analysis_port #(transaction) ap;

  function new (string name, uvm_component parent);
    super.new(name, parent);
    ap = new("ap", this);
  endfunction

  function void build_phase(
    uvm_phase phase
  );
    super.build_phase(phase);
        if (
            !uvm_config_db #(
                virtual fifo_inf #(DATA_WIDTH)
            )::get(
                this,
                "",
                "vif",
                vif
            )
        ) begin
            `uvm_fatal(
                "[MON]",
                "Cannot get virtual interface"
            )
        end
  endfunction

  task run_phase(
        uvm_phase phase
    );
        transaction t;
        forever begin
            @(posedge vif.clk);
            t = transaction::type_id::create("t");

            // Input at rising edge
            t.rst_n = vif.rst_n;
            t.wr_en = vif.wr_en;
            t.rd_en = vif.rd_en;
            t.din   = vif.din;

            #1ps;
            t.dout  = vif.dout;
            t.full  = vif.full;
            t.empty = vif.empty;
            ap.write(t); 
        end
    endtask

endclass
