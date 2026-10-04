class environment;

    transaction t;
    mailbox gdmbx;
    mailbox msmbx;
    monitor mon;
    scoreboard sco;
    driver drv;
    generator gen;
    event done;
    virtual fifo_inf #(DATA_WIDTH) vif;

    function new();
        gdmbx = new();
        msmbx = new();

        gen = new(gdmbx);
        drv = new(gdmbx);

        mon = new(msmbx);
        sco = new(msmbx);
    endfunction
  
    task run();
        gen.done = done;
        drv.done = done;

        mon.vif = vif;
        drv.vif = vif;
        
        fork
            mon.run();
            sco.run();
            drv.run();
        join_none
    endtask
endclass
