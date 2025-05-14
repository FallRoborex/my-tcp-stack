#include "arp.h"
#include "../ethernet/ethernet.h"
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>

#define MAX_ARP_CACHE 16

typedef struct {
    uint8_t ip_addr[4];
    uint8_t mac_addr[6];
} arp_entry_t;

static arp_entry_t arp_cache[MAX_ARP_CACHE];
static int arp_cache_size = 0;

void arp_cache_insert(const uint8_t *ip_addr, const uint8_t *mac_addr) {
    for (int i = 0; i < arp_cache_size; i++) {
        if (memcmp(arp_cache[i].ip_addr, ip_addr, 4) == 0) {
            memcpy(arp_cache[i].mac_addr, mac_addr, 6);
            return;
        }
    }

    if (arp_cache_size < MAX_ARP_CACHE) {
        memcpy(arp_cache[arp_cache_size].ip_addr, ip_addr, 4);
        memcpy(arp_cache[arp_cache_size].mac_addr, mac_addr, 6);
        arp_cache_size++;
    } else {
        // Cache is full, replace the oldest entry
        memcpy(arp_cache[0].ip_addr, ip_addr, 4);
        memcpy(arp_cache[0].mac_addr, mac_addr, 6);
    }
}

int arp_cache_lookup(const uint8_t *ip_addr, uint8_t *mac_addr_out) {
    for (int i = 0; i < arp_cache_size; i++) {
        if (memcmp(arp_cache[i].ip_addr, ip_addr, 4) == 0) {
            memcpy(mac_addr_out, arp_cache[i].mac_addr, 6);
            return 0;
        }
    }
    return -1;
}

void arp_send_request(const uint8_t *src_mac, const uint8_t *src_ip, const uint8_t *dst_ip) {
    arp_header arp_req;
    arp_req.hw_type = htons(ARP_HW_TYPE_ETHERNET);
    arp_req.proto = htons(ARP_PROTO_TYPE_IPv4);
    arp_req.hw_len = ARP_HW_LEN;
    arp_req.proto_len = ARP_PROTO_LEN;
    arp_req.operation = htons(ARP_OPCODE_REQUEST);
    memcpy(arp_req.sender_hw_addr, src_mac, ARP_HW_LEN);
    memcpy(arp_req.sender_ip_addr, src_ip, ARP_PROTO_LEN);
    memset(arp_req.target_hw_addr, 0, ARP_HW_LEN);
    memcpy(arp_req.target_ip_addr, dst_ip, ARP_PROTO_LEN);

    ethernet_send((uint8_t[]){0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 0x806, (uint8_t *)&arp_req, sizeof(arp_req));
}

void arp_receive(const uint8_t *packet, size_t len) {
    if (len < sizeof(arp_header)) {
        printf("ARP packet too short\n");
        return;
    }
    const arp_header *arp = (const arp_header *)packet;

    if (htonhs(arp->operation) == ARP_OPCODE_REPLY) {
        arp_cache_insert(arp->sender_ip_addr, arp->sender_hw_addr);
        printf("ARP Reply: Inserted %u.%u.%u.%u. -> %02x:%02x:%02x:%02x:%02x:%02x\n", 
            arp->sender_ip_addr[0], arp->sender_ip_addr[1], arp->sender_ip_addr[2], arp->sender_ip_addr[3],
            arp->sender_hw_addr[0], arp->sender_hw_addr[1], arp->sender_hw_addr[2], arp->sender_hw_addr[3], arp->sender_hw_addr[4], arp->sender_hw_addr[5]);
    }

}

