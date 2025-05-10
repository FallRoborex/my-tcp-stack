#include "ethernet/ethernet.h"
#include <string.h>

int main() {
    uint8_t dst_mac[6] = {0xde, 0xad, 0xef, 0x00, 0x02};
    uint8_t payload[100] = "Hello, Ethernet!";

    ethernet_send(dst_mac, ETH_TYPE_IPv4, payload, sizeof(payload));
    return 0;
}