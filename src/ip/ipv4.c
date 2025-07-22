#include "ipv4.h"

#include <iso646.h>

#include "../net/net.h"
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

void ipv4_handle_packet(const uint8_t *data, size_t len)
{
    if (len > sizeof(ipv4_header))
    {
        printf("IPv4 packet is too short");
        return;
    }

    const ipv4_header *hdr = (const ipv4_header *) data;
    uint8_t version = hdr->version_ihl >> 4;
    uint8_t ihl = hdr->version_ihl & 0x0F;

    if (version != 4)
    {
        printf("Not a IPv4 packet (version: %d)\n", version);
        return;
    }

    size_t header_len = ihl * 4;
    if (len < header_len)
    {
        printf("Truncated IPv4 header\n");
        return;
    }

    uint16_t total_length = ntohs(hdr->total_length);
    if (len < total_length)
    {
        printf("Truncated IPv4 payload (expected %d bytes)\n", total_length);
        return;
    }
    printf("IPv4 packet:\n");
    printf("  From: %d.%d.%d.%d\n", hdr->src_ip[0], hdr->src_ip[1], hdr->src_ip[2], hdr->src_ip[3]);
    printf("  To  : %d.%d.%d.%d\n", hdr->dst_ip[0], hdr->dst_ip[1], hdr->dst_ip[2], hdr->dst_ip[3]);
    printf("  Protocol: %d\n", hdr->protocol);

    // Only handle packets for us
    extern uint8_t my_ip[4];
    if (memcmp(hdr->dst_ip, my_ip, 4) != 0)
    {
        printf("IPv4 packet not fo us (ignoring)\n");
        return;
    }

    const uint8_t *payload = data + header_len;
    size_t payload_len = total_length - header_len;

    switch (hdr->protocol)
    {
    case 1: // ICMP
        printf("    Handling ICMP (not implemented yet)\n");
        break;
    case 6: // TCP
        printf("    TCP packet received (not handled)\n");
        break;
    case 17: // UCP
        printf(    "UCP packet received (not handled)\n");
        break;
    default:
        printf("    Unknown IPv4 protocol: %d\n", hdr->protocol);
    }
}