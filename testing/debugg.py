from scapy.all import *
from scapy.layers.inet import ICMP, IP
from scapy.layers.l2 import Ether, ARP, arping

# IP and MAC of the machine running Scapy (test host)
test_ip = "10.0.0.2"
test_mac = "02:00:00:00:00:02"

# IP and MAC your stack is listening on
stack_ip = "10.0.0.1"
stack_mac = "02:00:00:00:00:01"

# # Send ARP request to your stack
# arp_req = Ether(src=test_mac, dst="ff:ff:ff:ff:ff:ff") / ARP(
#     op=1,  # who-has
#     hwsrc=test_mac,
#     psrc=test_ip,
#     hwdst="00:00:00:00:00:00",
#     pdst=stack_ip
# )
# sendp(arp_req, iface="tap0")
#
# # (Optional) Wait for ARP reply if you want to verify
# def get_mac(ip):
#     ans, _ = arping(ip, iface="tap0", timeout=2)
#     for s, r in ans:
#         return r[Ether].src
#     return None

# mac = get_mac(stack_ip)
# print(f"Stack responded with MAC: {mac}")

# Send ICMP echo request ("ping")
# icmp = Ether(src=test_mac, dst=stack_mac) / IP(src=test_ip, dst=stack_ip) / ICMP()
# sendp(icmp, iface="tap0")

sendp(
    Ether(dst="de:ad:ef:00:00:01") /
    IP(src="10.0.0.1", dst="10.0.0.2") /
    ICMP(type=8) /
    b"Hello from scapy!",
    iface="tap0"
)
