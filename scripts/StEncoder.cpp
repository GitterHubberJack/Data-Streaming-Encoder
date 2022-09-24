#include "StEncoder.h"

#include <sstream>  
#include <unordered_map>
#include <vector>
#include <set>
#include <boost/algorithm/string.hpp>
#include <fstream>

using namespace std;

StEncoder::StEncoder(string clk, string rst, StructGenerator* structGen) {
    _clk = clk;
    _rst = rst;
    _structGen = structGen;
    _moduleName = "st_encoder";
}

void
StEncoder::createLocalVars(stringstream& o) {
    map<string, vector<string> > fieldsMap;
    // field          size    cycle
    map<string, pair<size_t, size_t > > valuesMap;
    size_t busSize;
    size_t count;
    size_t delayBusSize;
    size_t delay;

    string bus;
    string avail_cycle_dly;
    if(_structGen) {
        fieldsMap = _structGen->getFieldsMap();
        valuesMap = _structGen->getValuesMap();
    }

    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        string msgType = it1->first;
        count =  0;
        busSize =  0;
        for (vector<string>::iterator it2= it1->second.begin(); it2 != it1->second.end(); it2++) {
            string field = *it2;
            size_t field_size = valuesMap[msgType + field].first;
            count =  count + field_size;
            //
            bus = msgType +  "_"  + field + "_avail_dly";
            avail_cycle_dly =  msgType +  "_"  + field + "_avail_dly";
            o << "logic " << avail_cycle_dly << ";" << endl;
            o << "logic " << msgType << "_" << field << "_load ;" << endl;
            o << "logic [" << _frameSize - 1 << " : 0] serial_data_" << msgType << "_" << field << ";" << endl;
            o << "logic [" << ((field_size -1) / 4 + 1) * _frameSize - 1 << " : 0] expanded_data_" << msgType << "_" << field  << ";" << endl;
            o << "logic [" << field_size * 8 - 1 << " : 0] expanded_data_" << msgType << "_" << field  << "_r;" << endl;
        }

        busSize = count * 8; // byte
        o << endl;
        _expandedBus[msgType] =  busSize;
        delayBusSize =(count -1) / 4 +1;
        //
        //
        if (delayBusSize > _maxNumOfStates) {
            _maxNumOfStates = delayBusSize;
        }
    }
}

void
StEncoder::generateHeader(stringstream& o) {
    o << "`include \"pkg_msg.sv\"" << endl;
    o << "`include \"dut_if.svh\"" << endl;
    for (vector<string>::iterator it = _includes.begin(); it != _includes.end(); it++) {
        o << "`include \"" << *it << "\"" << endl;
    }
}

void
StEncoder::generateInterface(stringstream& o) {
    o << "module " << _moduleName << endl;
    o << "       import st_encoder_pkg::*;" << endl;
    o << "(" << endl;
    o << "    input              clk," << endl;
    o << "    input             " << _rst << "," << endl;
    o << "    st_intf.slave      i_msg_type," << endl;
    o << "    st_pkt_intf.master o_pkt_intf," << endl;

    map<string, vector<string> > fieldsMap;
    if(_structGen) {
        fieldsMap = _structGen->getFieldsMap();
    }
    bool first = true;
    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        if (first) {
            first = false;
        } else {
            o << "," << endl;
        }
        o << "    input " << it1->first << "_fields " << it1->first << "_i_fields " ;
    }
    o << endl;
    o << ");" << endl;
    o << endl;
}

void
StEncoder::generateCore(stringstream& o) {
    map<string, vector<string> > fieldsMap;
    if(_structGen) {
        fieldsMap = _structGen->getFieldsMap();
    }

    o << "//" << endl;
    o << "always@(*) begin" << endl;
    o << "    if(~" << _rst << ") begin" << endl;
    o << "        i_msg_type.ready = 0;" << endl;
    o << "    end else if (";
    bool first = true;
    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        if(first) {
            first = false;
        } else {
            o << ") ||";
        }
        o << " (i_msg_type.data == " << it1->first ; 
    }
    o << ") ) begin" << endl;
    o << "        i_msg_type.ready = i_msg_type.valid & ~o_pkt_intf.valid;" << endl;
    o << "        ready =  {" << fieldsMap.size() << "{o_pkt_intf.ready}};" << endl;
    o << "    end else begin" << endl;
    o << "        i_msg_type.ready = 0;" << endl;
    o << "        ready = '0; " << endl;
    o << "    end" << endl;
    o << "end" << endl;
    o << endl;
    o << "/// " << endl;
    // Drive o_pkt_intf interface
    o << endl;
    o << "//Generate FSM states" << endl;
    o << " enum {IDLE";
    for (size_t i = 0; i < _maxNumOfStates; ++i) {
        o << ", STATE_" << i ;
    }
    o << "} state;" << endl;
    o << endl;

    o << "always@(posedge clk ) begin" << endl;
    o << "    if(~" << _rst << ") begin" << endl;
    o << "        state <= IDLE;" << endl;
    o << "        o_pkt_intf.valid <= 0;" << endl;
    o << "        o_pkt_intf.sop   <= 0;" << endl;
    o << "        o_pkt_intf.eop   <= 0;" << endl;
    o << "        o_pkt_intf.len   <= 0;" << endl;
    o << "        o_pkt_intf.data  <= 0;" << endl;
    o << "    end else begin" << endl;
    //
    //
    generateFSM(o);
}

void
StEncoder::generateFSM(stringstream& o) {
    bool first = true;
    map<string, vector<string> > fieldsMap;
    map<string, pair<size_t, size_t > > valuesMap;
    size_t i =0;
    size_t idx =0;

    if(_structGen) {
        fieldsMap = _structGen->getFieldsMap();
        valuesMap = _structGen->getValuesMap();
    }
    //
    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        string msgType = it1->first;
        vector<string> fields = it1->second;
        size_t msgTypeSize = _structGen->getFieldsMap()[msgType].size();
        size_t state = 0;
        size_t expandedBusWidth = _expandedBus[msgType];
        size_t numOfStates = msgTypeSize;
        size_t numZeros = (expandedBusWidth % _frameSize) * 8; 
        {
        if(first) {
            o << "        if (i_msg_type.data == " << msgType << ") begin " << endl;
            first = false;
        } else {
            o << "        end else if (i_msg_type.data == " << msgType << ") begin " << endl;
        }
        o << "            case(state)" << endl;
        o << "                IDLE: " << endl;
        o << "                    begin" << endl;
        o << "                        if (ready_r[" << i << "]) begin" << endl;
        o << "                            state <= STATE_0;" << endl;
        o << "                        end else begin" << endl;
        o << "                            state <= IDLE;" << endl;
        o << "                       end" << endl;
        o << "                       o_pkt_intf.sop      <= 0;" << endl;
        o << "                       o_pkt_intf.valid    <= 0;" << endl;
        o << "                       o_pkt_intf.eop      <= 0;" << endl;
        o << "                       o_pkt_intf.len      <= 0;" << endl;
        o << "                       o_pkt_intf.data     <= 0;" << endl;
        o << "                       counter             <= 0;" << endl;
        o << "                    end" << endl;
        o << "//" << endl;
        }
        //
        size_t upper;
        size_t lower;
        size_t numOfZeros = 0;
        size_t prev_counter_width = 0;
        size_t avail_cycle = 0;
        size_t prev_avail_cycle = 0;
        size_t prev_accum_fram_size = 0;
        size_t accum_size = 0;
        size_t counter_width = 0;
        size_t offset = 0;
        size_t len = 0;
        for (vector<string>::iterator it2 = fields.begin(); it2 != fields.end(); it2++) {
            CycleInfo dlyCyleInfo   =   _delayCycles[idx];
            size_t overlap = dlyCyleInfo.overlap;
            size_t counter_width    =   dlyCyleInfo.delay_cycle_width - overlap;

            string field = *it2;
            size_t size = valuesMap[msgType + field].first;
            size_t frm_size = (size - 1) / 4 + 1;
            //
            upper = (state+1) * _frameSize;
            lower = state * _frameSize;
             //Last state: drive the actual length of the stream data
            if (state == fields.size() -1) {
                len = 4 - (size - overlap) % 4 ;
            }
            string serial_data = "serial_data_" + msgType + "_" + field;
            generateFieldFSM(o, state, serial_data,  numOfStates, counter_width,  msgType, field, len, offset, i);
            ////
            offset = offset + frm_size * _frameSize; 
            state++;
            idx++;
        }
        {
        o << "                default:" << endl;
        o << "                    begin" << endl;
        o << "                        state               <= IDLE;" << endl;
        o << "                        o_pkt_intf.sop      <= 0;" << endl;
        o << "                        o_pkt_intf.valid    <= 0;" << endl;
        o << "                        o_pkt_intf.eop      <= 1;" << endl;
        o << "                        o_pkt_intf.len      <= 0;" << endl;
        o << "                        o_pkt_intf.data     <= 0;" << endl;
        o << "                        counter             <= 0;" << endl;
        o << "                    end" << endl;
        o << "            endcase" << endl;
        //
        }
    }
    {
    //
    o << "        end else  begin " << endl;
    o << "            state               <= IDLE;" << endl;
    o << "            o_pkt_intf.sop      <= 0;" << endl;
    o << "            o_pkt_intf.valid    <= 0;" << endl;
    o << "            o_pkt_intf.eop      <= 0;" << endl;
    o << "            o_pkt_intf.len      <= 0;" << endl;
    o << "            o_pkt_intf.data     <= 0;" << endl;
    o << "            counter             <= 0;" << endl;
    o << "        end" << endl;
    o << "    end" << endl;
    o << "end" << endl;
    //
    }
    i++;
}

void
StEncoder::generateFieldFSM(stringstream& o, size_t state, std::string& serial_data, size_t numOfStates, size_t counter_width, string& msgType, string field,
    size_t len, size_t offset, size_t readyIdx) {
    //
    o << "                STATE_" << state << ": " << endl;
    o << "                    begin" << endl;
    if (counter_width == 0) {
        if (state == (numOfStates - 1) ) {
            o << "                        state               <= IDLE;" << endl;
            o << "                        o_pkt_intf.sop      <= 0;" << endl;
            o << "                        o_pkt_intf.valid    <= 0;" << endl;
            o << "                        o_pkt_intf.eop      <= 1;" << endl;
            o << "                        o_pkt_intf.len      <= 0;" << endl;
            o << "                        o_pkt_intf.data     <= 0;" << endl;
            o << "                        counter             <= 0;" << endl;
        } else {
            o << "                        assert (state != STATE_0) else $error(\"There must be data transfer in STATE_0\");" << endl;
            o << "                        state               <= STATE_" << state + 1 << ";" << endl;
            o << "                        o_pkt_intf.sop      <= 0;" << endl;
            o << "                        o_pkt_intf.valid    <= 1;" << endl;
            o << "                        o_pkt_intf.eop      <= 0;" << endl;
            o << "                        o_pkt_intf.len      <= 0;" << endl;
            o << "                        o_pkt_intf.data     <= 0;" << endl;
            o << "                        counter             <= 0;" << endl;
        }

    } else {
        o << "                        if (ready_r[" << readyIdx << "]) begin" << endl;
        o << "                            if (" << msgType << "_" << field << "_avail_dly" << ") begin" << endl;
        //
        o << "                                if (counter ==" << counter_width -1 << ") begin" << endl;
        o << "                                    counter <= 0;" << endl;
        if (state == (numOfStates - 1) ) {
            o << "                                    state               <= IDLE;" << endl;
        } else {
            o << "                                    state               <= STATE_" << state + 1 << ";" << endl;
        }
        o << "                                end else begin " << endl;
        o << "                                    counter <= counter +1;" << endl;
        o << "                                    state               <= STATE_" << state << ";" << endl;
        o << "                                end" << endl;
        if (state == (numOfStates - 1) ) {
            o << "                                o_pkt_intf.eop       <= 1;" << endl;
        } else {
            o << "                                o_pkt_intf.eop      <= 0;" << endl;
        }
        o << "                                o_pkt_intf.data     <= " << serial_data <<  ";" << endl;
        o << "                                o_pkt_intf.len      <= " << len << ";" << endl;
        o << endl;
        //
        if (state == 0) {
            o << "                                if (counter == 0) begin" << endl;
            o << "                                    o_pkt_intf.sop      <= 1;" << endl;
            o << "                                end else begin" << endl;
            o << "                                        o_pkt_intf.sop      <= 0;" << endl;
            o << "                                end" << endl;
        } else {
            o << "                                o_pkt_intf.sop      <= 0;" << endl;
        }
        o << "                                o_pkt_intf.valid    <= 1;" << endl;
        o << "                            end else begin" << endl;
        o << "                                state               <= STATE_" << state << ";" << endl;
        o << "                                o_pkt_intf.sop      <= 0;" << endl;
        o << "                                o_pkt_intf.eop      <= 0;" << endl;
        if (state == 0) {
            o << "                                o_pkt_intf.valid    <= 0;" << endl;
        } else {
            o << "                                o_pkt_intf.valid    <= 1;" << endl;
        }
        o << "                                o_pkt_intf.len      <= 0;" << endl;
        o << "                                o_pkt_intf.data     <= 0;" << endl;
        o << "                                counter             <= 0;" << endl;
        o << "                            end" << endl;
        o << "                        end else begin" << endl;
        {
        o << "                            state               <= IDLE;" << endl;
        o << "                            o_pkt_intf.sop      <= 0;" << endl;
        o << "                            o_pkt_intf.valid    <= 0;" << endl;
        o << "                            o_pkt_intf.eop      <= 0;" << endl;
        o << "                            o_pkt_intf.len      <= 0;" << endl;
        o << "                            o_pkt_intf.data     <= 0;" << endl;
        o << "                            counter             <= 0;" << endl;
        }
        o << "                        end" << endl;
    }
    o << "                    end" << endl;
    o << endl;
}
/*
In case some fiedls have similar available cycle, we need to apply pipelining delay to send them sequentienlly
*/
void
StEncoder::generatePipelines(stringstream& o) {
    map<string, vector<string> > fieldsMap;
    map<string, pair<size_t, size_t > > valuesMap;
    size_t idx = 0;
    if(_structGen) {
        fieldsMap = _structGen->getFieldsMap();
        valuesMap = _structGen->getValuesMap();
    }
    //
    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        string msgType = it1->first;
        vector<string> fields = it1->second;
        //
        size_t index = 1;
        size_t prev_delay_cycle = 0;
        bool first1 = true;
        size_t delay_cycle = 0;    
        size_t prev_l_frame_width = 0;
        size_t frame_width;
        size_t delay_cycle_width = 0;
        bool first = true;
        size_t count = 0;
        size_t r_bytes = 0;
        size_t overlap;
        size_t prev_size = 0;
        for (vector<string>::iterator it2 = fields.begin(); it2 != fields.end(); it2++) {
            string field = *it2;
            size_t size = valuesMap[msgType + field].first;
            size_t cycle = valuesMap[msgType + field].second;
            r_bytes = count % 4;
            frame_width = (size + r_bytes -1) / 4 + 1;

            if (cycle > index) {
                delay_cycle = cycle;
            } else {
                delay_cycle = index;
            }
            if (delay_cycle < (prev_delay_cycle + prev_l_frame_width) ) {
                delay_cycle  = prev_delay_cycle + prev_l_frame_width;
            }
            //

            if (delay_cycle > (prev_delay_cycle + prev_l_frame_width)) {
                overlap = 0;
                count = size;
                delay_cycle_width = (size -1) / 4 + 1;
            } else {
                count = count + size;
                delay_cycle_width = frame_width;
                overlap = prev_delay_cycle + ((prev_size - 1) / 4 + 1) - delay_cycle;
            }

            if(first1) {
                _readyDelay[msgType] = cycle -1;
                first1 = false;
            }
            CycleInfo dlyCycleInfo;
            dlyCycleInfo.avail_cycle = cycle;
            dlyCycleInfo.delay_cycle = delay_cycle;
            dlyCycleInfo.delay_cycle_width = delay_cycle_width;
            dlyCycleInfo.overlap = overlap;
            //
            _delayCycles[idx] = dlyCycleInfo; 
            prev_delay_cycle = delay_cycle;
            prev_l_frame_width = size / 4; //9 bytes ==> 2 l_frames
            index++;
            idx++;
            prev_size = size;
        }
    }
    //
    //
    o << "logic [" <<fieldsMap.size() - 1 << " : 0] ready;" <<endl; 
    o << "logic [" <<fieldsMap.size() - 1 << " : 0] ready_r;" <<endl;
    size_t i = 0;
    idx = 0;
    size_t max_counter_width = 0;
    size_t cnt = 0;
    size_t size;
    for (map<string, vector<string> >::iterator it1 = fieldsMap.begin(); it1 != fieldsMap.end(); it1++) {
        string msgType = it1->first;
        vector<string> fields = it1->second;
        //
        

        o << "//" << endl;
        o << "PipeLine #(" << endl;
        o << "    .NUM_SYNC_STAGES(" << _readyDelay[msgType] << ")" << endl;
        o << " ) ready_" << i << "_dly_inst (" << endl;
        o << "    .clock(" << _clk << ")," << endl;
        o << "    .in(ready[" << i << "])," << endl;
        o << "    .out0(ready_r[" << i << "])" << endl;
        o << ");" << endl;
        o << "//" << endl;
        i++; 
        //
        size_t index = 0;
        size_t lower_bound = 0;
        size_t upper_bound = 0;
        bool first = true;
        size_t first_delay_cycle;
        stringstream ss;
        string expanded_data_prev;
        size_t size_prev = 0;
        size_t overlap_bytes = 0;
        size_t overlap_bytes_prev = 0;
        bool managed = false;
        for (vector<string>::iterator it2 = fields.begin(); it2 != fields.end(); it2++) {
            //
            string field = *it2;
            size = valuesMap[msgType + field].first;
            CycleInfo dlyCyleInfo   =   _delayCycles[idx];
            size_t overlap          =   dlyCyleInfo.overlap;
            size_t delay_cycle      =   dlyCyleInfo.delay_cycle;
            size_t syncStages       =   delay_cycle - dlyCyleInfo.avail_cycle;
            size_t width            =   dlyCyleInfo.delay_cycle_width; 
            if (width > max_counter_width)
                max_counter_width = width;
            if (_maxCounterSize < (delay_cycle + width)) {
                _maxCounterSize = delay_cycle + width;
            }

            stringstream inSig;
            stringstream outSig0;
            stringstream load;

            stringstream instName;
            inSig << msgType << "_i_fields." << field << "_avail" ;
            instName << msgType  << "_" << field << "_pipeline_inst_" << index ;
            outSig0 << msgType << "_" << field << "_avail_dly" ;
            load << msgType << "_" << field << "_load" ;

            //Insert pipeleine
            addPipeline(o, syncStages, width, inSig.str(), outSig0.str(), load.str(), instName.str());
            //

            o << endl;
            size_t P_WIDTH = ((size -1) / 4 + 1) * 32;
            size_t SYNC_STAGES = 0;
            if (overlap != 0) {
                SYNC_STAGES = 1;
            }
            string paralle_in = "expanded_data_" + msgType + "_" + field ;
            string serial_out = "serial_data_" + msgType + "_" + field ;
            string instName_piso =  msgType + "_" + field + "_piso_inst";
            
            //Instantiate Parallel to serial shift register
            addPiSoInstance(o, P_WIDTH, SYNC_STAGES, load.str(), paralle_in,  serial_out, instName_piso);
            //

            // drive expanded_data bus
            size_t num_zeros = P_WIDTH - size * 8;
            string expanded_data =" expanded_data_" + msgType + "_" + field; 
            string expanded_data_r =" expanded_data_" + msgType + "_" + field + "_r"; 
            string orig_data = msgType + "_i_fields." + field ; 
 
            o << "assign " << expanded_data_r << " = {>>{" << orig_data << "}} ;" << endl;
            if(first) {
                ss << "assign " << expanded_data << "[" << size * 8 -1 << ": 0] = " << expanded_data_r << ";" << endl;
                first = false;
            } else {
                if (overlap > 0) {
                    overlap_bytes = 4 - (size_prev % 4);
                    if (size > overlap_bytes) {
                        if (!managed) {
                            ss << "assign " << expanded_data_prev << "[" << (size_prev + overlap_bytes) * 8 - 1 << ": " << size_prev * 8 << "] = " << expanded_data_r << "[" << overlap_bytes * 8 -1 << " : 0];" << endl;
                        } else {
                            managed = false;
                        }
                        ss << "assign " << expanded_data << "[" << (size - overlap_bytes) * 8 -1 << " : 0] = " << expanded_data_r << "[" << size * 8 -1 << " : " << overlap_bytes * 8 << "];" << endl;
                        if (index == fields.size() -1) {
                            size_t last_upper_bound = ((size -1) / 4 + 1) * _frameSize;
                            ss << "assign " << expanded_data << "[" << last_upper_bound - 1 << " : " << (size - overlap_bytes) * 8 << "] = '0;" << endl; 
                        }
                    } else {
                        if (!managed) {
                            ss << "assign " << expanded_data_prev << "[" << (size_prev + size) * 8 - 1 << ": " << size_prev * 8 << "] = " << expanded_data_r << ";" << endl;
                        } else {
                            managed = false;
                        }

                        if (overlap_bytes != size) {
                            if (!managed) {
                                ss << "assign " << expanded_data_prev << "[" << (size_prev + overlap_bytes ) * 8 - 1 << ": " << (size_prev + size) * 8 << "] = '0;" << endl;
                            } else {
                                managed = false;
                            }
                        }
                        ss << "assign " << expanded_data << " = '0 ;" << endl;
                        managed = true;
                    }

                } else {
                    size_t last_upper_bound = ((size_prev -1) / 4 + 1) * _frameSize;
                    if (!managed) {
                        ss << "assign " << expanded_data_prev << "[" << last_upper_bound -1  << " : " << (size_prev - overlap_bytes) * 8 << "] = '0;" << endl;
                    } else {
                        managed = false;
                    }
                    ss << "assign " << expanded_data << "[" << size * 8 -1 << ": 0] = " << expanded_data_r << ";" << endl;
                    overlap_bytes = 0;
                }
            }
            expanded_data_prev = expanded_data;
            size_prev = size;
            overlap_bytes_prev = overlap_bytes;
            index++;
            idx++;
        }   
        o << ss.str() << endl;
        cnt = cnt + fields.size();
    }
    o << "//" << endl;
    o << "////" << endl;
    o << "//" << endl;
    if (max_counter_width <= 2) {
        o << "[" << max_counter_width << " : 0] counter" << endl;
    } else {
        o << "logic [$clog2(" << max_counter_width << ") - 1 : 0] counter;" << endl;
    }

}

void 
StEncoder::addPipeline(stringstream& o, size_t syncStages, size_t width, string inSig, string outSig0, string outSig1, string instName) {
    o << "PipeLine #(" << endl;
    o << "    .NUM_SYNC_STAGES(" << syncStages << ")," << endl;
    o << "    .WIDTH(" << width << ")" << endl;
    o << " ) " << instName << " (" << endl;
    o << "    .clock(" << _clk << ")," << endl;
    o << "    .in(" << inSig << ")," << endl;
    o << "    .out0(" << outSig0 << ")," << endl;
    o << "    .out1(" << outSig1 << ")" << endl;
    o << ");" << endl;
}

void 
StEncoder::addPiSoInstance(stringstream& o, size_t P_WIDTH, size_t SYNC_STAGES, string load, string paralle_in, string serial_out, string instName) {
    o << "PISO_SR # (" << endl;
    o << "    .P_WIDTH(" << P_WIDTH << ")," << endl;
    o << "    .S_WIDTH(" << _frameSize << ")," << endl;
    o << "    .NUM_SYNC_STAGES(" << SYNC_STAGES << ")" << endl;
    o << ") " << instName << " (" << endl;
    o << "    .clk(clk)," << endl;
    o << "    .load(" << load << ")," << endl;
    o << "    .Parallel_In(" <<paralle_in << ")," << endl;
    o << "    .Serial_Out(" << serial_out << ")" << endl;
    o << ");" << endl;
}

void 
StEncoder::generate() {
    stringstream o;
    stringstream ss;
    ofstream ofile;
    string filename = _outDir + _moduleName + ".sv";
    //
    createLocalVars(ss);
    generateHeader(o);
    generateInterface(o);
    //
    o << ss.str() << endl;
    //
    generatePipelines(o);
    generateCore(o);
    //
    ofile.open(filename.c_str(), ios::app);
    if (!ofile.is_open()) {
        cout << "Could not open file <" << filename << "> . Please verify that you have the permission to write in it." << endl;
    }

    //
    ////
    //
    o << endl;
    o << "endmodule" << endl;
    o << endl;
    o << "///////////////////////////////////" << endl;
    o << endl;
    o << "module PipeLine (" << endl;
    o << "     clock," << endl;
    o << "     in," << endl;
    o << "     out0," << endl;
    o << "     out1" << endl;
    o << " );" << endl;
    o << " " << endl;
    o << " parameter   NUM_SYNC_STAGES             = 2;" << endl;
    o << " parameter   WIDTH                       = 1;" << endl;
    o << " " << endl;
    o << " input                                   clock;" << endl;
    o << " input                                   in;" << endl;
    o << " output   logic                          out0;" << endl;
    o << " output                                  out1;" << endl;
    o << " " << endl;
    o << " logic [NUM_SYNC_STAGES-1:0] synchronizer;" << endl;
    o << " integer i;" << endl;
    o << " logic out_r;" << endl;
    o << " logic out_r1;" << endl;
    o << " generate " << endl;
    o << "     if (NUM_SYNC_STAGES == 0) begin" << endl;
    o << "         assign out_r = in;" << endl;
    o << "     end else begin: data_synchronization " << endl;
    o << "         always @(posedge clock) begin" << endl;
    o << "             synchronizer[0] <= in;" << endl;
    o << "             for (i = 1; i < NUM_SYNC_STAGES; i = i+1) begin" << endl;
    o << "                 synchronizer[i] <= synchronizer[i-1];" << endl;
    o << "             end" << endl;
    o << "         end" << endl;
    o << "         assign out_r = synchronizer[NUM_SYNC_STAGES-1];" << endl;
    o << "     end" << endl;
    o << " " << endl;
    o << "     if (WIDTH == 1) begin" << endl;
    o << "         always@(posedge clock) begin" << endl;
    o << "             out_r1 <= out_r;" << endl;
    o << "             out0 <= out_r1;" << endl;
    o << "         end" << endl;
    o << "     end else begin" << endl;
    o << "         logic [WIDTH-2:0] counter;" << endl;
    o << "     " << endl;
    o << "         for (genvar i = 0; i < (WIDTH-2); i = i+1) begin " << endl;
    o << "             always@(posedge clock) begin" << endl;
    o << "                 counter[i+1] <= counter[i];" << endl;
    o << "             end " << endl;
    o << "         end   " << endl;
    o << "         always@(posedge clock) begin" << endl;
    o << "             counter[0] <= out_r;" << endl;
    o << "         end" << endl;
    o << " " << endl;
    o << "         always@(posedge clock) begin" << endl;
    o << "             out_r1 <= out_r | (|counter);" << endl;
    o << "             out0 <= out_r1;" << endl;
    o << "         end" << endl;
    o << " " << endl;
    o << "     end" << endl;
    o << "     assign out1 = out_r;" << endl;
    o << " endgenerate" << endl;
    o << " " << endl;
    o << " " << endl;
    o << " endmodule" << endl;

    o << "//" << endl;
    o << "////" << endl;
    o << "//" << endl;
    o << "module PISO_SR # (" << endl;
    o << "    parameter P_WIDTH = 4," << endl;
    o << "    parameter S_WIDTH = 1," << endl;
    o << "    parameter  NUM_SYNC_STAGES = 0" << endl;
    o << ") (" << endl;
    o << "    input clk," << endl;
    o << "    input load," << endl;
    o << "    input [P_WIDTH-1:0]Parallel_In," << endl;
    o << "    output logic [S_WIDTH-1: 0] Serial_Out" << endl;
    o << ");" << endl;
    o << "    logic [S_WIDTH-1: 0] serial_out_r;" << endl;
    o << "    logic [P_WIDTH-1 :0] tmp;" << endl;
    o << "    logic [S_WIDTH*NUM_SYNC_STAGES-1:0] synchronizer;" << endl;
    o << "    integer i;" << endl;
    o << "    always @(posedge clk) begin" << endl;
    o << "        if(load) begin" << endl;
    o << "            tmp <= Parallel_In;" << endl;
    o << "        end else begin" << endl;
    o << "            tmp <= tmp >> S_WIDTH;" << endl;
    o << "            serial_out_r <= tmp[S_WIDTH-1 : 0];" << endl;
    o << "        end" << endl;
    o << "    end" << endl;
    o << "    generate" << endl;
    o << "        if(NUM_SYNC_STAGES == 0) begin" << endl;
    o << "            assign Serial_Out = serial_out_r;" << endl;
    o << "        end else begin" << endl;
    o << "            always @(posedge clk) begin" << endl;
    o << "                synchronizer[0 +: S_WIDTH] <= serial_out_r;" << endl;
    o << "                for (i = 1; i < NUM_SYNC_STAGES; i = i+1) begin" << endl;
    o << "                    synchronizer[S_WIDTH*i +: S_WIDTH] <= synchronizer[S_WIDTH*(i-1) +: S_WIDTH];" << endl;
    o << "                end" << endl;
    o << "            end" << endl;
    o << "            assign Serial_Out = synchronizer[S_WIDTH*(NUM_SYNC_STAGES-1) +: S_WIDTH];" << endl;
    o << "        end" << endl;
    o << "    endgenerate" << endl;
    o << "" << endl;
    o << "endmodule" << endl;
    ofile << o.str() << endl;
    ofile.close();
}
