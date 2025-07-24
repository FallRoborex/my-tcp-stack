sudo ip link set tap0 up
ip link show tap0
sudo sysctl -w net.ipv6.conf.tap0.disable_ipv6=1
sudo python3 debugg.py
sudo tcpdump -i tap0 -n -e
