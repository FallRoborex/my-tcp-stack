#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stddef.h>

#define ETH_ADDR_LEN 6
#define ETH_TYPE_IPv4 0x0800
#define ETH_TYPE_ARP  0x0806

typedef struct {
    uint8_t dst_mac[ETH_ADDR_LEN];
    uint8_t src_mac[ETH_ADDR_LEN];
    uint16_t ethertype; // Ethernet type (A.K.A, IPv4, ARP)
    unsigned char payload[1500]; // Maximum payload size for Ethernet frame
} __attribute__((packed)) ethernet_header_t;

void ethernet_send(const uint8_t *dst_mac, uint16_t ethertype, const uint8_t *payload, size_t len);
void ethernet_receive(const uint8_t *frame, size_t len);
void ethernet_set_mac(const uint8_t *mac);
const uint8_t *ethernet_get_mac();

#endif
