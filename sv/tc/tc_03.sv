//Check FIFO full condition
class tc_03_full;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        for(int i = 0; i < DEPTH; i++) begin
            env.gen.run(1, 1, 0, i);
        end
    endtask
endclass