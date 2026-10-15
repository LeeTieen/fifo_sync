class scoreboard #(
    parameter int DEPTH = 8
  );

  transaction t;
  mailbox mbx;
  coverage cov;

  function new(mailbox mbx, coverage cov);
    this.mbx = mbx;
    this.cov = cov;
  endfunction

  bit [DATA_WIDTH-1:0] expected_dout;
  bit expected_full;
  bit expected_empty;
  bit read;
  bit write;

  bit [DATA_WIDTH-1:0] mem[$];
  
  task run();
    forever begin
        mbx.get(t);

        if(!t.rst_n)
          begin
            mem.delete();
            expected_empty = (mem.size() == 0);
            expected_full = (mem.size() == DEPTH);
            expected_dout = 'b0;

            assert((expected_empty == t.empty) && (expected_full == t.full) && (expected_dout == t.dout))
            else 
              begin
                $error("[SCO]: FAIL reset");
                $display("Received: expected_dout = %b, dout = %b\n,expected_full = %b, full = %b\n, expected_empty = %b, empty = %b\n",expected_dout, t.dout, expected_full, t.full, expected_empty, t.empty);
              end
          end
        else begin
            read = t.rd_en && (mem.size() > 0);
            write = t.wr_en && (mem.size() < DEPTH);
        if(t.rd_en)
          begin
            if(read) 
              expected_dout = mem.pop_front();
            else 
              expected_dout = 'b0;

            assert(expected_dout == t.dout)
            else 
              begin
                  $error("[SCO]: FAIL read");
                  $display("Received: expected_dout = %b, dout = %b\n",expected_dout, t.dout);
              end
                
          end

          if(t.wr_en)
            begin
              if (write) mem.push_back(t.din);
            end

          expected_full = (mem.size() == DEPTH);
          expected_empty = (mem.size() == 0);

        end

          assert (t.full == expected_full && t.empty == expected_empty) 
          else   
            begin
              $error("[SCO]: FAIL full/empty flag check\n");
              $display("Receive: full = %b, empty = %b, expected_full = %b, expected_empty = %b\n", t.full, t.empty, expected_full, expected_empty);
            end

            cov.sample(t);
    end
  endtask
endclass
