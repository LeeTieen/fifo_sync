class scoreboard extends uvm_scoreboard;

  `uvm_component_utils(scoreboard)

  uvm_analysis_imp #(
      transaction,
      scoreboard
  ) analysis_export;

  bit [DATA_WIDTH-1:0] mem[$];

  int error_count = 0;

  function new (string name, uvm_component parent);
    super.new(name, parent);
    analysis_export = new("analysis_export", this);
  endfunction

  function void write(
    transaction t
  );
    bit read_ok;
    bit write_ok;

    bit [DATA_WIDTH-1:0] expected_dout;

    if (!t.rst_n) begin
    mem.delete();

    if (t.dout !== '0) begin
      `uvm_error(
        "[SCO]",
        "Reset: dout should be 0"
      )
      error_count++;
    end

    if (t.empty !== 1'b1) begin
      `uvm_error(
        "[SCO]",
        "Reset: empty should be 1"
      )
      error_count++;
    end

    if(t.full !== 1'b0) begin
      `uvm_error(
        "SCO",
        "Reset: full should be 0"
      )
    end
    return;
  end

  read_ok   = t.rd_en && (mem.size() > 0);
  write_ok  = t.wr_en && (mem.size() < DEPTH);

  if (read_ok) begin
    expected_dout = mem.pop_front();

    assert (t.dout === expected_dout) 
    else begin
      `uvm_error(
        "[SCO]",
        $sformatf(
          "READ FAIL: expected = %h, actual = %h",
          expected_dout,
          t.dout
        )
      )
      error_count++;
    end
  end

  else if (t.rd_en) begin
    assert (t.dout === '0) 
    else begin
      `uvm_error(
        "[SCO]",
        $sformatf(
          "READ REJECTED: FIFO empty, dout = %h",
          t.dout
        )
      )
      error_count++;
    end
  end

  if (write_ok) mem.push_back(t.din);
  else if (t.wr_en) begin
    `uvm_error(
      "[SCO]",
      "WRITE REJECTED: FIFO full"
    )
    error_count++;
  end

  assert (t.full === (mem.size() == DEPTH)) 
  else begin
    `uvm_error(
      "[SCO]",
      "FULL FLAG FAIL"
    )
    error_count++;
  end

  assert (t.empty === (mem.size() == 0))
  else begin
    `uvm_error(
        "SCO",
        "EMPTY FLAG FAIL"
    )

error_count++;

end
  endfunction

  
  function void report_phase(
    uvm_phase phase
  );
    super.report_phase(phase);

    if (error_count == 0) begin
      `uvm_info(
        "[SCO]",
        "TEST RESULT: PASS",
        UVM_NONE
      )
    end

    else begin
      `uvm_error(
        "[SCO]",
        $sformatf(
          "TEST RESULT: FAIL - %0d error(s)",
          error_count
        )
      )
    end
    
  endfunction

endclass
