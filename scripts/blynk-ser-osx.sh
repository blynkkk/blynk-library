#!/bin/bash

# === Edit these lines to match your need ===
COMM_PORT=/dev/tty.usbserial
COMM_BAUD=115200
SERV_ADDR=cloud.blynk.cc
SERV_PORT=8282

# === Edit lines below only if absolutely sure what you're doing ===
while [ 1 ]; do
  sudo socat -U -d $COMM_PORT,clocal=1,cs8,nonblock=1,ixoff=0,ixon=0,ispeed=$COMM_BAUD,ospeed=$COMM_BAUD,raw,echo=0,crtscts=0 TCP:$SERV_ADDR:$SERV_PORT
  sleep 2
done
