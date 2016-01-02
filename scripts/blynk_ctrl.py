#!/usr/bin/python
'''
examples:

  Simple write operations:
    python blynk_ctrl.py --token=909fa1... -dw 5 1
    python blynk_ctrl.py --token=909fa1... -aw 9 134
    python blynk_ctrl.py --token=909fa1... -vw 1 value

  Simple read operations:
    python blynk_ctrl.py --token=909fa1... -vr 3

  Using named pins (like A1, supported by some boards):
    python blynk_ctrl.py --token=909fa1... -dw A1 1

  Multiple operations at once:
    python blynk_ctrl.py --token=909fa1... -aw 9 100 -dw 8 123 -vw 9 hello

  Sending arrays to virtual pins:
    python blynk_ctrl.py --token=909fa1... -vw 1 "value 1" "value 2"

  Do simple animations (delay commands):
    python blynk_ctrl.py --token=909fa1... --delayAll=0.2 -aw 9 0 -aw 9 50 -aw 9 100 -aw 9 150 -aw 9 200 -aw 9 255
    python blynk_ctrl.py --token=909fa1... -aw 9 10 --delay=0.5 -aw 9 50 --delay=2.0 -aw 9 100

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

def opAction(op, expand=False, minargs=1):
    class _action(argparse.Action):
        def __call__(self, parser, namespace, values, option_string=None):
            if len(values) < minargs:
                raise argparse.ArgumentError(self, "not enough parameters")
            
            if expand:
                pin = values[0]
                for v in values[1:]:
                    namespace.ops.append([op, pin, v])
            else:
                namespace.ops.append([op]+values)

    return _action

parser.add_argument('-t', '--token',  action="store",      dest='token',            help='auth token of the controller')

parser.add_argument('-dw', '--digitalWrite', action=opAction('dw', True, 2),  nargs='*', metavar=('PIN', 'VAL'))
parser.add_argument('-aw', '--analogWrite',  action=opAction('aw', True, 2),  nargs='*', metavar=('PIN', 'VAL'))
parser.add_argument('-vw', '--virtualWrite', action=opAction('vw', False, 2), nargs='*', metavar=('PIN', 'VAL'))

parser.add_argument('-dr', '--digitalRead',  action=opAction('dr'), nargs=1,   metavar='PIN')
parser.add_argument('-ar', '--analogRead',   action=opAction('ar'), nargs=1,   metavar='PIN')
parser.add_argument('-vr', '--virtualRead',  action=opAction('vr'), nargs=1,   metavar='PIN')

parser.add_argument('--delay',               action=opAction('delay'), nargs=1, type=float, metavar='SECs')

parser.add_argument('--delayAll',     action="store",      dest='delayAll', type=float, metavar='SECs', help='delay between all operations')

parser.add_argument('-s', '--server', action='store',      dest='server',           help='server address or domain name')
parser.add_argument('-p', '--port',   action="store",      dest='port',   type=int, help='server port')
parser.add_argument('--target',       action="store",      dest='target', metavar="TOKEN", help='auth token of the target device')
parser.add_argument('--dump',         action="store_true", dest='dump',             help='dump communication')

parser.set_defaults(
    server='cloud.blynk.cc',
    port=8442,
    dump=False,
    nodelay=True,
    bridge=64,
    ops = []
)

args = parser.parse_args()

#import pprint
#pprint.pprint(args)
#sys.exit()

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s %(message)s')
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
    HW_SYNC = 16
    HW_INFO = 17
    HW     = 20


class MsgStatus:
    OK     = 200

def compose(msg_type, *args):
    # Convert params to string and join using \0
    data = "\0".join(map(str, args))
    msg_id = genMsgId()
    msg_len = len(data)
    log.debug(" < %2d,%2d,%2d : %s", msg_type, msg_id, msg_len, "=".join(map(str, args)))
    return hdr.pack(msg_type, msg_id, msg_len) + data

static_msg_id = 0
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

def do_read(cmd, pin):
    conn.sendall(compose(MsgType.HW_SYNC, cmd, pin))
    while True:
        data = receive(conn, hdr.size)
        if not data:
            log.warning("Data read timeout")
            sys.exit(1)

        msg_type, msg_id, msg_len = hdr.unpack(data)
        if msg_type == MsgType.RSP:
            log.debug(" > %2d,%2d    : status %2d", msg_type, msg_id, msg_len)
        elif msg_type == MsgType.HW or msg_type == MsgType.BRIDGE:
            data = receive(conn, msg_len).split("\0")
            log.debug(" > %2d,%2d,%2d : %s", msg_type, msg_id, msg_len, "=".join(data))
            if data[0] == cmd[0]+'w' and data[1] == pin:
                data = data[2:]
                if len(data) > 1:
                    print data
                else:
                    print data[0]
                break

for op in args.ops:
    cmd = op[0]
    op = op[1:]
    if cmd == 'dw':
        conn.sendall(compose(MsgType.BRIDGE, args.bridge, "i", args.target))
        conn.sendall(compose(MsgType.BRIDGE, args.bridge, "dw", op[0], op[1]))
    elif cmd == 'aw':
        conn.sendall(compose(MsgType.BRIDGE, args.bridge, "i", args.target))
        conn.sendall(compose(MsgType.BRIDGE, args.bridge, "aw", op[0], op[1]))
    elif cmd == 'vw':
        conn.sendall(compose(MsgType.BRIDGE, args.bridge, "i", args.target))
        conn.sendall(compose(MsgType.BRIDGE, args.bridge, "vw", op[0], *op[1:]))
    elif cmd == 'dr' or cmd == 'ar' or cmd == 'vr':
        do_read(cmd, op[0])
    elif cmd == 'delay':
        time.sleep(op[0])
    else:
        log.warning("Wrong command:", cmd)

    if args.delayAll:
        time.sleep(args.delayAll)

# Finished

conn.close()
