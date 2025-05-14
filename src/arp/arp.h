#ifndef ARP_H
#define ARP_H

#include <stdint.h>
#include <stddef.h>

#define ARP_HW_TYPE_ETHERNET 1
#define ARP_PROTO_TYPE_IPv4 0x0800
#define ARP_HW_LEN 6
#define ARP_PROTO_LEN 4
#define ARP_OPCODE_REQUEST 1
#define ARP_OPCODE_REPLY 2



typedef struct {
    uint16_t hw_type;
    uint16_t proto;
    uint8_t hw_len;
    uint8_t proto_len;
    uint16_t operation; // Operation Code
    uint8_t sender_hw_addr[ARP_HW_LEN];
    uint8_t sender_ip_addr[ARP_PROTO_LEN];
    uint8_t target_hw_addr[ARP_HW_LEN];
    uint8_t target_ip_addr[ARP_PROTO_LEN];
} __attribute__((packed)) arp_header;

// void arp_handle_packet(const uint8_t *packet, size_t len);
void arp_send_request(const uint8_t *src_mac, const uint8_t *src_ip, const uint8_t *dst_ip);
void arp_receive(const uint8_t *packet, size_t len);
void arp_cache_insert(const uint8_t *ip_addr, const uint8_t *mac_addr);
int arp_cache_lookup(const uint8_t *ip_addr, uint8_t *mac_addr_out);

#endif