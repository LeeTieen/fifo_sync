# FIFO verification with UVM

This folder contains the initial UVM mapping of the
[SystemVerilog FIFO project](../sv/).

The implementation includes a sequence item, sequence, sequencer, driver,
monitor, scoreboard, environment, and test. `tb_top` sets the virtual interface
through `uvm_config_db` and calls `run_test("tb_test")`.

The current test starts the base `generator` sequence, which applies reset and
idle transactions. `tc/tc_00.sv` also defines an idle sequence; `tb_test` does not
select it directly. The remaining directed scenarios and functional coverage
have not been mapped into this version.

Use the [FIFO specification](../docs/fifo_spec.md) as the behavioral reference.

