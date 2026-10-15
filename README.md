# fifo_sync

Design and verification of a simple synchronous FIFO buffer.

The repository contains two verification versions:

| Folder | Contents | Status |
|---|---|---|
| [`sv/`](sv/) | Original SystemVerilog design, testbench, testcases, and coverage report | Existing implementation |
| [`uvm/`](uvm/) | UVM mapping of the FIFO verification environment | Initial implementation; reset/idle sequence |

```text
FIFO_SYNC/
├── docs/
│   └── fifo_spec.md
├── sv/
│   ├── rtl/
│   ├── tb/
│   ├── tc/
│   ├── sim/
│   ├── Makefile
│   ├── config.mk
│   ├── compile.f
│   ├── compile_tb.f
│   └── README.md
├── uvm/
│   ├── rtl/
│   ├── tb/
│   ├── tc/
│   ├── sim/
│   ├── Makefile
│   ├── config.mk
│   ├── compile.f
│   ├── compile_tb.f
│   └── README.md
├── .gitattributes
├── .gitignore
└── README.md
```


See the [FIFO specification](docs/fifo_spec.md),
[saved coverage report](sv/sim/coverage/html/index.html).

