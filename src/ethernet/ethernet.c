#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "ethernet.h"
#include <arpa/inet.h>

#include "../arp/arp.h"
#include "../ip/ipv4.h"

static uint8_t local_mac[ETH_ADDR_LEN] = {0xde, 0xad, 0xef, 0x00, 0x01};

void ethernet_set_mac(const uint8_t *mac) {
    memcpy(local_mac, mac, ETH_ADDR_LEN);
}

const uint8_t *ethernet_get_mac() {
    return local_mac;
}

void ethernet_send(const uint8_t *dst_mac, uint16_t ethertype, const uint8_t *payload, size_t len) {
    uint8_t frame[1514];
    ethernet_header_t *hdr = (ethernet_header_t *)frame;
    
    memcpy(hdr -> dst_mac, dst_mac, ETH_ADDR_LEN);
    memcpy(hdr -> src_mac, local_mac, ETH_ADDR_LEN);
    hdr -> ethertype = htons(ethertype);

    printf("Etherenet Header size: %zu\n", sizeof(ethernet_header_t));

    if (sizeof(ethernet_header_t) + len > 1514)
    {
        printf("Buffer overload: %lu\n", sizeof(ethernet_header_t) + len);
        perror("Buffer overload\n");
        return;
    }
    memcpy(frame + sizeof(ethernet_header_t), payload, len);

    size_t frame_len = sizeof(ethernet_header_t) + len;
    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
              hdr -> dst_mac[0], hdr -> dst_mac[1], hdr -> dst_mac[2],
              hdr -> dst_mac[3], hdr -> dst_mac[4], hdr -> dst_mac[5]);
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
              hdr -> src_mac[0], hdr -> src_mac[1], hdr -> src_mac[2],
              hdr -> src_mac[3], hdr -> src_mac[4], hdr -> src_mac[5]);
    
    // Send the frame to the network using a socket or TAP interface
    // For now, just print the frame length
    printf("Frame length: %zu bytes\n", frame_len);
    
}

void ethernet_receive(const uint8_t *frame, size_t len) {
    if (len < sizeof(ethernet_header_t)) {
        printf("Frame too short to be a valid Ethernet Frame\n");
        return;
    }

    const ethernet_header_t *hdr = (const ethernet_header_t *)frame;
    const uint8_t *payload = frame + sizeof(ethernet_header_t);
    size_t payload_len = len - sizeof(ethernet_header_t);
    uint16_t ethertype = ntohs(hdr->ethertype);

    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           hdr->src_mac[0], hdr->src_mac[1], hdr->src_mac[2],
           hdr->src_mac[3], hdr->src_mac[4], hdr->src_mac[5]);

    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           hdr->dst_mac[0], hdr->dst_mac[1], hdr->dst_mac[2],
           hdr->dst_mac[3], hdr->dst_mac[4], hdr->dst_mac[5]);

    printf("Ethertype: 0x%04x\n", ethertype);

    switch (ethertype) {
    case 0x0806: // ARP
        printf("ARP request received\n");
        arp_handle_packet(payload, payload_len);
        break;

    case 0x0800: // IPv4
        printf("IPv4 packet received (not handled yet)\n");
        ipv4_handle_packet(payload, payload_len);
        break;

    case 0x86DD: // IPv6
        printf("Skipping IPv6 packet\n");
        break;

    default:
        printf("Unknown ethertype: 0x%04x (skipping)\n", ethertype);
        break;
    }
}
