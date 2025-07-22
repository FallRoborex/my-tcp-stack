#ifndef NET_H
#define NET_H

#include <stdint.h>

extern uint8_t local_mac[6];
extern uint8_t local_ip[4];

uint16_t net_checksum(const void *data, int len);
#endif //NET_H
