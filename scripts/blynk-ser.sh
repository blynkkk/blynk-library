#!/bin/bash

# === Edit these lines to match your need ===
COMM_PORT=/dev/ttyUSB0
COMM_BAUD=9600
SERV_ADDR=blynk-test-east.cloudapp.net
SERV_PORT=8442

# === Edit lines below only if absolutely sure what you're doing ===
echo Connecting device at $COMM_PORT to $SERV_ADDR:$SERV_PORT...
while [ 1 ]; do
  socat -d FILE:$COMM_PORT,raw,echo=0,b$COMM_BAUD,nonblock=1 TCP:$SERV_ADDR:$SERV_PORT
  sleep 3
  echo Reconnecting...
done

