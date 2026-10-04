# Cau hinh project cho Windows + QuestaSim 10.7c
# Chay trong thu muc chua Makefile bang mingw32-make.

TOP_MODULE := fifo_sync
TB_NAME := tb_fifo_sync

QUESTA_HOME := D:/Mentor_Graphics_QuestaSim/questasim64_10.7c
QUESTA_BIN := $(QUESTA_HOME)/win64

# Testbench class SystemVerilog thuan: USE_UVM=0.
# Neu code thuc su import uvm_pkg, dung USE_UVM=1 va sua UVM_HOME/UVM_LIB
# theo cac thu muc ton tai trong bo cai QuestaSim Windows.
USE_UVM ?= 0
UVM_HOME ?= $(QUESTA_HOME)/verilog_src/uvm-1.2
UVM_LIB ?= $(QUESTA_HOME)/uvm-1.2
UVM_LIB_NAME ?= uvm
