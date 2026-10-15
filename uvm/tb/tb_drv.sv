class driver extends uvm_driver #(transaction);

  `uvm_component_utils(driver)

    virtual fifo_inf #(DATA_WIDTH) vif;

  // Constructor
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

  function void build_phase(
    uvm_phase phase
  );

    super.build_phase(phase);
    if(!uvm_config_db#(
      virtual fifo_inf #(DATA_WIDTH)
      )::get
          (this,
            "",
            "vif", vif
          )
    ) begin
            `uvm_fatal("[DRV]",
                        "Cannot get virtual interface"
            );
      end

  endfunction
    
  task run_phase(uvm_phase phase);
    transaction t;
    forever begin
      seq_item_port.get_next_item(t);

      @(negedge vif.clk)

        vif.rst_n = t.rst_n;
        vif.wr_en = t.wr_en;
        vif.rd_en = t.rd_en;
        vif.din = t.din;

      @(posedge vif.clk)
        seq_item_port.item_done();
    end

  endtask
endclass