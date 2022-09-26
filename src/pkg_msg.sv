package st_encoder_pkg;
    typedef logic[7:0] msg_type_t;

    typedef enum logic[7:0] {
        MSG3 = 4, 
        MSG4 = 6
    } msg_type_supported_t;


 // New Struct: MSG3
    typedef struct {
        logic field0_avail;
        logic[8:0] [7:0] field0;
        logic field1_avail;
        logic[2:0] [7:0] field1;
        logic Z_avail;
        logic[5:0] [7:0] Z;
        logic field2_avail;
        logic[4:0] [7:0] field2;
        logic field3_avail;
        logic[3:0] [7:0] field3;
    } MSG3_fields;

 // New Struct: MSG4
    typedef struct {
        logic field0_avail;
        logic[8:0] [7:0] field0;
        logic field1_avail;
        logic[2:0] [7:0] field1;
        logic Z_avail;
        logic[5:0] [7:0] Z;
        logic field2_avail;
        logic[4:0] [7:0] field2;
        logic field3_avail;
        logic[2:0] [7:0] field3;
    } MSG4_fields;


endpackage

