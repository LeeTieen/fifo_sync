package tb_pkg;
    parameter int DATA_WIDTH = 8;
    parameter int DEPTH = 8;
    
    `include "tb_trans.sv"
    `include "tb_gen.sv"
    `include "tb_drv.sv"
    `include "tb_mon.sv"
    `include "tb_cov.sv"
    `include "tb_sco.sv"
    `include "tb_env.sv"
    
endpackage
