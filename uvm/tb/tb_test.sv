class tb_test extends uvm_test;

    `uvm_component_utils(tb_test)
    environment env;

    function new(
        string name,
        uvm_component parent
    );

        super.new(name, parent);
    endfunction

    function void build_phase(
        uvm_phase phase
    );

        super.build_phase(phase);
        env = environment::type_id::create(
            "env",
            this
        );

    endfunction

    task run_phase(
        uvm_phase phase
    );

        generator seq;

        phase.raise_objection(this);

        seq = generator::type_id::create(
            "seq",
            this
        );

        seq.start(env.seqr);

        #1ns;
        phase.drop_objection(this);

    endtask
endclass