#include "src/ethernet/ethernet.h"
#include "src/tap/tap_interface.h"
#include <string.h>
#include <net/if.h> 
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "src/arp/arp.h"
#include "src/net/net.h"

#define IFNAMSIZ 16


int get_local_mac(const char *iface_name, uint8_t mac[6])
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd < 0)
    {
        perror("Socket Error");
        return -1;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface_name, IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0)
    {
        perror("IOCTL");
        close(fd);
        return -1;
    }

    close(fd);
    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    return 0;
}

int main() {
    printf("Starting TAP device\n");
    uint8_t dst_mac[6] = {0xde, 0xad, 0xef, 0x00, 0x02};
    uint8_t payload[100] = "Hello, world!";
    char dev[IFNAMSIZ] = "tap0";

    int tap_fd = tun_alloc(dev);
    printf("TAP device file descriptor: %d\n", tap_fd);
    if (tap_fd < 0) return 1;

    printf("Sending ARP request from 10.0.0.2\n");
    uint8_t test_ip[4] = {10, 0, 0, 2};

    if (get_local_mac(dev, local_mac))
    {
        fprintf(stderr, "FAILED to get mac address for %s\n", dev);
        return 1;
    }

    arp_send_request(local_mac, local_ip, test_ip);

    uint8_t buffer[1600];
    while (1) {
        int nread = read(tap_fd, buffer, sizeof(buffer));
        if (nread > 0) {
            // printf("\nRead %d bytes from TAP device\n", nread);
            ethernet_receive(buffer, nread);
            printf("\n");
        }
    }
    // return 0;
}