#include "src/ethernet/ethernet.h"
#include "src/tap/tap_interface.h"
#include <string.h>
#include <net/if.h> 
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define IFNAMSIZ 16

int main() {
    printf("Starting TAP device\n");
    uint8_t dst_mac[6] = {0xde, 0xad, 0xef, 0x00, 0x02};
    uint8_t payload[100] = "Hello, world!";
    char dev[IFNAMSIZ] = "tap0";

    printf("Got Here\n");
    int tap_fd = tun_alloc(dev);
    printf("TAP device file descriptor: %d\n", tap_fd);
    if (tap_fd < 0) return 1;

    uint8_t buffer[1600];
    while (1) {
        int nread = read(tap_fd, buffer, sizeof(buffer));
        if (nread > 0) {
            printf("Read %d bytes from TAP device\n", nread);
            ethernet_receive(buffer, nread);
        }
    }
    return 0;
}