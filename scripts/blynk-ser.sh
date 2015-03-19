#!/bin/bash

# === Edit these lines to match your need ===
COMM_PORT=/dev/ttyUSB0
COMM_BAUD=9600
SERV_ADDR=blynk-test-east.cloudapp.net
SERV_PORT=8442

# === Edit lines below only if absolutely sure what you're doing ===
trap "echo Exited!; exit;" SIGINT SIGTERM

while getopts "h?vs:p:c:" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    s)  SERV_ADDR=$OPTARG
        ;;
    p)  SERV_PORT=$OPTARG
        ;;
    c)  COMM_PORT=$OPTARG
        ;;
    b)  COMM_BAUD=$OPTARG
        ;;
    esac
done

if [ ! -e "$COMM_PORT" ]
then
    echo -n "Select port [" `ls /dev/ttyUSB* /dev/ttyACM* 2> /dev/null` "]: "
    read COMM_PORT
fi

echo Connecting device at $COMM_PORT to $SERV_ADDR:$SERV_PORT...
echo [ Press Ctrl+C to exit ]
while [ 1 ]; do
  if [[ "$OSTYPE" == "linux-gnu" ]]; then
    socat -d -d FILE:$COMM_PORT,raw,echo=0,b$COMM_BAUD,nonblock=1 TCP:$SERV_ADDR:$SERV_PORT
  elif [[ "$OSTYPE" == "darwin"* ]]; then
    socat -U -d -d GOPEN:$COMM_PORT,clocal=1,cs8,nonblock=1,ixoff=0,ixon=0,ispeed=$COMM_BAUD,ospeed=$COMM_BAUD,raw,echo=0,crtscts=0 TCP:$SERV_ADDR:$SERV_PORT
  else
    echo "Can't detect OS type!"
    exit 1
  fi
  echo Reconnecting in 3s...
  sleep 3
done

