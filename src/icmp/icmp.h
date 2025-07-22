#ifndef ICMP_H
#define ICMP_H

#include <stddef.h>
#include <stdint.h>


typedef struct
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence_number;
    uint32_t payload;
}icmp_header;

void icmp_handle_packet(const uint8_t *data, size_t len, const uint8_t src_ip[4], const uint8_t dst_ip[4]);
#endif //ICMP_H
