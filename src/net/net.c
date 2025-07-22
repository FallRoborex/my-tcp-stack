#include "net.h"

#include <sys/types.h>

uint8_t local_mac[6] = {0xde, 0xad, 0xef, 0x00, 0x01, 0x00}; // Random MAC ip
uint8_t local_ip[4] = {10, 0, 0, 2}; // TAP device ip

uint16_t net_checksum(const void *data, int len)
{
    register uint32_t sum = 0;
    const uint16_t *ptr = data;

    while (len > 1)
    {
        sum += *ptr++;
        len -= 2;
    }

    if (len > 0)
    {
        sum += *(uint8_t *) ptr;
    }

    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;

}