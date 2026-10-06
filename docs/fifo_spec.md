# Synchronous FIFO - Project Specification

This document is the project-specific behavioral reference for `fifo_sync`. It
describes the external behavior that the testbench checks. The implementation
of storage, pointers, and counters is intentionally left to the RTL.

## Scope and default configuration

The DUT is a single-clock, synchronous FIFO with first-in, first-out ordering.
The default project configuration is:

| Parameter | Default | Meaning |
|---|---:|---|
| `DATA_WIDTH` | 8 | Width of one stored entry |
| `DEPTH` | 8 | Maximum number of stored entries |

All operations are sampled on the rising edge of `clk`.

## Interface

| Signal | Direction | Width | Description |
|---|---|---:|---|
| `clk` | input | 1 | Sampling clock |
| `rst_n` | input | 1 | Synchronous, active-low reset |
| `wr_en` | input | 1 | Write request |
| `rd_en` | input | 1 | Read request |
| `din` | input | `DATA_WIDTH` | Data presented for a valid write |
| `dout` | output | `DATA_WIDTH` | Registered read data |
| `full` | output | 1 | FIFO contains `DEPTH` entries |
| `empty` | output | 1 | FIFO contains zero entries |

`din` is meaningful only for an accepted write. When `wr_en` is low, or when
the FIFO is full, its value must not affect the FIFO contents.

## Acceptance rules

The request decisions use the FIFO state immediately before the rising edge:

```text
write_accept = rst_n && wr_en && !full_before
read_accept  = rst_n && rd_en && !empty_before
```

When `rst_n == 0`, reset has priority and both requests are ignored.

## Reset behavior

When `rst_n == 0` at a rising edge:

- all logical FIFO contents are discarded;
- `empty` becomes `1`;
- `full` becomes `0`;
- `dout` becomes `0`;
- simultaneous read/write requests at that edge have no effect.

Reset is synchronous; changing `rst_n` between clock edges does not by itself
change the outputs.

## Write behavior

When `rst_n == 1`, `wr_en == 1`, and the FIFO is not full, `din` is appended to
the tail. The data must be returned by a later valid read in the same order in
which it was accepted.

When the FIFO is full, a write request is rejected. It must not change the
logical contents or make the rejected data visible on a later read.

## Read behavior

When `rst_n == 1`, `rd_en == 1`, and the FIFO is not empty, the oldest accepted
entry is removed and presented on registered output `dout` after the edge.

When the FIFO is empty, a read request is rejected. The FIFO remains empty and
`dout` is set to `0`.

When there is no accepted read, `dout` retains its previous value unless reset
or an empty-read operation sets it to `0`.

## Simultaneous read and write

The requests are evaluated against the state before the edge:

| State before edge | `wr_en` | `rd_en` | Result |
|---|---:|---:|---|
| Neither empty nor full | 1 | 1 | Both accepted; oldest entry goes to `dout`, `din` is appended, occupancy is unchanged |
| Full | 1 | 1 | Read accepted, write rejected; oldest entry goes to `dout`, occupancy decreases by one |
| Empty | 1 | 1 | Write accepted, read rejected; `dout = 0`, `din` becomes the first stored entry |

## Verification scenarios

The project should cover at least these scenarios:

| ID | Scenario | Expected result |
|---|---|---|
| TC-01 | Synchronous reset | FIFO becomes empty, `full=0`, `dout=0` |
| TC-02 | Normal write | Accepted data is stored and FIFO is no longer empty |
| TC-03 | Fill to full | `DEPTH` writes are accepted; `full=1` |
| TC-04 | Write while full | Write is rejected and contents do not change |
| TC-05 | Normal read | Oldest entry is returned on `dout` |
| TC-06 | Read while empty | Read is rejected; `empty=1`, `dout=0` |
| TC-07 | Simultaneous read/write | Both operations are accepted in a normal state |
| TC-08 | Simultaneous read/write while full | Read accepted, write rejected |
| TC-09 | Simultaneous read/write while empty | Write accepted, read rejected |
| TC-10 | Ordering and pointer wrap-around | All accepted data is returned exactly once and in order |

## Scoreboard model

The scoreboard uses a queue containing only accepted writes. For every sampled
cycle it shall:

- clear the queue on reset;
- push `din` only for an accepted write;
- pop and compare the front entry only for an accepted read;
- check `full` and `empty` against the queue size;
- confirm rejected writes do not modify the queue;
- confirm rejected reads do not remove an entry.

The testbench must check behavior through the interface and must not depend on
internal RTL signals such as memory arrays, pointers, or counters.

## Functional coverage targets

Functional coverage should include:

- idle, read-only, write-only, and simultaneous read/write operations;
- empty and full states;
- read while empty and write while full;
- simultaneous read/write while empty and while full;
- data values `0x00`, `0xFF`, low, middle, and high value ranges;
- transitions `empty -> non-empty`, `non-full -> full`, `full -> non-full`, and
  `non-empty -> empty`.

## Completion criteria

The project is complete when:

- all directed scenarios pass;
- scoreboard mismatches are zero;
- assertion failures are zero;
- functional coverage is at least 95%;
- statement coverage is at least 95%;
- branch coverage is at least 90%.

