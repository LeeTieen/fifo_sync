// Simultaneous read & write
class tc_07_read_write;
    environment env;
    function new(environment env);
        this.env = env;
    endfunction
    
    task test();
        env.gen.reset();
        env.gen.run(1, 1, 0, 8'hA5);
        
        env.gen.run(1, 1, 1, 8'hB6);
    endtask
endclass