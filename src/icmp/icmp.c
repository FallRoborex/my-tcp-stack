#include "icmp.h"
#include <stdio.h>
#include <string.h>

#include "../ip/ipv4.h"
#include "../net/net.h"


void icmp_handle_packet(const uint8_t *data, size_t len, const uint8_t src_ip[4], const uint8_t dst_ip[4])
{
    if (len < sizeof(icmp_header))
    {
        printf("ICMP packet too short");
        return;
    }

    const icmp_header *icmp = (icmp_header *) data;

    if (icmp->type != 8)
    {
        printf("ICMP not an Echo Request: type=%d, code=%d", icmp->type, icmp->code);
        return;
    }

    printf("ICMP Echo request receive, Sending Echo Reply\n");

    // Prepare reply packet
    size_t reply_len = len;
    uint8_t reply[1500];
    icmp_header *reply_icmp = (icmp_header *)reply;

    memcpy(reply, data, len);
    reply_icmp->type = 0; // Echo Reply
    reply_icmp->code = 0;
    reply_icmp->checksum = 0;
    reply_icmp->checksum = net_checksum(reply, reply_len);

    // Build IPv4 packet
    ipv4_send_packet(dst_ip, src_ip, 1, reply, reply_len);

}
