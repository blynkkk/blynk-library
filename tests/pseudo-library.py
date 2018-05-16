#!/usr/bin/python
'''
 This is a pseudo-library implementation
 
 Example:
   ./pseudo-library.py -t b168ccc8c8734fad98323247afbc1113 --dump

 Author:   Volodymyr Shymanskyy
 License:  The MIT license
'''
import select, socket, struct
import os, sys, time, getopt
from threading import Thread

# Configuration options

# Parse command line options
try:
    opts, args = getopt.getopt(sys.argv[1:],
        "hs:p:t:",
        ["help", "server=", "port=", "token=", "sndbuf=", "rcvbuf=", "nodelay=", "dump"])
except getopt.GetoptError:
    print >>sys.stderr, __doc__
    sys.exit(2)

# Default options
SERVER = "blynk-cloud.com"
PORT = 80
NODELAY = 1     # TCP_NODELAY
SNDBUF = 0      # No SNDBUF override
RCVBUF = 0      # No RCVBUF override
TOKEN = "YourAuthToken"
DUMP = 0

for o, v in opts:
    if o in ("-h", "--help"):
        print __doc__
        sys.exit()
    elif o in ("-s", "--server"):
        SERVER = v
    elif o in ("-p", "--port"):
        PORT = int(v)
    elif o in ("-t", "--token"):
        TOKEN = v
    elif o in ("--sndbuf",):
        SNDBUF = int(v)
    elif o in ("--rcvbuf",):
        RCVBUF = int(v)
    elif o in ("--nodelay",):
        NODELAY = int(v)
    elif o in ("--dump",):
        DUMP = 1


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

def hw(*args):
    # Convert params to string and join using \0
    data = "\0".join(map(str, args))
    dump("< " + " ".join(map(str, args)))
    # Prepend HW command header
    return hdr.pack(MsgType.HW, genMsgId(), len(data)) + data

def handle_hw(data):
    params = data.split("\0")
    cmd = params.pop(0)
    if cmd == 'info':
        pass

    ### DIRECT pin operations
    elif cmd == 'pm':
        pairs = zip(params[0::2], params[1::2])
        for (pin, mode) in pairs:
            pin = int(pin)
            if mode == 'in':
                log("Pin %d mode INPUT" % pin)
            elif mode == 'out':
                log("Pin %d mode OUTPUT" % pin)
            elif mode == 'pu':
                log("Pin %d mode INPUT_PULLUP" % pin)
            elif mode == 'pd':
                log("Pin %d mode INPUT_PULLDOWN" % pin)
            else:
                log("Unknown pin %d mode: %s" % (pin, mode))

    elif cmd == 'dw':
        pin = int(params.pop(0))
        val = params.pop(0)
        log("Digital write pin %d, value %s" % (pin, val))
        
    elif cmd == 'aw':
        pin = int(params.pop(0))
        val = params.pop(0)
        log("Analog write pin %d, value %s" % (pin, val))
        
    elif cmd == 'dr':           # This should read digital pin
        pin = int(params.pop(0))
        log("Digital read pin %d" % pin)
        conn.sendall(hw("dw", pin, 1)) # Send value

    elif cmd == 'ar':           # This should do ADC read
        pin = int(params.pop(0))
        log("Analog read pin %d" % pin)
        conn.sendall(hw("aw", pin, 123)) # Send value

    ### VIRTUAL pin operations
    elif cmd == 'vw':           # This should call user handler
        pin = int(params.pop(0))
        val = params.pop(0)
        log("Virtual write pin %d, value %s" % (pin, val))
        
    elif cmd == 'vr':           # This should call user handler
        pin = int(params.pop(0))
        log("Virtual read pin %d" % pin)
        conn.sendall(hw("vw", pin, "hello")) # Send value
        
    else:
        log("Unknown HW cmd: %s" % cmd)

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

# Threads

def readthread(conn):
    while (True):
        data = receive(conn, hdr.size)
        if not data:
            break
        msg_type, msg_id, msg_len = hdr.unpack(data)
        dump("Got {0}, {1}, {2}".format(msg_type, msg_id, msg_len))
        if msg_type == MsgType.RSP:
            pass
        elif msg_type == MsgType.PING:
            log("Got ping")
            # Send Pong
            conn.sendall(hdr.pack(MsgType.RSP, msg_id, MsgStatus.OK))
        elif msg_type == MsgType.HW or msg_type == MsgType.BRIDGE:
            data = receive(conn, msg_len)
            # Print HW message
            dump("> " + " ".join(data.split("\0")))
            handle_hw(data)
        else:
            log("Unknown msg type")
            break


def writethread(conn):
    while (True):
        time.sleep(10)
        log("Sending heartbeat...")
        conn.sendall(hdr.pack(MsgType.PING, genMsgId(), 0))

# Main code
log('Connecting to %s:%d' % (SERVER, PORT))
try:
    conn = socket.create_connection((SERVER, PORT), 3)
except:
    log("Can't connect")
    sys.exit(1)

if NODELAY != 0:
    conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
if SNDBUF != 0:
    sndbuf = conn.getsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF)
    log('Default SNDBUF %s changed to %s' % (sndbuf, SNDBUF))
    conn.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, SNDBUF)
if RCVBUF != 0:
    rcvbuf = conn.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF)
    log('Default RCVBUF %s changed to %s' % (rcvbuf, RCVBUF))
    conn.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, RCVBUF)
    
# Authenticate
conn.sendall(hdr.pack(MsgType.LOGIN, genMsgId(), len(TOKEN)))
conn.sendall(TOKEN)
data = receive(conn, hdr.size)
if not data:
    log("Auth timeout")
    sys.exit(1)

msg_type, msg_id, status = hdr.unpack(data)
dump("Got {0}, {1}, {2}".format(msg_type, msg_id, status))

if status != MsgStatus.OK:
    log("Auth failed: %d" % status)
    sys.exit(1)

wt = Thread(target=readthread,  args=(conn,))
rt = Thread(target=writethread, args=(conn,))

wt.start()
rt.start()

wt.join()
rt.join()

conn.close()
