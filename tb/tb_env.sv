class environment;

    transaction t;
    mailbox gdmbx;
    mailbox msmbx;
    monitor mon;
    scoreboard sco;
    driver drv;
    generator gen;
    event done;
    coverage cov;


    virtual fifo_inf #(DATA_WIDTH) vif;

    function new();
        cov = new();
        gdmbx = new();
        msmbx = new();

        gen = new(gdmbx);
        drv = new(gdmbx);

        mon = new(msmbx);
        sco = new(msmbx, cov);
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
