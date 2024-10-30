#!/bin/bash

PORT=4444
HOST=localhost
DELAY=1

while true; do
    clear
    echo "Connecting to QEMU serial port at $HOST:$PORT..."
    nc $HOST $PORT
    echo "Disconnected from QEMU serial port."
    echo "Reconnecting in $DELAY seconds..."
    sleep $DELAY
done
