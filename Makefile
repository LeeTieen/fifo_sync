# Windows/PowerShell: mingw32-make build; mingw32-make run
# Tat ca duong dan source duoc tinh tu thu muc chua Makefile.

ifeq ($(wildcard config.mk),)
$(error Missing config.mk in the current directory)
endif
include config.mk

SHELL := cmd.exe
.SHELLFLAGS := /D /C
.NOTPARALLEL:

# Defaults for the Windows installation; config.mk may override them.
QUESTA_HOME ?= D:/Mentor_Graphics_QuestaSim/questasim64_10.7c
QUESTA_BIN ?= $(QUESTA_HOME)/win64
USE_UVM ?= 0
UVM_HOME ?= $(QUESTA_HOME)/verilog_src/uvm-1.2
UVM_LIB ?= $(QUESTA_HOME)/uvm-1.2
UVM_LIB_NAME ?= uvm

RTL_DIR := rtl
TB_DIR := tb
TC_DIR := tc
SIM_DIR := sim
COMPILE_FILE := compile.f
TB_COMPILE_FILE := compile_tb.f
TESTNAME ?= $(if $(wildcard tc/tc_pkg.sv tc_pkg.sv),tc_pkg,$(basename $(notdir $(firstword $(wildcard tc/*.sv tc_*.sv)))))
# These are filenames. TB_NAME in config.mk must be the actual top module name.
INTERFACE_FILE ?= $(if $(wildcard tb/tb_inf.sv tb_inf.sv),tb_inf.sv,tb_fifo_sync_inf.sv)
TB_PACKAGE_FILE ?= $(if $(wildcard tb/tb_pkg.sv tb_pkg.sv),tb_pkg.sv,tb_fifo_sync_pkg.sv)
TB_TOP_FILE ?= $(if $(wildcard tb/tb_top.sv tb_top.sv),tb_top.sv,$(TB_NAME).sv)
TB_MODULE ?= $(TB_NAME)
RADIX ?= bin

VLIB := "$(QUESTA_BIN)/vlib.exe"
VMAP := "$(QUESTA_BIN)/vmap.exe"
VLOG := "$(QUESTA_BIN)/vlog.exe"
VSIM := "$(QUESTA_BIN)/vsim.exe"
VCOVER := "$(QUESTA_BIN)/vcover.exe"

ifeq ($(USE_UVM),1)
UVM_INC := $(UVM_HOME)/src
UVM_VLOG_OPT := -L $(UVM_LIB_NAME) "+incdir+$(UVM_INC)"
UVM_VSIM_OPT := -L $(UVM_LIB_NAME)
UVM_MAP := $(VMAP) $(UVM_LIB_NAME) "$(UVM_LIB)"
else
UVM_VLOG_OPT :=
UVM_VSIM_OPT :=
UVM_MAP := echo UVM disabled (USE_UVM=0)
endif

# Windows QuestaSim handles its own DPI compiler. No Linux /usr/bin/gcc.
VLOG_COMMON := -sv $(UVM_VLOG_OPT) +incdir+../rtl +incdir+../tb +incdir+../tc
# Recursive expansion: init can move source files before build selects them.
# Override TB_SOURCES to compile a subset; paths are relative to sim/.
TB_SOURCES ?= ../tb/$(INTERFACE_FILE) ../tb/$(TB_PACKAGE_FILE) ../tc/$(TESTNAME).sv ../tb/$(TB_TOP_FILE)

.PHONY: all all_wave all_mem all_cov init prepare_dirs gen_filelist check_uvm build build_tb build_cov run run_cov mem wave gen_cov gen_html clean help

all: build run
all_wave: build run wave
all_mem: build mem
all_cov: build_cov run_cov

# Directory creation is safe to repeat during every build.
prepare_dirs:
	@if not exist "$(RTL_DIR)" mkdir "$(RTL_DIR)"
	@if not exist "$(TB_DIR)" mkdir "$(TB_DIR)"
	@if not exist "$(TC_DIR)" mkdir "$(TC_DIR)"
	@if not exist "$(SIM_DIR)/log" mkdir "$(SIM_DIR)/log"

# Generate ordered source lists for vlog -f. The compiler runs in sim/.
# Class files included by the TB package must not also be listed individually.
define WRITE_FILELISTS
powershell.exe -NoProfile -NonInteractive -Command "$$ErrorActionPreference = 'Stop'; $$tb = @('$(TB_SOURCES)' -split '\s+' | Where-Object { $$_ }); $$tbLines = @('// Generated testbench list. Paths are relative to sim/.') + @($$tb | ForEach-Object { [string][char]34 + $$_ + [string][char]34 }); $$fullLines = @('// Generated full list. Paths are relative to sim/.', ([string][char]34 + '../rtl/$(TOP_MODULE).sv' + [string][char]34)) + $$tbLines; $$enc = New-Object System.Text.UTF8Encoding($$false); [System.IO.File]::WriteAllLines((Join-Path (Get-Location).Path '$(COMPILE_FILE)'), [string[]]$$fullLines, $$enc); [System.IO.File]::WriteAllLines((Join-Path (Get-Location).Path '$(TB_COMPILE_FILE)'), [string[]]$$tbLines, $$enc)"
endef

gen_filelist:
	@$(WRITE_FILELISTS)
	@echo Generated $(COMPILE_FILE) and $(TB_COMPILE_FILE).

# Run init explicitly when arranging root source files.
# Build compiles the files already in rtl/, tb/, tc/.
init: prepare_dirs
	@powershell.exe -NoProfile -NonInteractive -Command "$$ErrorActionPreference = 'Stop'; $$root = (Get-Location).Path; $$moves = @(Get-ChildItem -LiteralPath $$root -File | Where-Object { $$_.Extension -in '.v','.sv' } | ForEach-Object { if ($$_.Name -like 'tb_*') { $$dir = 'tb' } elseif ($$_.Name -like 'tc_*') { $$dir = 'tc' } else { $$dir = 'rtl' }; [PSCustomObject]@{ Source = $$_.FullName; Destination = (Join-Path (Join-Path $$root $$dir) $$_.Name) } }); foreach ($$item in $$moves) { if (Test-Path -LiteralPath $$item.Destination) { throw ('Destination already exists: ' + $$item.Destination) } }; foreach ($$item in $$moves) { Move-Item -LiteralPath $$item.Source -Destination $$item.Destination; Write-Host ('Moved: ' + $$item.Source + ' -> ' + $$item.Destination) }"
	@$(WRITE_FILELISTS)
	@echo Project directories, source files and compile lists ready.

check_uvm:
	@if not exist "$(QUESTA_BIN)/vlog.exe" (echo Missing $(QUESTA_BIN)/vlog.exe & exit /b 1)
	@if not exist "$(QUESTA_BIN)/vsim.exe" (echo Missing $(QUESTA_BIN)/vsim.exe & exit /b 1)
ifeq ($(USE_UVM),1)
	@if not exist "$(UVM_INC)/uvm_pkg.sv" (echo Missing $(UVM_INC)/uvm_pkg.sv & exit /b 1)
	@if not exist "$(UVM_LIB)" (echo Missing UVM library $(UVM_LIB) & exit /b 1)
endif
	@echo QuestaSim tools found. USE_UVM=$(USE_UVM)

build: check_uvm prepare_dirs gen_filelist
	@if not exist "$(RTL_DIR)/$(TOP_MODULE).sv" (echo Missing $(RTL_DIR)/$(TOP_MODULE).sv & exit /b 1)
	@if not exist "$(TC_DIR)/$(TESTNAME).sv" (echo Missing $(TC_DIR)/$(TESTNAME).sv. Use TESTNAME=your_file_name_without_sv & exit /b 1)
	@echo BUILD: $(TESTNAME)
	@cd /d "$(SIM_DIR)" && if not exist work $(VLIB) work
	@cd /d "$(SIM_DIR)" && $(VMAP) work work
ifeq ($(USE_UVM),1)
	@cd /d "$(SIM_DIR)" && $(UVM_MAP)
endif
	@cd /d "$(SIM_DIR)" && $(VLOG) $(VLOG_COMMON) -f ../$(COMPILE_FILE) -l log/compile.log
	@echo Build complete. Log: $(SIM_DIR)/log/compile.log

# Compile testbench sources only. Does not elaborate or run the DUT.
# Dependent types/packages/classes must still be available.
build_tb: check_uvm prepare_dirs gen_filelist
	@echo BUILD TESTBENCH ONLY
	@cd /d "$(SIM_DIR)" && if not exist work $(VLIB) work
	@cd /d "$(SIM_DIR)" && $(VMAP) work work
ifeq ($(USE_UVM),1)
	@cd /d "$(SIM_DIR)" && $(UVM_MAP)
endif
	@cd /d "$(SIM_DIR)" && $(VLOG) $(VLOG_COMMON) -f ../$(TB_COMPILE_FILE) -l log/compile_tb.log
	@echo Testbench compile complete. Log: $(SIM_DIR)/log/compile_tb.log

build_cov: check_uvm prepare_dirs gen_filelist
	@if not exist "$(RTL_DIR)/$(TOP_MODULE).sv" (echo Missing $(RTL_DIR)/$(TOP_MODULE).sv & exit /b 1)
	@if not exist "$(TC_DIR)/$(TESTNAME).sv" (echo Missing $(TC_DIR)/$(TESTNAME).sv. Use TESTNAME=your_file_name_without_sv & exit /b 1)
	@echo BUILD COVERAGE: $(TESTNAME)
	@cd /d "$(SIM_DIR)" && if not exist work $(VLIB) work
	@cd /d "$(SIM_DIR)" && $(VMAP) work work
ifeq ($(USE_UVM),1)
	@cd /d "$(SIM_DIR)" && $(UVM_MAP)
endif
	@cd /d "$(SIM_DIR)" && $(VLOG) $(VLOG_COMMON) +cover=bcesft -f ../$(COMPILE_FILE) -l log/compile.log
	@echo Coverage build complete. Log: $(SIM_DIR)/log/compile.log

run:
	@echo RUN: $(TB_MODULE)
	@cd /d "$(SIM_DIR)" && $(VSIM) $(UVM_VSIM_OPT) -l sim.log -voptargs=+acc -assertdebug -c $(TB_MODULE) -do "log -r /*; run -all; quit"

run_cov:
	@cd /d "$(SIM_DIR)" && $(VSIM) $(UVM_VSIM_OPT) -coverage -l sim.log -c $(TB_MODULE) -voptargs=+cover=bcesft -assertdebug -do "coverage save -onexit coverage.ucdb; log -r /*; run -all; quit"

mem:
	@cd /d "$(SIM_DIR)" && $(VSIM) $(UVM_VSIM_OPT) -gui -voptargs=+acc -assertdebug $(TB_MODULE) -do "view memory; add wave -r /*; run -all"

wave:
	@cd /d "$(SIM_DIR)" && $(VSIM) -view vsim.wlf -do "add wave -r /*; radix -$(RADIX)"

gen_cov:
	@if not exist "$(SIM_DIR)/coverage" mkdir "$(SIM_DIR)/coverage"
	@cd /d "$(SIM_DIR)" && $(VCOVER) merge coverage/IP.ucdb coverage.ucdb
	@cd /d "$(SIM_DIR)" && $(VCOVER) report coverage/IP.ucdb -output coverage/summary_report.txt
	@cd /d "$(SIM_DIR)" && $(VCOVER) report -zeros -details -code bcesft -annotate -All -codeAll coverage/IP.ucdb -output coverage/detail_report.txt

gen_html:
	@if not exist "$(SIM_DIR)/coverage" mkdir "$(SIM_DIR)/coverage"
	@cd /d "$(SIM_DIR)" && $(VCOVER) merge coverage/IP.ucdb coverage.ucdb
	@cd /d "$(SIM_DIR)" && $(VCOVER) report -zeros -details -code bcesft -annotate -testhitdataAll -html coverage/IP.ucdb

clean:
	@if exist "$(SIM_DIR)/work" rmdir /s /q "$(SIM_DIR)/work"
	@if exist "$(SIM_DIR)/log" rmdir /s /q "$(SIM_DIR)/log"
	@if exist "$(SIM_DIR)/coverage" rmdir /s /q "$(SIM_DIR)/coverage"
	@if exist "$(SIM_DIR)/modelsim.ini" del /q "$(SIM_DIR)/modelsim.ini"
	@if exist "$(SIM_DIR)/sim.log" del /q "$(SIM_DIR)/sim.log"
	@if exist "$(SIM_DIR)/coverage.ucdb" del /q "$(SIM_DIR)/coverage.ucdb"
	@if exist "$(SIM_DIR)/vsim.wlf" del /q "$(SIM_DIR)/vsim.wlf"
	@echo Simulation outputs removed.

help:
	@echo Run from the folder containing Makefile with Windows PowerShell.
	@echo mingw32-make build_tb TESTNAME=tc_pkg  -- testbench compile only
	@echo mingw32-make build TESTNAME=tc_pkg
	@echo mingw32-make run
	@echo mingw32-make all TESTNAME=tc_pkg
	@echo init moves root source files. Run it explicitly when setting up the project.
	@echo build uses compile.f. build_tb uses compile_tb.f.
	@echo gen_filelist regenerates both source lists without compiling.
	@echo Other targets: init, check_uvm, build_cov, run_cov, wave, mem, gen_cov, gen_html, clean.
	@echo For UVM-based code: mingw32-make all TESTNAME=tc_pkg USE_UVM=1
