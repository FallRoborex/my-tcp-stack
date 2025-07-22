#ifndef IPv4_H
#define IPv4_H

#include <stdint.h>
#include <stddef.h>


typedef struct
{
    uint8_t version_ihl; // Version (4 bits) + header length (4 bits)
    uint8_t tos;
    uint8_t total_length;
    uint8_t identification;
    uint16_t flags_fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint8_t src_ip[4];
    uint8_t dst_ip[4];
} ipv4_header;

void ipv4_handle_packet(const uint8_t *data, size_t len);
void ipv4_send_packet(const uint8_t src_ip[4], const uint8_t dst_ip[4], uint8_t protocol, const uint8_t *payload, size_t payload_len);


#endif