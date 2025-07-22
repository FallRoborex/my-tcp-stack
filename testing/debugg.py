from scapy.all import *
from scapy.layers.l2 import Ether, ARP

# Must match your TAP device name
iface = "tap0"

# IP and MAC of the simulated host
fake_ip = "10.0.0.2"
fake_mac = "aa:bb:cc:dd:ee:ff"

# Your stack's IP address (the one it should resolve)
target_ip = "10.0.0.1"

# Create and send ARP reply to simulate response to request
arp_reply = Ether(src=fake_mac, dst="ff:ff:ff:ff:ff:ff") / ARP(
    op="is-at", psrc=fake_ip, hwsrc=fake_mac, pdst=target_ip, hwdst="de:ad:ef:00:00:00"
)
sendp(arp_reply, iface=iface)

print("Sent ARP reply")
