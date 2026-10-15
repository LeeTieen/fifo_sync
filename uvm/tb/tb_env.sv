class environment extends uvm_env;
    `uvm_component_utils(environment)

    driver drv;
    monitor mon;
    scoreboard sco;
    sequencer seqr;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);

        drv  = driver::type_id::create("drv", this);
        mon  = monitor::type_id::create("mon", this);
        sco  = scoreboard::type_id::create("sco", this);
        seqr = sequencer::type_id::create("seqr", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);

        drv.seq_item_port.connect(seqr.seq_item_export);
        
        mon.ap.connect(sco.analysis_export);
    endfunction
endclass