#!/usr/bin/python
'''
 Examples:
   python blynk_ctrl.py --token=b168... write D0 1
   python blynk_ctrl.py --token=b168... write A0 123
   python blynk_ctrl.py --token=b168... write V0 "some value"
   
 Note: read is not supported yet

 Author:   Volodymyr Shymanskyy
 License:  The MIT license
'''
import socket, struct
import sys, time
import argparse

parser = argparse.ArgumentParser(
    #usage = '%prog [args] <command> <pin> [<value>...]',
    description = 'This script uses Bridge feature to control another device from the command line.',
    epilog = __doc__
)

parser.add_argument('-s', '--server', action='store',      dest='server',           help='server address or domain name')
parser.add_argument('-p', '--port',   action="store",      dest='port', type=int, help='server port')
parser.add_argument('-t', '--token',  action="store",      dest='token',            help='auth token of the controller')
parser.add_argument('--target',       action="store",      dest='target', metavar="TOKEN", help='auth token of the target device')
parser.add_argument('--dump',         action="store_true", dest='dump',             help='dump communication')
parser.add_argument('command',        help='operation to perform')
parser.add_argument('pin',            help='pin name')
parser.add_argument('value',          nargs='*', help='value')

parser.set_defaults(
    server='cloud.blynk.cc',
    port=8442,
    dump=False,
    nodelay=True,
    bridge=64
)

args = parser.parse_args()

if not args.target and args.token:
    args.target = args.token

if not args.token:
    parser.error("token not specified!")

# Helpers
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
    dump("< " + "=".join(map(str, args)))
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
    print >>sys.stderr, "[{:7.3f}] {:}".format(float(time.time() - start_time), msg)

def dump(msg):
    if args.dump:
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
    conn = socket.create_connection((args.server, args.port), 3)
except:
    log("Can't connect to %s:%d" % (args.server, args.port))
    sys.exit(1)

if args.nodelay:
    conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    
# Authenticate
conn.sendall(hdr.pack(MsgType.LOGIN, 1, len(args.token)))
conn.sendall(args.token)
data = receive(conn, hdr.size)
if not data:
    log("Login timeout")
    sys.exit(1)

msg_type, msg_id, status = hdr.unpack(data)
if MsgType.RSP != 0 or msg_id != 1 or status != MsgStatus.OK:
    log("Login failed: {0}, {1}, {2}".format(msg_type, msg_id, status))
    sys.exit(1)

conn.sendall(bridge(args.bridge, "i", args.target));

pin = args.pin

if args.command == 'write' or args.command == 'set':
    if not args.value:
        parser.error("value not specified!")
    val = args.value
    if pin[0] == 'D' or pin[0] == 'd':
        conn.sendall(bridge(args.bridge, "dw", pin[1:], *val));
    elif pin[0] == 'A' or pin[0] == 'a':
        conn.sendall(bridge(args.bridge, "aw", 'A'+pin[1:], *val));
    elif pin[0] == 'V' or pin[0] == 'v':
        conn.sendall(bridge(args.bridge, "vw", pin[1:], *val));
    else:
        log("Invalid pin format")
elif args.command == 'read' or args.command == 'get':
    if pin[0] == 'D' or pin[0] == 'd':
        conn.sendall(bridge(args.bridge, "dr", pin[1:]));
    elif pin[0] == 'A' or pin[0] == 'a':
        conn.sendall(bridge(args.bridge, "ar", 'A'+pin[1:]));
    elif pin[0] == 'V' or pin[0] == 'v':
        conn.sendall(bridge(args.bridge, "vr", pin[1:]));
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
