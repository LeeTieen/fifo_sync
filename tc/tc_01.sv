//Check FIFO reset condition
class tc_01_reset;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        env.gen.run(1, 1, 0, 8'hA5);
        env.gen.run(1, 1, 0, 8'hB6);
        env.gen.reset();
        env.gen.run(1, 0, 1, 'b0);
    endtask
endclass