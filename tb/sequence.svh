
import st_encoder_pkg::*;

class TRANSACTION extends uvm_sequence_item;

    `uvm_object_utils(TRANSACTION)
    
    virtual dut_if dut_vif;
    virtual internal_if internal_vif;
    bit [71 : 0] MSG3_field0;
    bit [23 : 0] MSG3_field1;
    bit [47 : 0] MSG3_Z;
    bit [39 : 0] MSG3_field2;
    bit [31 : 0] MSG3_field3;
    bit [71 : 0] MSG4_field0;
    bit [23 : 0] MSG4_field1;
    bit [47 : 0] MSG4_Z;
    bit [39 : 0] MSG4_field2;
    bit [23 : 0] MSG4_field3;

  function new (string name = "");
    super.new(name);
  endfunction

endclass: TRANSACTION

class SEQUENCE extends uvm_sequence#(TRANSACTION);

  `uvm_object_utils(SEQUENCE)

  function new (string name = "");
    super.new(name);
  endfunction

  task body;
    ///////////////////////////////////////////////////////

    forever begin
        req = TRANSACTION::type_id::create("req");
        start_item(req);
        //
        // Get interfaces reference from config database
        if(!uvm_config_db#(virtual dut_if)::get(m_sequencer , "", "dut_vif", req.dut_vif)) begin
          `uvm_error("", "uvm_config_db::get failed")
        end
        if(!uvm_config_db#(virtual internal_if)::get(m_sequencer, "", "internal_vif", req.internal_vif)) begin
          `uvm_error("", "uvm_config_db::get failed")
        end

        req.MSG3_field0 = $urandom();
        req.MSG3_field1 = $urandom();
        req.MSG3_Z = $urandom();
        req.MSG3_field2 = $urandom();
        req.MSG3_field3 = $urandom();
        req.MSG4_field0 = $urandom();
        req.MSG4_field1 = $urandom();
        req.MSG4_Z = $urandom();
        req.MSG4_field2 = $urandom();
        req.MSG4_field3 = $urandom();


        @(posedge req.dut_vif.clk) begin
            if (~req.dut_vif.rst_n) begin
             `uvm_info("DUT", $sformatf("===============================   asserting reset: counter = %d ===================", req.internal_vif.counter), UVM_MEDIUM)
                req.internal_vif.counter          <= 0;
                req.dut_vif.MSG3_i_fields.field0_avail <= 0;
                req.dut_vif.MSG3_i_fields.field1_avail <= 0;
                req.dut_vif.MSG3_i_fields.Z_avail <= 0;
                req.dut_vif.MSG3_i_fields.field2_avail <= 0;
                req.dut_vif.MSG3_i_fields.field3_avail <= 0;
                req.dut_vif.MSG4_i_fields.field0_avail <= 0;
                req.dut_vif.MSG4_i_fields.field1_avail <= 0;
                req.dut_vif.MSG4_i_fields.Z_avail <= 0;
                req.dut_vif.MSG4_i_fields.field2_avail <= 0;
                req.dut_vif.MSG4_i_fields.field3_avail <= 0;

                req.internal_vif.msg_valid        <= 0;
                req.internal_vif.msg_type         <= 0;
                req.dut_vif.o_pkt_intf.ready      <= 0 ;
                req.dut_vif.i_msg_type.valid <= 0;
            end else begin
                req.dut_vif.o_pkt_intf.ready      <= 1 ;
             `uvm_info("DUT", $sformatf("===============================   releasing reset: counter = %d ===================", req.internal_vif.counter), UVM_MEDIUM)
                if (req.internal_vif.counter == 1) begin
                    req.dut_vif.i_msg_type.valid <= 1;
                end else begin
                    //req.internal_vif.msg_valid <= 1;
                    req.dut_vif.i_msg_type.valid <= 0;
                end
                if (req.internal_vif.counter == 15) begin
                    req.internal_vif.counter <= 0;
                    if (req.internal_vif.msg_type == 6) begin
                        req.internal_vif.msg_type <= 0;
                    end else begin
                        req.internal_vif.msg_type <= req.internal_vif.msg_type + 1 ;
                    end
                end else begin
                    req.internal_vif.counter <= req.internal_vif.counter +1;
                end

        
                //
                if (req.internal_vif.counter == 9) begin
                    req.dut_vif.MSG3_i_fields.field2_avail <= 1;
                    req.dut_vif.MSG3_i_fields.field2 <= {>> {req.MSG3_field2}};
                    req.dut_vif.MSG4_i_fields.field2_avail <= 1;
                    req.dut_vif.MSG4_i_fields.field2 <= {>> {req.MSG4_field2}};
                end else begin
                    req.dut_vif.MSG3_i_fields.field2_avail <= 0;
                    req.dut_vif.MSG4_i_fields.field2_avail <= 0;
                end 
                //
                if (req.internal_vif.counter == 7) begin
                    req.dut_vif.MSG3_i_fields.Z_avail <= 1;
                    req.dut_vif.MSG3_i_fields.Z <= {>> {req.MSG3_Z}};
                    req.dut_vif.MSG3_i_fields.field3_avail <= 1;
                    req.dut_vif.MSG3_i_fields.field3 <= {>> {req.MSG3_field3}};
                    req.dut_vif.MSG4_i_fields.Z_avail <= 1;
                    req.dut_vif.MSG4_i_fields.Z <= {>> {req.MSG4_Z}};
                    req.dut_vif.MSG4_i_fields.field3_avail <= 1;
                    req.dut_vif.MSG4_i_fields.field3 <= {>> {req.MSG4_field3}};
                end else begin
                    req.dut_vif.MSG3_i_fields.Z_avail <= 0;
                    req.dut_vif.MSG3_i_fields.field3_avail <= 0;
                    req.dut_vif.MSG4_i_fields.Z_avail <= 0;
                    req.dut_vif.MSG4_i_fields.field3_avail <= 0;
                end 
                //
                if (req.internal_vif.counter == 2) begin
                    req.dut_vif.MSG3_i_fields.field1_avail <= 1;
                    req.dut_vif.MSG3_i_fields.field1 <= {>> {req.MSG3_field1}};
                    req.dut_vif.MSG4_i_fields.field1_avail <= 1;
                    req.dut_vif.MSG4_i_fields.field1 <= {>> {req.MSG4_field1}};
                end else begin
                    req.dut_vif.MSG3_i_fields.field1_avail <= 0;
                    req.dut_vif.MSG4_i_fields.field1_avail <= 0;
                end 
                //
                if (req.internal_vif.counter == 3) begin
                    req.dut_vif.MSG3_i_fields.field0_avail <= 1;
                    req.dut_vif.MSG3_i_fields.field0 <= {>> {req.MSG3_field0}};
                    req.dut_vif.MSG4_i_fields.field0_avail <= 1;
                    req.dut_vif.MSG4_i_fields.field0 <= {>> {req.MSG4_field0}};
                end else begin
                    req.dut_vif.MSG3_i_fields.field0_avail <= 0;
                    req.dut_vif.MSG4_i_fields.field0_avail <= 0;
                end 
                //
            end
        end
        //
        req.dut_vif.i_msg_type.data  =  req.internal_vif.msg_type;
        //req.dut_vif.i_msg_type.valid = req.internal_vif.msg_valid;
        req.dut_vif.o_pkt_intf.ready = 1 ;
        ///////////////////////////////////////////////////////

        finish_item(req);
    end
  endtask: body

endclass: SEQUENCE

