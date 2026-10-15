// Simultaneous read & write while empty
class tc_09;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset(); // FIFO is empty
        env.gen.run(1, 1, 1, 8'hFA);
    endtask
endclass