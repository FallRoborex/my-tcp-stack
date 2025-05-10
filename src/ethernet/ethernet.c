#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "ethernet.h"

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
    memcpy(frame + sizeof(ethernet_header_t), payload, len);

    size_t frame_len = sizeof(ethernet_header_t) + len;
    printf("Destination MAC\n: %02x:%02x:%02x:%02x:%02x:%02x\n",
              hdr -> dst_mac[0], hdr -> dst_mac[1], hdr -> dst_mac[2],
              hdr -> dst_mac[3], hdr -> dst_mac[4], hdr -> dst_mac[5]);
    printf("Source MAC\n: %02x:%02x:%02x:%02x:%02x:%02x\n",
              hdr -> src_mac[0], hdr -> src_mac[1], hdr -> src_mac[2],
              hdr -> src_mac[3], hdr -> src_mac[4], hdr -> src_mac[5]);
    
    // Send the frame to the network using a socket or TAP interface
    // For now, just print the frame length
    printf("Frame length: %zu bytes\n", frame_len);
    
}

void ethernet_receive(const uint8_t *frame, size_t len) {
    const ethernet_header_t *hdr = (const ethernet_header_t *)frame;
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
              hdr -> src_mac[0], hdr -> src_mac[1], hdr -> src_mac[2],
              hdr -> src_mac[3], hdr -> src_mac[4], hdr -> src_mac[5]);
    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
              hdr -> dst_mac[0], hdr -> dst_mac[1], hdr -> dst_mac[2],
              hdr -> dst_mac[3], hdr -> dst_mac[4], hdr -> dst_mac[5]);
    
}