class tc_10;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction

    task test();
        env.gen.reset();
        for(int i = 0; i < DEPTH; i++) begin
            env.gen.run(1, 1, 0, i);
        end
        for(int j = 0; j < 4; j++) begin
            env.gen.run(1, 0, 1, 'b0);
        end
        for(int k = 0; k < 4; k++) begin
            env.gen.run(1, 1, 0, DEPTH + k);
        end
        for(int q = 0; q < DEPTH; q++) begin
            env.gen.run(1, 0, 1, 'b0);
        end
    endtask
endclass