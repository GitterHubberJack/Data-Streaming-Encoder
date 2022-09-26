/*******************************************
UVM StEncoder testbench.
*******************************************/

`include "uvm_macros.svh"
`include "testbench_pkg.svh"
`include "dut_if.svh"

`timescale 1 ns / 1 ps
module tb;
import uvm_pkg::*;
import testbench_pkg::*;
//
// Instantiate the interfaces
st_intf i_msg_type();
st_pkt_intf o_pkt_intf();
internal_if internal_if1();
 

dut_if dut_if1 (
    .i_msg_type(i_msg_type),
    .o_pkt_intf(o_pkt_intf)
);

st_encoder st_encoder_inst (
    .rst_n(dut_if1.rst_n),
    .clk(dut_if1.clk),
    .i_msg_type(dut_if1.i_msg_type.slave),
    .o_pkt_intf(dut_if1.o_pkt_intf.master),
    .MSG3_i_fields(dut_if1.MSG3_i_fields),
    .MSG4_i_fields(dut_if1.MSG4_i_fields)
);

initial begin
    dut_if1.clk = 0;
    forever #25 dut_if1.clk = ~dut_if1.clk;
end
  
initial begin
    dut_if1.rst_n = 0;
    #1000 dut_if1.rst_n = 1;
end
initial begin
  // Place the interface into the UVM configuration database
  uvm_config_db#(virtual dut_if)::set(null, "*", "dut_vif", dut_if1);
  uvm_config_db#(virtual internal_if)::set(null, "*", "internal_vif",internal_if1);
  // Start the test
  run_test("TEST");
end

// Dump waves
initial begin
  `uvm_info("DUT", "
===============================   Dumping the vcd waveform ==========================
", UVM_LOW);
  $dumpfile("wave.vcd");
  $dumpvars(0, tb);
end
endmodule

