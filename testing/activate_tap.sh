sudo ip link set tap0 up
ip link show tap0
sudo python3 debugg.py
sudo tcpdump -i tap0 -n -e
