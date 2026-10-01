class scoreboard #(
    parameter int DEPTH = 8
  );

  transaction t;
  mailbox mbx;

  function new(mailbox mbx);
    this.mbx = mbx;
  endfunction

  bit [DATA_WIDTH-1:0] expected_dout;
  bit expected_full;
  bit expected_empty;
  bit read;
  bit write;
  bit read_write;

  bit mem[$];

  task run(
      bit rst_n,
      bit rd_en,
      bit wr_en,
      bit [DATA_WIDTH-1:0] din,

      bit [DATA_WIDTH-1:0] dout,
      bit full,
      bit empty
    );
    forever begin
      t = new();
      mbx.get(t);

      if(!t.rst_n)
      begin
        mem.delete();
        expected_empty = (mem.size() == 0);
        expected_full = (mem.size() == DEPTH);
        expected_dout = '0;

        if((expected_empty == t.empty) && (expected_full == t.full)) begin
          $display("[SCO]: PASS reset\n");
          $display("Received: expected_dout = %b, dout = %b\n,expected_full = %b, full = %b\n, expected_empty = %b, empty = %b\n",expected_dout, t.dout, expected_full, t.full, expected_full, t.empty);
        end
        else begin
          $$display("[SCO]: FAIL reset\n");
          $display("Received: expected_dout = %b, dout = %b\n,expected_full = %b, full = %b\n, expected_empty = %b, empty = %b\n",expected_dout, t.dout, expected_full, t.full, expected_full, t.empty);
        end
      end
      else
      begin
        read = rd_en && !empty;
        write = wr_en && !full;

        if(read)
            begin
                expected_dout = mem.pop_front();
                if (expected_dout == t.dout) begin
                    $display("[SCO]: PASS read\n");
                    $display("Received: expected_dout = %b, dout = %b\n,expected_full = %b, full = %b\n, expected_empty = %b, empty = %b\n",expected_dout, t.dout, expected_full, t.full, expected_full, t.empty);
                end
                else begin
                    $$display("[SCO]: FAIL read\n");
                    $display("Received: expected_dout = %b, dout = %b\n,expected_full = %b, full = %b\n, expected_empty = %b, empty = %b\n",expected_dout, t.dout, expected_full, t.full, expected_full, t.empty);
                end
            end

        if(write)
            begin
                mem.push_front(t.din);
            end

    end
    end
  endtask
endclass
