#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

int tun_alloc(char *dev) {
    struct ifreq ifr;
    int fd;

    char *dev_path = "/dev/net/tun";

    // Open the TAP device
    if ((fd = open(dev_path, O_RDWR)) < 0) {
        perror("Cannot open TUN/TAP dev\n");
        exit(1);
    }

    // Clear the structure
    memset(&ifr, 0, sizeof(ifr));

    // Set up the flags for the interface
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    // If a device name was provided, copy it to the structure
    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';  // Ensure null-termination
    }

    // Set the TAP device with the specified interface name
    if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
        perror("Error: Could not ioctl tun\n");
        close(fd);
        return -1;
    }
    
    // Copy the interface name back to dev
    strcpy(dev, ifr.ifr_name);
    printf("TAP device %s created\n", dev);
    printf("I GOT HERE\n");

    return fd;
}
