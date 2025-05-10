
from scapy.all import *

eth = Ether(dst="de:ad:ef:00:00:00", src="ca:fe:ba:be:00:00", type=0x0800)
payload = b"Hello from Scapy!"
frame = eth / Raw(load=payload)

sendp(frame, iface="tap0")
print("Message sent")
