// Idle state
class tc_00_idle;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        env.gen.run(1, 0, 0, 'b0);
    endtask
endclass