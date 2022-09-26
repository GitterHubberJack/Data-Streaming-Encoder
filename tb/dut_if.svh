import st_encoder_pkg::*;

interface st_intf #(
    parameter type data_t = logic [7:0]
) (
);
logic ready;
    logic valid;
    data_t data;

    modport master (input  ready ,
                    output valid , data   );

    modport slave  (input  valid , data ,
                    output ready          );

endinterface

interface st_pkt_intf #(
    parameter WIDTH = 32
) (
);
    logic                       ready ;
    logic                       valid ;
    logic                       sop   ;
    logic                       eop   ;
    logic[WIDTH-1:0]            data  ;
    logic[$clog2(WIDTH/8)-1:0]  len   ;

    modport master(input  ready ,
                   output valid , sop   ,
                          eop   , data  ,
                          len           );

    modport slave (input  valid , sop   ,
                          eop   , data  ,
                          len   ,
                   output ready         );

endinterface
interface dut_if (
    st_intf i_msg_type,
    st_pkt_intf o_pkt_intf
);
    logic clk;
    logic rst_n;
    //
    MSG3_fields  MSG3_i_fields;
    MSG4_fields  MSG4_i_fields;
endinterface
//
interface internal_if;
    reg [$clog2(15) -1 : 0] counter;
    reg msg_valid;
    msg_type_t msg_type;
endinterface

