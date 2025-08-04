#!/bin/bash

# Enable broadcast
sudo sysctl -w net.ipv4.icmp_echo_ignore_broadcasts=0

# Check if broadcast is enabled
echo "Broadcast settings:"
sysctl net.ipv4.icmp_echo_ignore_broadcasts

echo "Network setup complete. You can now run the chat program."