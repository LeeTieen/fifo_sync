class generator extends uvm_sequence #(transaction);
  
    `uvm_object_utils(generator)
    
    //Constructor
    function new(string name = "generator");
        super.new(name);
    endfunction

    task run(
            bit                   rst_n,
            bit                   wr_en,
            bit                   rd_en,
            bit [DATA_WIDTH-1:0]  din
        );
            transaction t;

            t = transaction::type_id::create("t");

            start_item(t);

            t.rst_n = rst_n;
            t.wr_en = wr_en;
            t.rd_en = rd_en;
            t.din = din;

            finish_item(t);
    endtask

    task body();
        run(0, 0, 0, 'b0);
        run(1, 0, 0, 'b0);
    endtask
endclass

