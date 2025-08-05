#!/bin/bash

# Проверка прав root
if [ "$(id -u)" -ne 0 ]; then
    echo "This script must be run as root" >&2
    exit 1
fi

# 1. Настройка широковещания
echo "Configuring network settings..."
sudo sysctl -w net.ipv4.icmp_echo_ignore_broadcasts=0
sudo sysctl -w net.ipv4.ip_forward=0          # Отключаем форвардинг (для безопасности)
sudo sysctl -w net.ipv4.conf.all.bc_forwarding=0

# 2. Настройка сетевых интерфейсов
INTERFACE=$(ip route | awk '/default/ {print $5}')
if [ -z "$INTERFACE" ]; then
    echo "Could not determine network interface" >&2
    exit 1
fi

echo "Detected network interface: $INTERFACE"

# 3. Включение широковещания на интерфейсе
sudo sysctl -w net.ipv4.conf.$INTERFACE.broadcast=1
sudo ifconfig $INTERFACE broadcast 255.255.255.255

# 4. Настройка firewall (UFW)
if command -v ufw >/dev/null 2>&1; then
    echo "Configuring firewall..."
    sudo ufw allow out to 255.255.255.255     # Разрешаем исходящий broadcast
    sudo ufw allow in to any port 12345       # Разрешаем порт чата (замените на ваш)
fi

# 5. Проверка настроек
echo -e "\nCurrent network settings:"
echo "---------------------------------"
sysctl net.ipv4.icmp_echo_ignore_broadcasts
sysctl net.ipv4.conf.$INTERFACE.broadcast
ip addr show $INTERFACE | grep -w inet
echo "---------------------------------"

echo -e "\nNetwork setup complete. You can now run the chat program."