#!/usr/bin/python
'''
examples:
  python blynk_ctrl.py --token=909fa1...1a9774 digitalWrite 10 1
  python blynk_ctrl.py --token=909fa1...1a9774 digitalWrite A0 1
  python blynk_ctrl.py --token=909fa1...1a9774  analogWrite 9 123
  python blynk_ctrl.py --token=909fa1...1a9774 virtualWrite 5 "some value"

note:
  read is not supported yet

 Author:   Volodymyr Shymanskyy
 License:  The MIT license
'''
import socket, struct
import sys, time
import argparse
import logging

parser = argparse.ArgumentParser(
    formatter_class=argparse.RawTextHelpFormatter,
    description = 'This script uses Bridge feature to control another device from the command line.',
    epilog = __doc__
)

parser.add_argument('-s', '--server', action='store',      dest='server',           help='server address or domain name')
parser.add_argument('-p', '--port',   action="store",      dest='port',   type=int, help='server port')
parser.add_argument('-t', '--token',  action="store",      dest='token',            help='auth token of the controller')
parser.add_argument('--target',       action="store",      dest='target', metavar="TOKEN", help='auth token of the target device')
parser.add_argument('--dump',         action="store_true", dest='dump',             help='dump communication')
parser.add_argument('command',                                                      help='operation to perform')
parser.add_argument('pin',                                                          help='pin name')
parser.add_argument('value',          nargs='*',                                    help='value')

parser.set_defaults(
    server='cloud.blynk.cc',
    port=8442,
    dump=False,
    nodelay=True,
    bridge=64
)

args = parser.parse_args()

logging.basicConfig(level=logging.INFO)
log = logging.getLogger("blynk_ctrl")

if not args.target and args.token:
    args.target = args.token

if not args.token:
    parser.error("token not specified!")
    
if args.dump:
    log.setLevel(logging.DEBUG)

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

def compose(msg_type, *args):
    # Convert params to string and join using \0
    data = "\0".join(map(str, args))
    msg_id = genMsgId()
    msg_len = len(data)
    log.debug("< %2d,%2d,%2d : %s", msg_type, msg_id, msg_len, "=".join(map(str, args)))
    return hdr.pack(msg_type, msg_id, msg_len) + data

static_msg_id = 1
def genMsgId():
    global static_msg_id
    static_msg_id += 1
    return static_msg_id

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
    log.error("Can't connect to %s:%d", args.server, args.port)
    sys.exit(1)

if args.nodelay:
    conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    
# Authenticate
conn.sendall(compose(MsgType.LOGIN, args.token))
data = receive(conn, hdr.size)
if not data:
    log.error("Login timeout")
    sys.exit(1)

msg_type, msg_id, msg_status = hdr.unpack(data)
if msg_type != MsgType.RSP or msg_status != MsgStatus.OK:
    log.error("Login failed: %d,%d,%d", msg_type, msg_id, msg_status)
    sys.exit(1)

conn.sendall(compose(MsgType.BRIDGE, args.bridge, "i", args.target));

if args.command.endswith('Write'):
    if not args.value:
        parser.error("value not specified!")

cmd = None
if args.command == 'digitalWrite':
    cmd = compose(MsgType.BRIDGE, args.bridge, "dw", args.pin, *args.value)
elif args.command == 'analogWrite':
    cmd = compose(MsgType.BRIDGE, args.bridge, "aw", args.pin, *args.value)
elif args.command == 'virtualWrite':
    cmd = compose(MsgType.BRIDGE, args.bridge, "vw", args.pin, *args.value)
elif args.command == 'digitalRead':
    cmd = compose(MsgType.BRIDGE, args.bridge, "dr", args.pin)
elif args.command == 'analogRead':
    cmd = compose(MsgType.BRIDGE, args.bridge, "ar", args.pin)
elif args.command == 'virtualRead':
    cmd = compose(MsgType.BRIDGE, args.bridge, "vr", args.pin)

if not cmd:
    parser.error("command not recognized!")

conn.sendall(cmd)

if args.command.endswith('Read'):
    while True:
        data = receive(conn, hdr.size)
        if not data:
            log.warning("Data read timeout")
            sys.exit(1)

        msg_type, msg_id, msg_len = hdr.unpack(data)
        if (msg_type == MsgType.RSP):
            log.debug("> %2d,%2d    : status %2d", msg_type, msg_id, msg_len)
        else:
            data = receive(conn, msg_len)
            log.debug("> %2d,%2d,%2d : %s", msg_type, msg_id, msg_len, "=".join(data.split("\0")))

conn.close()
