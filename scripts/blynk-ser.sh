#!/bin/sh

# === Edit these lines to match your need ===
COMM_PORT=/dev/ttyUSB0
COMM_BAUD=115200
SERV_ADDR=cloud.blynk.cc
SERV_PORT=8282

# === Edit lines below only if absolutely sure what you're doing ===
while [ 1 ]; do
  socat -d FILE:$COMM_PORT,raw,echo=0,b$COMM_BAUD,nonblock=1 TCP:$SERV_ADDR:$SERV_PORT
  sleep 2
done
