//Check FIFO write condition
class tc_02_write;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        env.gen.run(1, 1, 0, 8'hFF);
        env.gen.run(1, 0, 1, 'b0);
    endtask
endclass