#!/bin/bash

# === Edit default options to match your need ===

COMM_PORT_LINUX=/dev/ttyUSB0
COMM_PORT_OSX=/dev/tty.usbserial
COMM_BAUD=9600
SERV_ADDR=blynk-test-east.cloudapp.net
SERV_PORT=8442

# === Edit the lines below only if absolutely sure what you're doing ===

# Detect OS type
if [[ "$OSTYPE" == "linux-gnu" ]]; then
    COMM_PORT=$COMM_PORT_LINUX
elif [[ "$OSTYPE" == "darwin"* ]]; then
    COMM_PORT=$COMM_PORT_OSX
else
    echo "Can't detect OS type!"
    exit 1
fi

# Setup exit handler
trap "echo Exited!; exit;" SIGINT SIGTERM

usage="
    This script redirects serial communication to the server.

    You can specify port, baud rate, and server endpoint like this:
      blynk-ser.sh -c <serial port> -b <baud rate> -s <server address> -p <server port>

    The defaults are:
      -c    /dev/ttyUSB0       (on Linux)
            COM1               (on Windows)
            /dev/tty.usbserial (on OSX)
      -b    9600
      -s    blynk-test-east.cloudapp.net
      -p    8442

    If the specified serial port is not found, it will ask to enter another one.
    The script also tries to reestablish connection if it was lost.
"

# Detect socat
if ! hash socat2 2>/dev/null; then
    echo "This script used socat utility, but could not find it."
    echo
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        echo "    Try installing it using: sudo apt-get install socat"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "    Try installing it using this guide: http://www.xappsoftware.com/wordpress/2013/10/10/how-to-run-socat-on-mac-os-x/"
    fi
    exit 1
fi

# Get command line options
while getopts "h?vs:p:c:" opt; do
    case "$opt" in
    h|\?)
        echo -n "$usage"
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

# Ask for serial port interactively if not found
if [ ! -e "$COMM_PORT" ]
then
    echo $COMM_PORT not found.
    echo -n "Select serial port [" `ls /dev/ttyUSB* /dev/ttyACM* 2> /dev/null` "]: "
    read COMM_PORT
fi

# Do the job
echo Connecting device $COMM_PORT to $SERV_ADDR:$SERV_PORT...
echo [ Press Ctrl+C to exit ]
while [ 1 ]; do
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        socat -d -d FILE:$COMM_PORT,raw,echo=0,b$COMM_BAUD,nonblock=1 TCP:$SERV_ADDR:$SERV_PORT,nodelay
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        socat -U -d -d GOPEN:$COMM_PORT,clocal=1,cs8,nonblock=1,ixoff=0,ixon=0,ispeed=$COMM_BAUD,ospeed=$COMM_BAUD,raw,echo=0,crtscts=0 TCP:$SERV_ADDR:$SERV_PORT,nodelay
    fi
    echo Reconnecting in 3s...
    sleep 3
done

