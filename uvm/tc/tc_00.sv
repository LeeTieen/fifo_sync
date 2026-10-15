//Idle state
class tc_00_idle extends generator;
    `uvm_object_utils(tc_00_idle)

    function new(
        string name = "tc_00_idle"
    );

        super.new(name);
    endfunction

    task body();
        run(0, 0, 0, '0);
        run(1, 0, 0, '0);
    endtask
endclass