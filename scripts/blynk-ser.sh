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
echo [ Press Ctrl+C to exit ]

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

avrdude_warn="Warning: avrdude is running
 Avoid uploading sketches and running this script at the same time.
 If you didn't run avrdude, maybe it just stuck. You can kill it using:
   killall avrdude
"

function detect_conflicts {
    # Detect if avrdude is running
    if pidof -x avrdude > /dev/null; then echo -n "$avrdude_warn"; fi
}

detect_conflicts

# Detect socat
if ! hash socat 2>/dev/null; then
    echo "This script uses socat utility, but could not find it."
    echo
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        echo "  Try installing it using: sudo apt-get install socat"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "  Try installing it using this guide: http://www.xappsoftware.com/wordpress/2013/10/10/how-to-run-socat-on-mac-os-x/"
    fi
    exit 1
fi

# Get command line options
while getopts "h?s:p:c:b:" opt; do
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
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        echo -n "Select serial port [" `ls /dev/ttyUSB* /dev/ttyACM* 2> /dev/null` "]: "
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo -n "Select serial port [" `ls /dev/tty.usbserial* 2> /dev/null` "]: "
    fi
    read COMM_PORT
fi

# Do the job
echo Resetting device $COMM_PORT...
stty -F $COMM_PORT hupcl

while [ 1 ]; do
    echo Connecting device $COMM_PORT to $SERV_ADDR:$SERV_PORT...

    TCP_ATTR="nodelay" #,nonblock=1,rcvtimeo=1,sndtimeo=1
    SER_ATTR="raw,echo=0,clocal=1,cs8,nonblock=1"
    GEN_ATTR="-d -d -T 15"

    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        socat $GEN_ATTR  FILE:$COMM_PORT,$SER_ATTR,b$COMM_BAUD TCP:$SERV_ADDR:$SERV_PORT,$TCP_ATTR
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        socat $GEN_ATTR GOPEN:$COMM_PORT,$SER_ATTR,ixoff=0,ixon=0,ispeed=$COMM_BAUD,ospeed=$COMM_BAUD,crtscts=0 TCP:$SERV_ADDR:$SERV_PORT,$TCP_ATTR
    fi
    detect_conflicts
    echo Reconnecting in 3s...
    sleep 3
done

