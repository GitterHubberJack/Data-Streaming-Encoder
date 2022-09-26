class DRIVER extends uvm_driver #(TRANSACTION);

  `uvm_component_utils(DRIVER)

  virtual dut_if dut_vif;

  function new(string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  function void build_phase(uvm_phase phase);
    // Get interface reference from config database
    if(!uvm_config_db#(virtual dut_if)::get(this, "", "dut_vif", dut_vif)) begin
      `uvm_error("", "uvm_config_db::get failed")
    end
  endfunction 

  task run_phase(uvm_phase phase);
    // Now drive normal traffic
    forever begin
      seq_item_port.get_next_item(req);
      seq_item_port.item_done();
    end
  endtask

endclass: DRIVER

