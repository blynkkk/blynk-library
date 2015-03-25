#!/usr/bin/python
'''
 This is a pseudo-server that sends predefined pattern to any connected client.
 It is used to test transport behaviour and throughput.

 If you want to use it with a sketch, connect your PC and Blynk-enabled device
 into the same network and configure Blynk to connect to this pseudo-server:

   IPAddress serv(192,168,0,105); // IP address of your PC
   Blynk.begin(auth, serv, 8888);

 Author:   Volodymyr Shymanskyy
 License:  The MIT license
'''
import struct
import select
import socket
import sys
from thread import *

# Configuration options

HOST = ''       # Bind to all interfaces
PORT = 8888     # Bind to port 8888
NODELAY = 0     # Enable TCP_NODELAY

WAIT = 0.001    # Wait some time between switching LED
MSG_QTY = 1024  # Amount of messages
DIVIDER = 1     # Only 1 in DIVIDER messages will be sent

LED_PIN = 9     # LED pin

# Blynk protocol helpers

hdr = struct.Struct("!BHH")

class MsgType:
    RSP    = 0
    LOGIN  = 2
    PING   = 6
    HW     = 20

class MsgStatus:
    OK     = 200

def hw(*args):
    # Convert params to string and join using \0
    data = "\0".join(map(str, args))
    # Prepend HW command header
    return hdr.pack(MsgType.HW, 1, len(data)) + data

# Main code

serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    # Set SO_REUSEADDR, this is needed to ignore WAIT state on next run
    serv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    serv.bind((HOST, PORT))
except socket.error as msg:
    print 'Bind failed. Error Code: {0}, Msg: {1}'.format(str(msg[0]), msg[1])
    sys.exit()

serv.listen(10)
print 'Listening on port', PORT

def clientthread(conn, addr):
    print 'Connection from {0}:{1}'.format(addr[0], str(addr[1]))
    if NODELAY:
        conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    b = 0
    msgs = 0
    authenticated = False
    while(True):
        rs, _, _ = select.select([conn] , [], [], WAIT*DIVIDER)
        if rs:
            data = conn.recv(hdr.size)
            if not data:
                break
            msg_type, msg_id, msg_len = hdr.unpack(data)
            #print "Got {0}, {1}, {2}".format(msg_type, msg_id, msg_len)
            if msg_type == MsgType.RSP:
                pass
            elif msg_type == MsgType.LOGIN:
                auth = conn.recv(msg_len)
                print "Auth {0}".format(auth)
                # Send auth OK and pin modes
                conn.sendall(hdr.pack(MsgType.RSP, msg_id, MsgStatus.OK))
                conn.sendall(hw("pm", LED_PIN, "out"))
                authenticated = True
            elif msg_type == MsgType.PING:
                print "Ping"
                # Send Pong
                conn.sendall(hdr.pack(MsgType.RSP, msg_id, MsgStatus.OK))
            elif msg_type == MsgType.HW:
                data = conn.recv(msg_len)
                # Print HW messages (just for fun :)
                print "Hw: {0}".format(data)
            else:
                break
        
        if (authenticated):
            conn.sendall(hw("aw", LED_PIN, b))
            b = (b + DIVIDER) % 256
            msgs += 1
        if msgs >= MSG_QTY/DIVIDER:
            break

    print "Finished sending {0} messages".format(msgs)
    conn.close()
 
# Wait for clients
while True:
    start_new_thread(clientthread, serv.accept())
 
serv.close()
