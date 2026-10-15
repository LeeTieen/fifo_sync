class coverage;
    covergroup fifo_cg with function sample(
        bit                     rst_n,
        bit                     wr_en,
        bit                     rd_en,
        bit                     full_before,
        bit                     empty_before,
        bit [DATA_WIDTH-1:0]    din
    );

        cp_operation: coverpoint{wr_en, rd_en} iff (rst_n){
            bins idle       = {2'b00};
            bins read_only  = {2'b01};
            bins write_only = {2'b10};
            bins read_write = {2'b11};
        }

        cp_state:     coverpoint{full_before, empty_before} iff (rst_n){
            bins normal          = {2'b00};
            bins full            = {2'b10};
            bins empty           = {2'b01};
            illegal_bins invalid = {2'b11};
        }

        operation_state: cross cp_operation, cp_state;

        cp_data:      coverpoint din iff (rst_n && wr_en && !full_before){
            bins zero   = {8'h00};
            bins low    = {[8'h01:8'h54]};
            bins middle = {[8'h55:8'hAA]};
            bins high   = {[8'hAB:8'hFE]};
            bins max    = {8'hFF};
        }
    endgroup

    function new();
        fifo_cg = new();
    endfunction

    function void sample(transaction t);
        fifo_cg.sample(t.rst_n, t.wr_en, t.rd_en, t.full_before, t.empty_before, t.din);
    endfunction
endclass