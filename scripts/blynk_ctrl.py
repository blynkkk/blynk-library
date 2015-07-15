#!/usr/bin/env python
'''
 This script uses Bridge feature to control another device from the command line.

 Examples:
   python blynk_ctrl.py --token=b168ccc8c8734fad98323247afbc1113 write D0 1
   python blynk_ctrl.py --token=b168ccc8c8734fad98323247afbc1113 write A0 123
   python blynk_ctrl.py --token=b168ccc8c8734fad98323247afbc1113 write V0 "some value"
   
   Note: read is not supported yet

 Author:   Volodymyr Shymanskyy
 License:  The MIT license
'''

import select, socket, struct
import os, sys, time, getopt # TODO: use optparse

# Parse command line options
try:
    opts, args = getopt.getopt(sys.argv[1:],
        "hs:p:t:",
        ["help", "server=", "port=", "token=", "dump", "target="])
except getopt.GetoptError:
    print >>sys.stderr, __doc__
    sys.exit(2)

# Default options
TOKEN = "YourAuthToken"

SERVER = "cloud.blynk.cc"
PORT = 8442

# Expert options
DUMP = 0
NODELAY = 1     # TCP_NODELAY
BRIDGE_PIN = 64
TARGET = None

for o, v in opts:
    if o in ("-h", "--help"):
        print __doc__
        sys.exit()
    elif o in ("-s", "--server"):
        SERVER = v
    elif o in ("-p", "--port"):
        PORT = int(v)
    elif o in ("--dump",):
        DUMP = 1
    elif o in ("-t", "--token"):
        TOKEN = v
        TARGET = v
    elif o in ("--target",):
        TARGET = v
    else:
        print __doc__
        sys.exit(1)

if not TARGET or len(args) < 2:
    print __doc__
    sys.exit(1)
# Blynk protocol helpers

hdr = struct.Struct("!BHH")

class MsgType:
    RSP    = 0
    LOGIN  = 2
    PING   = 6
    BRIDGE = 15
    HW     = 20

class MsgStatus:
    OK     = 200

def bridge(*args):
    # Convert params to string and join using \0
    data = "\0".join(map(str, args))
    dump("< " + " ".join(map(str, args)))
    # Prepend BRIDGE command header
    return hdr.pack(MsgType.BRIDGE, genMsgId(), len(data)) + data

static_msg_id = 1
def genMsgId():
    global static_msg_id
    static_msg_id += 1
    return static_msg_id

# Other utilities

start_time = time.time()
def log(msg):
    print "[{:7.3f}] {:}".format(float(time.time() - start_time), msg)

def dump(msg):
    if DUMP:
        log(msg)

def receive(sock, length):
    d = []
    l = 0
    while l < length:
        r = ''
        try:
            r = sock.recv(length-l)
        except socket.timeout:
            continue
        if not r:
            return ''
        d.append(r)
        l += len(r)
    return ''.join(d)

# Main code
try:
    conn = socket.create_connection((SERVER, PORT), 3)
except:
    log("Can't connect to %s:%d" % (SERVER, PORT))
    sys.exit(1)

if NODELAY != 0:
    conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    
# Authenticate
conn.sendall(hdr.pack(MsgType.LOGIN, 1, len(TOKEN)))
conn.sendall(TOKEN)
data = receive(conn, hdr.size)
if not data:
    log("Login timeout")
    sys.exit(1)

msg_type, msg_id, status = hdr.unpack(data)
if MsgType.RSP != 0 or msg_id != 1 or status != MsgStatus.OK:
    log("Login failed: {0}, {1}, {2}".format(msg_type, msg_id, status))
    sys.exit(1)

conn.sendall(bridge(BRIDGE_PIN, "i", TARGET));

op = args[0]
pin = args[1]

if op == 'write' or op == 'set':
    val = args[2]
    if pin[0] == 'D' or pin[0] == 'd':
        conn.sendall(bridge(BRIDGE_PIN, "dw", pin[1:], val));
    elif pin[0] == 'A' or pin[0] == 'a':
        conn.sendall(bridge(BRIDGE_PIN, "aw", 'A'+pin[1:], val));
    elif pin[0] == 'V' or pin[0] == 'v':
        conn.sendall(bridge(BRIDGE_PIN, "vw", pin[1:], val));
    else:
        log("Invalid pin format")
elif op == 'read' or op == 'get':
    if pin[0] == 'D' or pin[0] == 'd':
        conn.sendall(bridge(BRIDGE_PIN, "dr", pin[1:]));
    elif pin[0] == 'A' or pin[0] == 'a':
        conn.sendall(bridge(BRIDGE_PIN, "ar", 'A'+pin[1:]));
    elif pin[0] == 'V' or pin[0] == 'v':
        conn.sendall(bridge(BRIDGE_PIN, "vr", pin[1:]));
    else:
        log("Invalid pin format")

    while True:
        data = receive(conn, hdr.size)
        if not data:
            log("Data read timeout")
            sys.exit(1)

        msg_type, msg_id, status = hdr.unpack(data)
        dump("Got: {0}, {1}, {2}".format(msg_type, msg_id, status))

conn.close()
