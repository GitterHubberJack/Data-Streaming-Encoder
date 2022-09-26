`include "pkg_msg.sv"
`include "dut_if.svh"
module st_encoder
       import st_encoder_pkg::*;
(
    input              clk,
    input             rst_n,
    st_intf.slave      i_msg_type,
    st_pkt_intf.master o_pkt_intf,
    input MSG3_fields MSG3_i_fields ,
    input MSG4_fields MSG4_i_fields 
);

logic MSG3_field0_avail_dly;
logic MSG3_field0_load ;
logic [31 : 0] serial_data_MSG3_field0;
logic [95 : 0] expanded_data_MSG3_field0;
logic [71 : 0] expanded_data_MSG3_field0_r;
logic MSG3_field1_avail_dly;
logic MSG3_field1_load ;
logic [31 : 0] serial_data_MSG3_field1;
logic [31 : 0] expanded_data_MSG3_field1;
logic [23 : 0] expanded_data_MSG3_field1_r;
logic MSG3_Z_avail_dly;
logic MSG3_Z_load ;
logic [31 : 0] serial_data_MSG3_Z;
logic [63 : 0] expanded_data_MSG3_Z;
logic [47 : 0] expanded_data_MSG3_Z_r;
logic MSG3_field2_avail_dly;
logic MSG3_field2_load ;
logic [31 : 0] serial_data_MSG3_field2;
logic [63 : 0] expanded_data_MSG3_field2;
logic [39 : 0] expanded_data_MSG3_field2_r;
logic MSG3_field3_avail_dly;
logic MSG3_field3_load ;
logic [31 : 0] serial_data_MSG3_field3;
logic [31 : 0] expanded_data_MSG3_field3;
logic [31 : 0] expanded_data_MSG3_field3_r;

logic MSG4_field0_avail_dly;
logic MSG4_field0_load ;
logic [31 : 0] serial_data_MSG4_field0;
logic [95 : 0] expanded_data_MSG4_field0;
logic [71 : 0] expanded_data_MSG4_field0_r;
logic MSG4_field1_avail_dly;
logic MSG4_field1_load ;
logic [31 : 0] serial_data_MSG4_field1;
logic [31 : 0] expanded_data_MSG4_field1;
logic [23 : 0] expanded_data_MSG4_field1_r;
logic MSG4_Z_avail_dly;
logic MSG4_Z_load ;
logic [31 : 0] serial_data_MSG4_Z;
logic [63 : 0] expanded_data_MSG4_Z;
logic [47 : 0] expanded_data_MSG4_Z_r;
logic MSG4_field2_avail_dly;
logic MSG4_field2_load ;
logic [31 : 0] serial_data_MSG4_field2;
logic [63 : 0] expanded_data_MSG4_field2;
logic [39 : 0] expanded_data_MSG4_field2_r;
logic MSG4_field3_avail_dly;
logic MSG4_field3_load ;
logic [31 : 0] serial_data_MSG4_field3;
logic [31 : 0] expanded_data_MSG4_field3;
logic [23 : 0] expanded_data_MSG4_field3_r;


logic [1 : 0] ready;
logic [1 : 0] ready_r;
//
PipeLine #(
    .NUM_SYNC_STAGES(2)
 ) ready_0_dly_inst (
    .clock(clk),
    .in(ready[0]),
    .out0(ready_r[0])
);
//
PipeLine #(
    .NUM_SYNC_STAGES(0),
    .WIDTH(3)
 ) MSG3_field0_pipeline_inst_0 (
    .clock(clk),
    .in(MSG3_i_fields.field0_avail),
    .out0(MSG3_field0_avail_dly),
    .out1(MSG3_field0_load)
);

PISO_SR # (
    .P_WIDTH(96),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(0)
) MSG3_field0_piso_inst (
    .clk(clk),
    .load(MSG3_field0_load),
    .Parallel_In(expanded_data_MSG3_field0),
    .Serial_Out(serial_data_MSG3_field0)
);
assign  expanded_data_MSG3_field0_r = {>>{MSG3_i_fields.field0}} ;
PipeLine #(
    .NUM_SYNC_STAGES(3),
    .WIDTH(1)
 ) MSG3_field1_pipeline_inst_1 (
    .clock(clk),
    .in(MSG3_i_fields.field1_avail),
    .out0(MSG3_field1_avail_dly),
    .out1(MSG3_field1_load)
);

PISO_SR # (
    .P_WIDTH(32),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(1)
) MSG3_field1_piso_inst (
    .clk(clk),
    .load(MSG3_field1_load),
    .Parallel_In(expanded_data_MSG3_field1),
    .Serial_Out(serial_data_MSG3_field1)
);
assign  expanded_data_MSG3_field1_r = {>>{MSG3_i_fields.field1}} ;
PipeLine #(
    .NUM_SYNC_STAGES(0),
    .WIDTH(2)
 ) MSG3_Z_pipeline_inst_2 (
    .clock(clk),
    .in(MSG3_i_fields.Z_avail),
    .out0(MSG3_Z_avail_dly),
    .out1(MSG3_Z_load)
);

PISO_SR # (
    .P_WIDTH(64),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(0)
) MSG3_Z_piso_inst (
    .clk(clk),
    .load(MSG3_Z_load),
    .Parallel_In(expanded_data_MSG3_Z),
    .Serial_Out(serial_data_MSG3_Z)
);
assign  expanded_data_MSG3_Z_r = {>>{MSG3_i_fields.Z}} ;
PipeLine #(
    .NUM_SYNC_STAGES(0),
    .WIDTH(2)
 ) MSG3_field2_pipeline_inst_3 (
    .clock(clk),
    .in(MSG3_i_fields.field2_avail),
    .out0(MSG3_field2_avail_dly),
    .out1(MSG3_field2_load)
);

PISO_SR # (
    .P_WIDTH(64),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(0)
) MSG3_field2_piso_inst (
    .clk(clk),
    .load(MSG3_field2_load),
    .Parallel_In(expanded_data_MSG3_field2),
    .Serial_Out(serial_data_MSG3_field2)
);
assign  expanded_data_MSG3_field2_r = {>>{MSG3_i_fields.field2}} ;
PipeLine #(
    .NUM_SYNC_STAGES(3),
    .WIDTH(2)
 ) MSG3_field3_pipeline_inst_4 (
    .clock(clk),
    .in(MSG3_i_fields.field3_avail),
    .out0(MSG3_field3_avail_dly),
    .out1(MSG3_field3_load)
);

PISO_SR # (
    .P_WIDTH(32),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(1)
) MSG3_field3_piso_inst (
    .clk(clk),
    .load(MSG3_field3_load),
    .Parallel_In(expanded_data_MSG3_field3),
    .Serial_Out(serial_data_MSG3_field3)
);
assign  expanded_data_MSG3_field3_r = {>>{MSG3_i_fields.field3}} ;
assign  expanded_data_MSG3_field0[71: 0] =  expanded_data_MSG3_field0_r;
assign  expanded_data_MSG3_field0[95: 72] =  expanded_data_MSG3_field1_r;
assign  expanded_data_MSG3_field1 = '0 ;
assign  expanded_data_MSG3_Z[47: 0] =  expanded_data_MSG3_Z_r;
assign  expanded_data_MSG3_Z[63 : 48] = '0;
assign  expanded_data_MSG3_field2[39: 0] =  expanded_data_MSG3_field2_r;
assign  expanded_data_MSG3_field2[63: 40] =  expanded_data_MSG3_field3_r[23 : 0];
assign  expanded_data_MSG3_field3[7 : 0] =  expanded_data_MSG3_field3_r[31 : 24];
assign  expanded_data_MSG3_field3[31 : 8] = '0;

//
PipeLine #(
    .NUM_SYNC_STAGES(2)
 ) ready_1_dly_inst (
    .clock(clk),
    .in(ready[1]),
    .out0(ready_r[1])
);
//
PipeLine #(
    .NUM_SYNC_STAGES(0),
    .WIDTH(3)
 ) MSG4_field0_pipeline_inst_0 (
    .clock(clk),
    .in(MSG4_i_fields.field0_avail),
    .out0(MSG4_field0_avail_dly),
    .out1(MSG4_field0_load)
);

PISO_SR # (
    .P_WIDTH(96),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(0)
) MSG4_field0_piso_inst (
    .clk(clk),
    .load(MSG4_field0_load),
    .Parallel_In(expanded_data_MSG4_field0),
    .Serial_Out(serial_data_MSG4_field0)
);
assign  expanded_data_MSG4_field0_r = {>>{MSG4_i_fields.field0}} ;
PipeLine #(
    .NUM_SYNC_STAGES(3),
    .WIDTH(1)
 ) MSG4_field1_pipeline_inst_1 (
    .clock(clk),
    .in(MSG4_i_fields.field1_avail),
    .out0(MSG4_field1_avail_dly),
    .out1(MSG4_field1_load)
);

PISO_SR # (
    .P_WIDTH(32),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(1)
) MSG4_field1_piso_inst (
    .clk(clk),
    .load(MSG4_field1_load),
    .Parallel_In(expanded_data_MSG4_field1),
    .Serial_Out(serial_data_MSG4_field1)
);
assign  expanded_data_MSG4_field1_r = {>>{MSG4_i_fields.field1}} ;
PipeLine #(
    .NUM_SYNC_STAGES(0),
    .WIDTH(2)
 ) MSG4_Z_pipeline_inst_2 (
    .clock(clk),
    .in(MSG4_i_fields.Z_avail),
    .out0(MSG4_Z_avail_dly),
    .out1(MSG4_Z_load)
);

PISO_SR # (
    .P_WIDTH(64),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(0)
) MSG4_Z_piso_inst (
    .clk(clk),
    .load(MSG4_Z_load),
    .Parallel_In(expanded_data_MSG4_Z),
    .Serial_Out(serial_data_MSG4_Z)
);
assign  expanded_data_MSG4_Z_r = {>>{MSG4_i_fields.Z}} ;
PipeLine #(
    .NUM_SYNC_STAGES(0),
    .WIDTH(2)
 ) MSG4_field2_pipeline_inst_3 (
    .clock(clk),
    .in(MSG4_i_fields.field2_avail),
    .out0(MSG4_field2_avail_dly),
    .out1(MSG4_field2_load)
);

PISO_SR # (
    .P_WIDTH(64),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(0)
) MSG4_field2_piso_inst (
    .clk(clk),
    .load(MSG4_field2_load),
    .Parallel_In(expanded_data_MSG4_field2),
    .Serial_Out(serial_data_MSG4_field2)
);
assign  expanded_data_MSG4_field2_r = {>>{MSG4_i_fields.field2}} ;
PipeLine #(
    .NUM_SYNC_STAGES(3),
    .WIDTH(1)
 ) MSG4_field3_pipeline_inst_4 (
    .clock(clk),
    .in(MSG4_i_fields.field3_avail),
    .out0(MSG4_field3_avail_dly),
    .out1(MSG4_field3_load)
);

PISO_SR # (
    .P_WIDTH(32),
    .S_WIDTH(32),
    .NUM_SYNC_STAGES(1)
) MSG4_field3_piso_inst (
    .clk(clk),
    .load(MSG4_field3_load),
    .Parallel_In(expanded_data_MSG4_field3),
    .Serial_Out(serial_data_MSG4_field3)
);
assign  expanded_data_MSG4_field3_r = {>>{MSG4_i_fields.field3}} ;
assign  expanded_data_MSG4_field0[71: 0] =  expanded_data_MSG4_field0_r;
assign  expanded_data_MSG4_field0[95: 72] =  expanded_data_MSG4_field1_r;
assign  expanded_data_MSG4_field1 = '0 ;
assign  expanded_data_MSG4_Z[47: 0] =  expanded_data_MSG4_Z_r;
assign  expanded_data_MSG4_Z[63 : 48] = '0;
assign  expanded_data_MSG4_field2[39: 0] =  expanded_data_MSG4_field2_r;
assign  expanded_data_MSG4_field2[63: 40] =  expanded_data_MSG4_field3_r;
assign  expanded_data_MSG4_field3 = '0 ;

//
////
//
logic [$clog2(3) - 1 : 0] counter;
//
always@(*) begin
    if(~rst_n) begin
        i_msg_type.ready = 0;
    end else if ( (i_msg_type.data == MSG3) || (i_msg_type.data == MSG4) ) begin
        i_msg_type.ready = i_msg_type.valid & ~o_pkt_intf.valid;
        ready =  {2{o_pkt_intf.ready}};
    end else begin
        i_msg_type.ready = 0;
        ready = '0; 
    end
end

/// 

//Generate FSM states
 enum {IDLE, STATE_0, STATE_1, STATE_2, STATE_3, STATE_4, STATE_5, STATE_6} state;

always@(posedge clk ) begin
    if(~rst_n) begin
        state <= IDLE;
        o_pkt_intf.valid <= 0;
        o_pkt_intf.sop   <= 0;
        o_pkt_intf.eop   <= 0;
        o_pkt_intf.len   <= 0;
        o_pkt_intf.data  <= 0;
    end else begin
        if (i_msg_type.data == MSG3) begin 
            case(state)
                IDLE: 
                    begin
                        if (ready_r[0]) begin
                            state <= STATE_0;
                        end else begin
                            state <= IDLE;
                       end
                       o_pkt_intf.sop      <= 0;
                       o_pkt_intf.valid    <= 0;
                       o_pkt_intf.eop      <= 0;
                       o_pkt_intf.len      <= 0;
                       o_pkt_intf.data     <= 0;
                       counter             <= 0;
                    end
//
                STATE_0: 
                    begin
                        if (ready_r[0]) begin
                            if (MSG3_field0_avail_dly) begin
                                if (counter ==2) begin
                                    counter <= 0;
                                    state               <= STATE_1;
                                end else begin 
                                    counter <= counter +1;
                                    state               <= STATE_0;
                                end
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.data     <= serial_data_MSG3_field0;
                                o_pkt_intf.len      <= 0;

                                if (counter == 0) begin
                                    o_pkt_intf.sop      <= 1;
                                end else begin
                                        o_pkt_intf.sop      <= 0;
                                end
                                o_pkt_intf.valid    <= 1;
                            end else begin
                                state               <= STATE_0;
                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.valid    <= 0;
                                o_pkt_intf.len      <= 0;
                                o_pkt_intf.data     <= 0;
                                counter             <= 0;
                            end
                        end else begin
                            state               <= IDLE;
                            o_pkt_intf.sop      <= 0;
                            o_pkt_intf.valid    <= 0;
                            o_pkt_intf.eop      <= 0;
                            o_pkt_intf.len      <= 0;
                            o_pkt_intf.data     <= 0;
                            counter             <= 0;
                        end
                    end

                STATE_1: 
                    begin
                        assert (state != STATE_0) else $error("There must be data transfer in STATE_0");
                        state               <= STATE_2;
                        o_pkt_intf.sop      <= 0;
                        o_pkt_intf.valid    <= 1;
                        o_pkt_intf.eop      <= 0;
                        o_pkt_intf.len      <= 0;
                        o_pkt_intf.data     <= 0;
                        counter             <= 0;
                    end

                STATE_2: 
                    begin
                        if (ready_r[0]) begin
                            if (MSG3_Z_avail_dly) begin
                                if (counter ==1) begin
                                    counter <= 0;
                                    state               <= STATE_3;
                                end else begin 
                                    counter <= counter +1;
                                    state               <= STATE_2;
                                end
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.data     <= serial_data_MSG3_Z;
                                o_pkt_intf.len      <= 0;

                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.valid    <= 1;
                            end else begin
                                state               <= STATE_2;
                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.valid    <= 1;
                                o_pkt_intf.len      <= 0;
                                o_pkt_intf.data     <= 0;
                                counter             <= 0;
                            end
                        end else begin
                            state               <= IDLE;
                            o_pkt_intf.sop      <= 0;
                            o_pkt_intf.valid    <= 0;
                            o_pkt_intf.eop      <= 0;
                            o_pkt_intf.len      <= 0;
                            o_pkt_intf.data     <= 0;
                            counter             <= 0;
                        end
                    end

                STATE_3: 
                    begin
                        if (ready_r[0]) begin
                            if (MSG3_field2_avail_dly) begin
                                if (counter ==1) begin
                                    counter <= 0;
                                    state               <= STATE_4;
                                end else begin 
                                    counter <= counter +1;
                                    state               <= STATE_3;
                                end
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.data     <= serial_data_MSG3_field2;
                                o_pkt_intf.len      <= 0;

                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.valid    <= 1;
                            end else begin
                                state               <= STATE_3;
                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.valid    <= 1;
                                o_pkt_intf.len      <= 0;
                                o_pkt_intf.data     <= 0;
                                counter             <= 0;
                            end
                        end else begin
                            state               <= IDLE;
                            o_pkt_intf.sop      <= 0;
                            o_pkt_intf.valid    <= 0;
                            o_pkt_intf.eop      <= 0;
                            o_pkt_intf.len      <= 0;
                            o_pkt_intf.data     <= 0;
                            counter             <= 0;
                        end
                    end

                STATE_4: 
                    begin
                        if (ready_r[0]) begin
                            if (MSG3_field3_avail_dly) begin
                                if (counter ==0) begin
                                    counter <= 0;
                                    state               <= IDLE;
                                end else begin 
                                    counter <= counter +1;
                                    state               <= STATE_4;
                                end
                                o_pkt_intf.eop       <= 1;
                                o_pkt_intf.data     <= serial_data_MSG3_field3;
                                o_pkt_intf.len      <= 1;

                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.valid    <= 1;
                            end else begin
                                state               <= STATE_4;
                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.valid    <= 1;
                                o_pkt_intf.len      <= 0;
                                o_pkt_intf.data     <= 0;
                                counter             <= 0;
                            end
                        end else begin
                            state               <= IDLE;
                            o_pkt_intf.sop      <= 0;
                            o_pkt_intf.valid    <= 0;
                            o_pkt_intf.eop      <= 0;
                            o_pkt_intf.len      <= 0;
                            o_pkt_intf.data     <= 0;
                            counter             <= 0;
                        end
                    end

                default:
                    begin
                        state               <= IDLE;
                        o_pkt_intf.sop      <= 0;
                        o_pkt_intf.valid    <= 0;
                        o_pkt_intf.eop      <= 1;
                        o_pkt_intf.len      <= 0;
                        o_pkt_intf.data     <= 0;
                        counter             <= 0;
                    end
            endcase
        end else if (i_msg_type.data == MSG4) begin 
            case(state)
                IDLE: 
                    begin
                        if (ready_r[0]) begin
                            state <= STATE_0;
                        end else begin
                            state <= IDLE;
                       end
                       o_pkt_intf.sop      <= 0;
                       o_pkt_intf.valid    <= 0;
                       o_pkt_intf.eop      <= 0;
                       o_pkt_intf.len      <= 0;
                       o_pkt_intf.data     <= 0;
                       counter             <= 0;
                    end
//
                STATE_0: 
                    begin
                        if (ready_r[0]) begin
                            if (MSG4_field0_avail_dly) begin
                                if (counter ==2) begin
                                    counter <= 0;
                                    state               <= STATE_1;
                                end else begin 
                                    counter <= counter +1;
                                    state               <= STATE_0;
                                end
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.data     <= serial_data_MSG4_field0;
                                o_pkt_intf.len      <= 0;

                                if (counter == 0) begin
                                    o_pkt_intf.sop      <= 1;
                                end else begin
                                        o_pkt_intf.sop      <= 0;
                                end
                                o_pkt_intf.valid    <= 1;
                            end else begin
                                state               <= STATE_0;
                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.valid    <= 0;
                                o_pkt_intf.len      <= 0;
                                o_pkt_intf.data     <= 0;
                                counter             <= 0;
                            end
                        end else begin
                            state               <= IDLE;
                            o_pkt_intf.sop      <= 0;
                            o_pkt_intf.valid    <= 0;
                            o_pkt_intf.eop      <= 0;
                            o_pkt_intf.len      <= 0;
                            o_pkt_intf.data     <= 0;
                            counter             <= 0;
                        end
                    end

                STATE_1: 
                    begin
                        assert (state != STATE_0) else $error("There must be data transfer in STATE_0");
                        state               <= STATE_2;
                        o_pkt_intf.sop      <= 0;
                        o_pkt_intf.valid    <= 1;
                        o_pkt_intf.eop      <= 0;
                        o_pkt_intf.len      <= 0;
                        o_pkt_intf.data     <= 0;
                        counter             <= 0;
                    end

                STATE_2: 
                    begin
                        if (ready_r[0]) begin
                            if (MSG4_Z_avail_dly) begin
                                if (counter ==1) begin
                                    counter <= 0;
                                    state               <= STATE_3;
                                end else begin 
                                    counter <= counter +1;
                                    state               <= STATE_2;
                                end
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.data     <= serial_data_MSG4_Z;
                                o_pkt_intf.len      <= 0;

                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.valid    <= 1;
                            end else begin
                                state               <= STATE_2;
                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.valid    <= 1;
                                o_pkt_intf.len      <= 0;
                                o_pkt_intf.data     <= 0;
                                counter             <= 0;
                            end
                        end else begin
                            state               <= IDLE;
                            o_pkt_intf.sop      <= 0;
                            o_pkt_intf.valid    <= 0;
                            o_pkt_intf.eop      <= 0;
                            o_pkt_intf.len      <= 0;
                            o_pkt_intf.data     <= 0;
                            counter             <= 0;
                        end
                    end

                STATE_3: 
                    begin
                        if (ready_r[0]) begin
                            if (MSG4_field2_avail_dly) begin
                                if (counter ==1) begin
                                    counter <= 0;
                                    state               <= STATE_4;
                                end else begin 
                                    counter <= counter +1;
                                    state               <= STATE_3;
                                end
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.data     <= serial_data_MSG4_field2;
                                o_pkt_intf.len      <= 0;

                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.valid    <= 1;
                            end else begin
                                state               <= STATE_3;
                                o_pkt_intf.sop      <= 0;
                                o_pkt_intf.eop      <= 0;
                                o_pkt_intf.valid    <= 1;
                                o_pkt_intf.len      <= 0;
                                o_pkt_intf.data     <= 0;
                                counter             <= 0;
                            end
                        end else begin
                            state               <= IDLE;
                            o_pkt_intf.sop      <= 0;
                            o_pkt_intf.valid    <= 0;
                            o_pkt_intf.eop      <= 0;
                            o_pkt_intf.len      <= 0;
                            o_pkt_intf.data     <= 0;
                            counter             <= 0;
                        end
                    end

                STATE_4: 
                    begin
                        state               <= IDLE;
                        o_pkt_intf.sop      <= 0;
                        o_pkt_intf.valid    <= 0;
                        o_pkt_intf.eop      <= 1;
                        o_pkt_intf.len      <= 0;
                        o_pkt_intf.data     <= 0;
                        counter             <= 0;
                    end

                default:
                    begin
                        state               <= IDLE;
                        o_pkt_intf.sop      <= 0;
                        o_pkt_intf.valid    <= 0;
                        o_pkt_intf.eop      <= 1;
                        o_pkt_intf.len      <= 0;
                        o_pkt_intf.data     <= 0;
                        counter             <= 0;
                    end
            endcase
        end else  begin 
            state               <= IDLE;
            o_pkt_intf.sop      <= 0;
            o_pkt_intf.valid    <= 0;
            o_pkt_intf.eop      <= 0;
            o_pkt_intf.len      <= 0;
            o_pkt_intf.data     <= 0;
            counter             <= 0;
        end
    end
end

endmodule

///////////////////////////////////

module PipeLine (
     clock,
     in,
     out0,
     out1
 );
 
 parameter   NUM_SYNC_STAGES             = 2;
 parameter   WIDTH                       = 1;
 
 input                                   clock;
 input                                   in;
 output   logic                          out0;
 output                                  out1;
 
 logic [NUM_SYNC_STAGES-1:0] synchronizer;
 integer i;
 logic out_r;
 logic out_r1;
 generate 
     if (NUM_SYNC_STAGES == 0) begin
         assign out_r = in;
     end else begin: data_synchronization 
         always @(posedge clock) begin
             synchronizer[0] <= in;
             for (i = 1; i < NUM_SYNC_STAGES; i = i+1) begin
                 synchronizer[i] <= synchronizer[i-1];
             end
         end
         assign out_r = synchronizer[NUM_SYNC_STAGES-1];
     end
 
     if (WIDTH == 1) begin
         always@(posedge clock) begin
             out_r1 <= out_r;
             out0 <= out_r1;
         end
     end else begin
         logic [WIDTH-2:0] counter;
     
         for (genvar i = 0; i < (WIDTH-2); i = i+1) begin 
             always@(posedge clock) begin
                 counter[i+1] <= counter[i];
             end 
         end   
         always@(posedge clock) begin
             counter[0] <= out_r;
         end
 
         always@(posedge clock) begin
             out_r1 <= out_r | (|counter);
             out0 <= out_r1;
         end
 
     end
     assign out1 = out_r;
 endgenerate
 
 
 endmodule
//
////
//
module PISO_SR # (
    parameter P_WIDTH = 4,
    parameter S_WIDTH = 1,
    parameter  NUM_SYNC_STAGES = 0
) (
    input clk,
    input load,
    input [P_WIDTH-1:0]Parallel_In,
    output logic [S_WIDTH-1: 0] Serial_Out
);
    logic [S_WIDTH-1: 0] serial_out_r;
    logic [P_WIDTH-1 :0] tmp;
    logic [S_WIDTH*NUM_SYNC_STAGES-1:0] synchronizer;
    integer i;
    always @(posedge clk) begin
        if(load) begin
            tmp <= Parallel_In;
        end else begin
            tmp <= tmp >> S_WIDTH;
            serial_out_r <= tmp[S_WIDTH-1 : 0];
        end
    end
    generate
        if(NUM_SYNC_STAGES == 0) begin
            assign Serial_Out = serial_out_r;
        end else begin
            always @(posedge clk) begin
                synchronizer[0 +: S_WIDTH] <= serial_out_r;
                for (i = 1; i < NUM_SYNC_STAGES; i = i+1) begin
                    synchronizer[S_WIDTH*i +: S_WIDTH] <= synchronizer[S_WIDTH*(i-1) +: S_WIDTH];
                end
            end
            assign Serial_Out = synchronizer[S_WIDTH*(NUM_SYNC_STAGES-1) +: S_WIDTH];
        end
    endgenerate

endmodule

