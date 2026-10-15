//Check FIFO write while full
class tc_04_write_full;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        for(int i = 0; i < DEPTH; i++) begin
            env.gen.run(1, 1, 0, i);
        end
        env.gen.run(1, 1, 0, 8'h22);
    endtask
endclass