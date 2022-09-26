package testbench_pkg;
  import uvm_pkg::*;
  
  // The UVM sequence, transaction item, and driver are in these files:
  `include "sequence.svh"
  `include "driver.svh"
  
  // The agent contains sequencer, driver, and monitor (not included)
  class AGENT extends uvm_agent;
    `uvm_component_utils(AGENT)
    
    DRIVER driver;
    uvm_sequencer#(TRANSACTION) sequencer;
    
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    
    function void build_phase(uvm_phase phase);
      driver = DRIVER ::type_id::create("driver", this);
      sequencer =
        uvm_sequencer#(TRANSACTION)::type_id::create("sequencer", this);
    endfunction    
    
    // In UVM connect phase, we connect the sequencer to the driver.
    function void connect_phase(uvm_phase phase);
      driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
    
    task run_phase(uvm_phase phase);
      // We raise objection to keep the test from completing
      phase.raise_objection(this);
      begin
        SEQUENCE seq;
        seq = SEQUENCE::type_id::create("seq");
        seq.start(sequencer);
      end
      // We drop objection to allow the test to complete
      phase.drop_objection(this);
    endtask

  endclass
  
  class ENV extends uvm_env;
    `uvm_component_utils(ENV)
    
    AGENT agent;

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    
    function void build_phase(uvm_phase phase);
      agent = AGENT::type_id::create("agent", this);
    endfunction

  endclass
  
  class TEST extends uvm_test;
    `uvm_component_utils(TEST)
    
    ENV env;

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    
    function void build_phase(uvm_phase phase);
      env = ENV::type_id::create("env", this);
    endfunction
    
    task run_phase(uvm_phase phase);
      // We raise objection to keep the test from completing
      phase.raise_objection(this);
      #10;
      `uvm_info("", "Start Testing", UVM_MEDIUM)
      // We drop objection to allow the test to complete
      phase.drop_objection(this);
    endtask

  endclass
  
endpackage

