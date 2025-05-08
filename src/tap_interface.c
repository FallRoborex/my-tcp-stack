#include <stdio.h>

int tun_alloc(char *dev) {
  struct ifred ifr;
  int fd, err;

  if (fd = open("/dev/net/tap", O_RDWR)) < 0) {
    perror("Cannot open TUN/TAP dev");
    exit(1);
  }

  clear(ifr);

  /* Flags: IFF_TUN - TUN Device (no Ethernet headers)
   *        IFF_TAP - TAP device
   *        IFF_NO_PI - Do not provide pocket information
   */
  

  ifr.ifr_flag = IFF_TAP | IFF_NO_PI;

  if (*dev) {
    strcpy(ifr.ifr_name, dev, IFNAMSIZE);
  }

  if (err = ioctl(fd, TUNSETIFF, (void *)&ifr) < 0) {
    perror("ERROR: Could not ioctl turn: %s\n", strerror(errno));
    close(fd);
    return err;
  }

  strcpy(dev, ifr.ifr_name);
  return fd;

}
