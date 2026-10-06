// Simultaneous read & write while full
class tc_08;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        for(int i = 0; i < DEPTH; i++) begin
            env.gen.run(1, 1, 0, i);
        end

        env.gen.run(1, 1, 1, 8'hB2);
    endtask
endclass
