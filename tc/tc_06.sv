//Check FIFO read while empty
class tc_06_read_empty;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        env.gen.run(1, 0, 1, 'b0);
    endtask
endclass