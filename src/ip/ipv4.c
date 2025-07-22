#include "ipv4.h"
#include "../net/net.h"
#include "../arp/arp.h"

#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

#include "ethernet.h"

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
        printf("Incomplete Header: len=%zu, expected=%zu\n", len, header_len);
        return;
    }

    // Verify the Checksum
    uint16_t receive_checksum = ntohs(hdr->header_checksum);
    uint16_t compute_checksum = net_checksum(data, header_len);
    if (compute_checksum != 0)
    {
        printf("IPv4 invalid checksum got 0x%04x , expected 0x0000\n", compute_checksum);
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
    if (memcmp(hdr->dst_ip, local_ip, 4) != 0)
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

void ipv4_send_packet(const uint8_t src_ip[4], const uint8_t dst_ip[4], uint8_t protocol, const uint8_t *payload, size_t payload_len)
{
    uint8_t packet[1500];
    ipv4_header *ip = (ipv4_header *) packet;

    ip->version_ihl = 0x45;
    ip->tos = 0;
    ip->total_length = htons(sizeof(ipv4_header) + payload_len);
    ip->identification = htons(0);
    ip->flags_fragment_offset = htons(0);
    ip->ttl = 64;
    ip->protocol = protocol;
    ip->header_checksum = 0;
    memcpy(ip->src_ip, src_ip, 4);
    memcpy(ip->dst_ip, dst_ip, 4);
    ip->header_checksum = net_checksum(packet, sizeof(ipv4_header));

    memcpy(packet + sizeof(ipv4_header), payload, payload_len);

    // Resolve destination MAC using ARP
    uint8_t dst_mac[6];
    if (!arp_resolve(dst_mac, dst_ip))
    {
        printf("IPv4: Cannot send packet, ARP lookup failed\n");
        return;
    }

    ethernet_send(dst_ip, ARP_HW_TYPE_ETHERNET, packet, sizeof(ipv4_header) + payload_len);
}