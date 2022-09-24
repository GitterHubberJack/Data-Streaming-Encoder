#include "TBGenerator.h"
#include <string>
#include <unordered_map>
//#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

using namespace std;

TBGenerator::TBGenerator(StEncoder* stEncoder, size_t dutClkPeriod, size_t rstTime) {
    _dutName = string("st_encoder");
    _dutClkPeriod = dutClkPeriod;
    _rstTime = rstTime;
    _stEncoder     = stEncoder;
}

void
TBGenerator::generateHeader(stringstream& o) {
    o << "/*******************************************" << endl;
    o << "UVM StEncoder testbench." << endl;
    o << "*******************************************/" << endl;
    o << endl;
    o << "`include \"uvm_macros.svh\"" << endl;
    o << "`include \"testbench_pkg.svh\"" << endl;
    o << "`include \"dut_if.svh\"" << endl;
    o << endl;
    o << "`timescale 1 ns / 1 ps" << endl; 
}

void
TBGenerator::generateInterfaces() {
    stringstream o;
    o << "import st_encoder_pkg::*;" << endl;
    o << endl;
    o << "interface st_intf #(" << endl;
    o << "    parameter type data_t = logic [7:0]" << endl;
    o << ") (" << endl;
    o << ");" << endl;
    o << "logic ready;" << endl;
    o << "    logic valid;" << endl;
    o << "    data_t data;" << endl;
    o << "" << endl;
    o << "    modport master (input  ready ," << endl;
    o << "                    output valid , data   );" << endl;
    o << "" << endl;
    o << "    modport slave  (input  valid , data ," << endl;
    o << "                    output ready          );" << endl;
    o << "" << endl;
    o << "endinterface" << endl;
    o << endl;
    o << "interface st_pkt_intf #(" << endl;
    o << "    parameter WIDTH = 32" << endl;
    o << ") (" << endl;
    o << ");" << endl;
    o << "    logic                       ready ;" << endl;
    o << "    logic                       valid ;" << endl;
    o << "    logic                       sop   ;" << endl;
    o << "    logic                       eop   ;" << endl;
    o << "    logic[WIDTH-1:0]            data  ;" << endl;
    o << "    logic[$clog2(WIDTH/8)-1:0]  len   ;" << endl;
    o << "" << endl;
    o << "    modport master(input  ready ," << endl;
    o << "                   output valid , sop   ," << endl;
    o << "                          eop   , data  ," << endl;
    o << "                          len           );" << endl;
    o << "" << endl;
    o << "    modport slave (input  valid , sop   ," << endl;
    o << "                          eop   , data  ," << endl;
    o << "                          len   ," << endl;
    o << "                   output ready         );" << endl;
    o << "" << endl;
    o << "endinterface" << endl;
    o << "interface dut_if (" << endl;
    o << "    st_intf i_msg_type," << endl;
    o << "    st_pkt_intf o_pkt_intf" << endl;
    o << ");" << endl;
    o << "    logic clk;" << endl;
    o << "    logic rst_n;" << endl;
    o << "    //" << endl;
    // generate st encoder fields
    StructGenerator* structGen = _stEncoder->getStructGen();
    map<string, vector<string> > fieldsMap = structGen->getFieldsMap();

    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        string msgType = it1->first;
        o << "    " << msgType << "_fields  " << msgType << "_i_fields;" << endl;
    }
    o << "endinterface" << endl;
    o << "//" << endl;
    o << "interface internal_if;" << endl;
    if ((_stEncoder->getMaxCounterSize() + _offset) == 1) {
        o << "    reg [0 : 0] counter;" << endl;
    } else {
        o << "    reg [$clog2(" << _stEncoder->getMaxCounterSize() + _offset << ") -1 : 0] counter;" << endl;
    }

    o << "    reg msg_valid;" << endl;
    o << "    msg_type_t msg_type;" << endl;
    o << "endinterface" << endl;
    ofstream ofile;
    string filename = _outDir + "dut_if.svh";
    //
    ofile.open(filename.c_str(), ios::app);
    if (!ofile.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }
    ofile << o.str() << endl;
    ofile.close();

}

void
TBGenerator::instantiateDUT(stringstream& o) {
    o << "// Instantiate the interfaces" << endl;
    o << "st_intf i_msg_type();" << endl;
    o << "st_pkt_intf o_pkt_intf();" << endl;
    o << "internal_if internal_if1();" << endl;
    o << " " << endl;
    o << endl;
    o << "dut_if dut_if1 (" << endl;
    o << "    .i_msg_type(i_msg_type),"  << endl;
    o << "    .o_pkt_intf(o_pkt_intf)" << endl;
    o << ");" << endl;
    o << endl;

    o << _dutName << " " << _dutName << "_inst (" << endl;
    o << "    .rst_n(dut_if1.rst_n)," << endl;
    o << "    .clk(dut_if1.clk)," << endl;
    o << "    .i_msg_type(dut_if1.i_msg_type.slave)," << endl;
    o << "    .o_pkt_intf(dut_if1.o_pkt_intf.master)," << endl;
    
    bool first = true;
    StructGenerator* structGen = _stEncoder->getStructGen();
    map<string, vector<string> > fieldsMap = structGen->getFieldsMap();
    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        string msgType = it1->first;
        if (first) {
            first = false;
        } else {
            o << ","  << endl;
        }
        o << "    ." << msgType << "_i_fields(dut_if1." << msgType << "_i_fields)";
    }
    o << endl;
    o << ");" << endl;
    o << endl;
}

void
TBGenerator::generateClkAndRst(stringstream& o) {
o << "initial begin" << endl;
o << "    dut_if1.clk = 0;" << endl;
o << "    forever #" << _dutClkPeriod/2 << " dut_if1.clk = ~dut_if1.clk;" << endl;
o << "end" << endl;
o << "  " << endl;
o << "initial begin" << endl;
o << "    dut_if1.rst_n = 0;" << endl;
o << "    #" << _rstTime << " dut_if1.rst_n = 1;" << endl;
o << "end" << endl;
}

void 
TBGenerator::generateMakefile() {
    stringstream o;
    //
    o << "VSIM_CMDS?=\"run " << _simTime << " us; quit\"" << endl;
    o << "VSIM_OPTS?=" << endl;
    o << "ELAB_OPTS?=" << endl;
    o << "TOP?= tb" << endl;
    o << "" << endl;
    o << "all: clean compile optimize elaborate simulate" << endl;
    o << "" << endl;
    o << "compile:" << endl;
    o << "	vlog -64 -sv -f files.vc  | tee vlog.log" << endl;
    o << "optimize:" << endl;
    o << "	vopt -64 +acc $(TOP) -designfile $(TOP).bin -o $(TOP)_vopt | tee vopt.log " << endl;
    o << "" << endl;
    o << "elaborate:" << endl;
    o << "	vsim -64 -elab $(TOP).elab -compress_elab $(TOP)_vopt $(ELAB_OPTS)  | tee elab.log" << endl;
    o << "" << endl;
    o << "simulate:" << endl;
    o << "	vsim -64 -c -load_elab $(TOP).elab $(VSIM_OPTS) +notimingchecks -qwavedb=memory+signal+wavefile=$(TOP).db -do $(VSIM_CMDS) | tee vsim.log" << endl;
    o << "" << endl;
    o << "view:" << endl;
    o << "	visualizer -designfile $(TOP).bin -wavefile $(TOP).db" << endl;
    o << endl;
    o << "view_vcd:" << endl;
    o << "	vcd2wlf wave.vcd wave.wlf" << endl;
    o << "	vsim wave.wlf" << endl;
    o <<  endl;
    o << "clean:" << endl;
    o << "	rm -rf work" << endl;
    o << "	rm -f *.log" << endl;
    o << "	rm -f *.jou" << endl;
    o << "	rm -f *.elab" << endl;
    o << "	rm -f *_vopt" << endl;
    o << "	rm -f *.bin" << endl;
    o << "	rm -f *.db" << endl;
    o << "	rm -f *.so" << endl;
    o << "	rm -f *.vstf" << endl;
    o << "	rm -f *.vcd" << endl;
    o << "	rm -f *.wlf" << endl;
    o << "	rm -f modelsim.in" << endl;
    o << "	rm -f transcript" << endl;

    ofstream ofile;
    string filename = _outDir + "Makefile";
    ofile.open(filename.c_str(), ios::app);
    if (!ofile.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }
    ofile << o.str() << endl;
    ofile.close();

    ofstream ofile2;
    stringstream ss;
    ss << "../src/st_encoder.sv" << endl;
    ss << "tb.sv" << endl;
    ss << "+incdir+$(PWD)" << endl;
    filename = _outDir + "files.vc";
    ofile2.open(filename.c_str(), ios::app);
    if (!ofile2.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }
    ofile2 << ss.str() << endl;
    ofile2.close();

}

void
TBGenerator::generateUvmDriver() {
    ofstream ofile2;
    stringstream o;
    string filename = _outDir + "driver.svh";
    ofile2.open(filename.c_str(), ios::app);
    if (!ofile2.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }

    o << "class DRIVER extends uvm_driver #(TRANSACTION);" << endl;
    o << "" << endl;
    o << "  `uvm_component_utils(DRIVER)" << endl;
    o << "" << endl;
    o << "  virtual dut_if dut_vif;" << endl;
    o << "" << endl;
    o << "  function new(string name, uvm_component parent);" << endl;
    o << "    super.new(name, parent);" << endl;
    o << "  endfunction" << endl;
    o << "" << endl;
    o << "  function void build_phase(uvm_phase phase);" << endl;
    o << "    // Get interface reference from config database" << endl;
    o << "    if(!uvm_config_db#(virtual dut_if)::get(this, \"\", \"dut_vif\", dut_vif)) begin" << endl;
    o << "      `uvm_error(\"\", \"uvm_config_db::get failed\")" << endl;
    o << "    end" << endl;
    o << "  endfunction " << endl;
    o << "" << endl;
    o << "  task run_phase(uvm_phase phase);" << endl;
    o << "    // Now drive normal traffic" << endl;
    o << "    forever begin" << endl;
    o << "      seq_item_port.get_next_item(req);" << endl;
    o << "      seq_item_port.item_done();" << endl;
    o << "    end" << endl;
    o << "  endtask" << endl;
    o << "" << endl;
    o << "endclass: DRIVER" << endl;
    ofile2 << o.str() << endl;
    ofile2.close();
}

void
TBGenerator::generateTBPkg() {
    stringstream o;
    o << "package testbench_pkg;" << endl;
    o << "  import uvm_pkg::*;" << endl;
    o << "  " << endl;
    o << "  // The UVM sequence, transaction item, and driver are in these files:" << endl;
    o << "  `include \"sequence.svh\"" << endl;
    o << "  `include \"driver.svh\"" << endl;
    o << "  " << endl;
    o << "  // The agent contains sequencer, driver, and monitor (not included)" << endl;
    o << "  class AGENT extends uvm_agent;" << endl;
    o << "    `uvm_component_utils(AGENT)" << endl;
    o << "    " << endl;
    o << "    DRIVER driver;" << endl;
    o << "    uvm_sequencer#(TRANSACTION) sequencer;" << endl;
    o << "    " << endl;
    o << "    function new(string name, uvm_component parent);" << endl;
    o << "      super.new(name, parent);" << endl;
    o << "    endfunction" << endl;
    o << "    " << endl;
    o << "    function void build_phase(uvm_phase phase);" << endl;
    o << "      driver = DRIVER ::type_id::create(\"driver\", this);" << endl;
    o << "      sequencer =" << endl;
    o << "        uvm_sequencer#(TRANSACTION)::type_id::create(\"sequencer\", this);" << endl;
    o << "    endfunction    " << endl;
    o << "    " << endl;
    o << "    // In UVM connect phase, we connect the sequencer to the driver." << endl;
    o << "    function void connect_phase(uvm_phase phase);" << endl;
    o << "      driver.seq_item_port.connect(sequencer.seq_item_export);" << endl;
    o << "    endfunction" << endl;
    o << "    " << endl;
    o << "    task run_phase(uvm_phase phase);" << endl;
    o << "      // We raise objection to keep the test from completing" << endl;
    o << "      phase.raise_objection(this);" << endl;
    o << "      begin" << endl;
    o << "        SEQUENCE seq;" << endl;
    o << "        seq = SEQUENCE::type_id::create(\"seq\");" << endl;
    o << "        seq.start(sequencer);" << endl;
    o << "      end" << endl;
    o << "      // We drop objection to allow the test to complete" << endl;
    o << "      phase.drop_objection(this);" << endl;
    o << "    endtask" << endl;
    o << "" << endl;
    o << "  endclass" << endl;
    o << "  " << endl;
    o << "  class ENV extends uvm_env;" << endl;
    o << "    `uvm_component_utils(ENV)" << endl;
    o << "    " << endl;
    o << "    AGENT agent;" << endl;
    o << "" << endl;
    o << "    function new(string name, uvm_component parent);" << endl;
    o << "      super.new(name, parent);" << endl;
    o << "    endfunction" << endl;
    o << "    " << endl;
    o << "    function void build_phase(uvm_phase phase);" << endl;
    o << "      agent = AGENT::type_id::create(\"agent\", this);" << endl;
    o << "    endfunction" << endl;
    o << "" << endl;
    o << "  endclass" << endl;
    o << "  " << endl;
    o << "  class TEST extends uvm_test;" << endl;
    o << "    `uvm_component_utils(TEST)" << endl;
    o << "    " << endl;
    o << "    ENV env;" << endl;
    o << "" << endl;
    o << "    function new(string name, uvm_component parent);" << endl;
    o << "      super.new(name, parent);" << endl;
    o << "    endfunction" << endl;
    o << "    " << endl;
    o << "    function void build_phase(uvm_phase phase);" << endl;
    o << "      env = ENV::type_id::create(\"env\", this);" << endl;
    o << "    endfunction" << endl;
    o << "    " << endl;
    o << "    task run_phase(uvm_phase phase);" << endl;
    o << "      // We raise objection to keep the test from completing" << endl;
    o << "      phase.raise_objection(this);" << endl;
    o << "      #10;" << endl;
    o << "      `uvm_info(\"\", \"Start Testing\", UVM_MEDIUM)" << endl;
    o << "      // We drop objection to allow the test to complete" << endl;
    o << "      phase.drop_objection(this);" << endl;
    o << "    endtask" << endl;
    o << "" << endl;
    o << "  endclass" << endl;
    o << "  " << endl;
    o << "endpackage" << endl;
    
    ofstream ofile;
    string filename = _outDir + "testbench_pkg.svh";
    ofile.open(filename.c_str(), ios::app);
    if (!ofile.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }
    ofile << o.str() << endl;
    ofile.close();
}

void
TBGenerator::generateSequence() {
    ofstream ofile;
    stringstream o;
    stringstream s, ss;
    string filename = _outDir + "sequence.svh";
    //
    o << "" << endl;
    o << "import st_encoder_pkg::*;" << endl;
    o << "" << endl;
    o << "class TRANSACTION extends uvm_sequence_item;" << endl;
    o << "" << endl;
    o << "    `uvm_object_utils(TRANSACTION)" << endl;
    o << "    " << endl;
    o << "    virtual dut_if dut_vif;" << endl;
    o << "    virtual internal_if internal_vif;" << endl;
    //
    // generate random busses to drive st encoder fields 
    StructGenerator* structGen = _stEncoder->getStructGen();
    map<string, vector<string> > fieldsMap = structGen->getFieldsMap();
    map<string, pair<size_t, size_t> > valuesMap = structGen->getValuesMap();
    //
    size_t cycle = 0;
    size_t maxCycle = 0;
    string field_r;
    string busName;
    size_t maxNumFields = _stEncoder->getStructGen()->getMaxNumFields();
    size_t maxMsgType = _stEncoder->getStructGen()->getMaxMsgType();
    unordered_map<size_t, vector<pair<string, string> > > mapDr;
    //

    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        string msgType = it1->first;
        vector<string> fields = it1->second;
        for(vector<string>::iterator it2 = fields.begin(); it2 != fields.end(); it2++) {
            string field = *it2;
            size_t size =  valuesMap[msgType + field].first;
            cycle = valuesMap[msgType + field].second;
            //
            o << "    bit [" << size * 8 - 1 << " : 0] " << msgType << "_" << field << ";" << endl;
            s << "        req." << msgType << "_" << field << " = $urandom();" << endl;
            ss << "                req.dut_vif." << msgType << "_i_fields." << field << "_avail <= 0;" << endl;
            //
            //
            field_r = msgType + "_i_fields." + field;
            busName = msgType + "_" + field;
            mapDr[cycle].push_back(make_pair(busName, field_r));
            if(cycle > maxCycle) {
                maxCycle = cycle;
            }
        }
    }
    //
    o << "" << endl;
    o << "  function new (string name = \"\");" << endl;
    o << "    super.new(name);" << endl;
    o << "  endfunction" << endl;
    o << "" << endl;
    o << "endclass: TRANSACTION" << endl;
    o << "" << endl;
    o << "class SEQUENCE extends uvm_sequence#(TRANSACTION);" << endl;
    o << "" << endl;
    o << "  `uvm_object_utils(SEQUENCE)" << endl;
    o << "" << endl;
    o << "  function new (string name = \"\");" << endl;
    o << "    super.new(name);" << endl;
    o << "  endfunction" << endl;
    o << "" << endl;
    o << "  task body;" << endl;
    o << "    ///////////////////////////////////////////////////////" << endl;
    o << "" << endl;
    o << "    forever begin" << endl;
    o << "        req = TRANSACTION::type_id::create(\"req\");" << endl;
    o << "        start_item(req);" << endl;
    o << "        //" << endl;
    o << "        // Get interfaces reference from config database" << endl;
    o << "        if(!uvm_config_db#(virtual dut_if)::get(m_sequencer , \"\", \"dut_vif\", req.dut_vif)) begin" << endl;
    o << "          `uvm_error(\"\", \"uvm_config_db::get failed\")" << endl;
    o << "        end" << endl;
    o << "        if(!uvm_config_db#(virtual internal_if)::get(m_sequencer, \"\", \"internal_vif\", req.internal_vif)) begin" << endl;
    o << "          `uvm_error(\"\", \"uvm_config_db::get failed\")" << endl;
    o << "        end" << endl;
    o << "" << endl;
    //
    o << s.str() << endl;
    o << endl;
    o << "        @(posedge req.dut_vif.clk) begin" << endl;
    o << "            if (~req.dut_vif.rst_n) begin" << endl;
    o << "             `uvm_info(\"DUT\", $sformatf(\"===============================   asserting reset: counter = %d ===================\", req.internal_vif.counter), UVM_MEDIUM)" << endl;
    o << "                req.internal_vif.counter          <= 0;" << endl;
    //
    o << ss.str() << endl;
    o << "                req.internal_vif.msg_valid        <= 0;" << endl;
    o << "                req.internal_vif.msg_type         <= 0;" << endl;
    o << "                req.dut_vif.o_pkt_intf.ready      <= 0 ;" << endl;
    o << "                req.dut_vif.i_msg_type.valid <= 0;" << endl;
    o << "            end else begin" << endl;
    o << "                req.dut_vif.o_pkt_intf.ready      <= 1 ;" << endl;
    o << "             `uvm_info(\"DUT\", $sformatf(\"===============================   releasing reset: counter = %d ===================\", req.internal_vif.counter), UVM_MEDIUM)" << endl;
    o << "                if (req.internal_vif.counter == 1) begin" << endl;
    o << "                    req.dut_vif.i_msg_type.valid <= 1;" << endl;
    o << "                end else begin" << endl;
    o << "                    //req.internal_vif.msg_valid <= 1;" << endl;
    o << "                    req.dut_vif.i_msg_type.valid <= 0;" << endl;
    o << "                end" << endl;
    //
    o << "                if (req.internal_vif.counter == " << _stEncoder->getMaxCounterSize() + _offset << ") begin" << endl;
    o << "                    req.internal_vif.counter <= 0;" << endl;
    o << "                    if (req.internal_vif.msg_type == " << maxMsgType << ") begin" << endl;
    o << "                        req.internal_vif.msg_type <= 0;" << endl;
    o << "                    end else begin" << endl;
    o << "                        req.internal_vif.msg_type <= req.internal_vif.msg_type + 1 ;" << endl;
    o << "                    end" << endl;
    o << "                end else begin" << endl;
    o << "                    req.internal_vif.counter <= req.internal_vif.counter +1;" << endl;
    o << "                end" << endl;
    o << "" << endl;
    o << "        " << endl;
    o << "                //" << endl;

    for (unordered_map<size_t, vector<pair<string, string> > >::iterator itc3 = mapDr.begin(); itc3 != mapDr.end(); itc3++) {
        o << "                if (req.internal_vif.counter == " << itc3->first << ") begin" << endl;
        for(vector<pair<string, string>>::iterator itc4 = itc3->second.begin(); itc4 != itc3->second.end(); itc4++) {
            o << "                    req.dut_vif." << itc4->second << "_avail <= 1;" << endl;
            o << "                    req.dut_vif." << itc4->second << " <= {>> {req." << itc4->first << "}};" << endl;
        }
        o << "                end else begin" << endl;
        for(vector<pair<string, string>>::iterator itc4 = itc3->second.begin(); itc4 != itc3->second.end(); itc4++) {
            o << "                    req.dut_vif." << itc4->second << "_avail <= 0;" << endl;
        }
        o << "                end " << endl;
        o << "                //" << endl;
    }
    o << "            end" << endl;
    o << "        end" << endl;
    o << "        //" << endl;
    o << "        req.dut_vif.i_msg_type.data  =  req.internal_vif.msg_type;" << endl;
    o << "        //req.dut_vif.i_msg_type.valid = req.internal_vif.msg_valid;" << endl;
    o << "        req.dut_vif.o_pkt_intf.ready = 1 ;" << endl;
    o << "        ///////////////////////////////////////////////////////" << endl;
    o << "" << endl;
    o << "        finish_item(req);" << endl;
    o << "    end" << endl;
    o << "  endtask: body" << endl;
    o << "" << endl;
    o << "endclass: SEQUENCE" << endl;

    //
    ofile.open(filename.c_str(), ios::app);
    if (!ofile.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }
    ofile << o.str() << endl;
    ofile.close();
}
void
TBGenerator::generate() {
    stringstream o;
    generateHeader(o);
    //
    o << "module tb;" << endl;
    o << "import uvm_pkg::*;" << endl;
    o << "import testbench_pkg::*;" << endl;
    o << "//" << endl;
    //
    generateInterfaces();
    instantiateDUT(o);
    generateClkAndRst(o);
    //
    //
    ////
    //
    ofstream ofile;
    string filename = _outDir + "tb.sv";
    ofile.open(filename.c_str(), ios::app);
    if (!ofile.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }

    o << "initial begin" << endl;
    o << "  // Place the interface into the UVM configuration database" << endl;
    o << "  uvm_config_db#(virtual dut_if)::set(null, \"*\", \"dut_vif\", dut_if1);" << endl;
    o << "  uvm_config_db#(virtual internal_if)::set(null, \"*\", \"internal_vif\",internal_if1);" << endl;
    o << "  // Start the test" << endl;
    o << "  run_test(\"TEST\");" << endl;
    o << "end" << endl;
    o << "" << endl;
    o << "// Dump waves" << endl;
    o << "initial begin" << endl;
    o << "  `uvm_info(\"DUT\", \"\n===============================   Dumping the vcd waveform ==========================\n\", UVM_LOW);" << endl;
    o << "  $dumpfile(\"wave.vcd\");" << endl;
    o << "  $dumpvars(0, tb);" << endl;
    o << "end" << endl;
    o << "endmodule";
    o << endl;
    ofile << o.str() << endl;
    ofile.close();
    //
    generateMakefile();
    generateUvmDriver();
    generateTBPkg();
    generateSequence();
}

