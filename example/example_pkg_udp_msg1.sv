//****************************************************//
//     AUTOMATICALLY GENERATED FILE - DO NOT EDIT     //
//****************************************************//

package st_encoder_pkg;

    typedef logic[7:0] msg_type_t;

    typedef enum logic[7:0] {
        UDP_MSG1 = 1
    } msg_type_supported_t;

    typedef struct {
        logic eth_dst_avail;
        logic[5:0][7:0] eth_dst;
        logic eth_src_avail;
        logic[5:0][7:0] eth_src;
        logic eth_type_avail;
        logic[1:0][7:0] eth_type;
        logic ip_version_ihl_avail;
        logic[0:0][7:0] ip_version_ihl;
        logic ip_dscp_ecn_avail;
        logic[0:0][7:0] ip_dscp_ecn;
        logic ip_length_avail;
        logic[1:0][7:0] ip_length;
        logic ip_id_avail;
        logic[1:0][7:0] ip_id;
        logic ip_flags_fragment_offset_avail;
        logic[1:0][7:0] ip_flags_fragment_offset;
        logic ip_ttl_avail;
        logic[0:0][7:0] ip_ttl;
        logic ip_protocol_avail;
        logic[0:0][7:0] ip_protocol;
        logic ip_checksum_avail;
        logic[1:0][7:0] ip_checksum;
        logic ip_src_avail;
        logic[3:0][7:0] ip_src;
        logic ip_dst_avail;
        logic[3:0][7:0] ip_dst;
        logic udp_src_port_avail;
        logic[1:0][7:0] udp_src_port;
        logic udp_dst_port_avail;
        logic[1:0][7:0] udp_dst_port;
        logic udp_len_avail;
        logic[1:0][7:0] udp_len;
        logic udp_checksum_avail;
        logic[1:0][7:0] udp_checksum;
        logic field0_avail;
        logic[3:0][7:0] field0;
        logic field1_avail;
        logic[2:0][7:0] field1;
        logic field2_avail;
        logic[4:0][7:0] field2;
    } fields_t;

endpackage
