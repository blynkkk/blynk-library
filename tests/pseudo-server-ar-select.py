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
import select, socket, struct
import os, sys, time, getopt
from threading import Thread

# Configuration options

# Parse command line options
opts, args = getopt.getopt(sys.argv[1:],
    "hb:p:",
    ["help", "bind=", "port=", "sndbuf=", "rcvbuf=", "nodelay", "wait=", "sleep=", "qty=", "dump"])

# Default options
HOST = ''       # Bind to all interfaces
PORT = 8888     # Bind to port 8888
NODELAY = 0     # No TCP_NODELAY
SNDBUF = 0      # No SNDBUF override
RCVBUF = 0      # No RCVBUF override
WAIT = -1       # No select timeout
MSG_QTY = 1000  # Amount of messages
SLEEP = 0       # Wait some time between IO
HW_PIN = 14     # Pin #
DUMP = 0

for o, v in opts:
    if o in ("-h", "--help"):
        usage()
        sys.exit()
    elif o in ("-b", "--bind"):
        HOST = v
    elif o in ("-p", "--port"):
        PORT = int(v)
    elif o in ("--sndbuf",):
        SNDBUF = int(v)
    elif o in ("--rcvbuf",):
        RCVBUF = int(v)
    elif o in ("--nodelay",):
        NODELAY = 1
    elif o in ("--wait",):
        WAIT = float(v)
    elif o in ("--sleep",):
        SLEEP = float(v)
    elif o in ("--qty",):
        MSG_QTY = int(v)
    elif o in ("--pin",):
        HW_PIN = int(v)
    elif o in ("--dump",):
        DUMP = 1


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
    dump("< " + " ".join(map(str, args)))
    # Prepend HW command header
    return hdr.pack(MsgType.HW, 1, len(data)) + data

start_time = time.time()
def log(msg):
    print "[{:7.3f}] {:}".format(float(time.time() - start_time), msg)

def draw(c):
    if not DUMP:
        sys.stdout.write(c)
        sys.stdout.flush()

def dump(msg):
    if DUMP:
        log(msg)

def receive(sock, length):
	d = []
	l = 0
	while l < length:
		r = sock.recv(length-l)
		if not r:
			return ''
		d.append(r)
		l += len(r)
	return ''.join(d)


def clientthread(conn, addr):
    log('Connection from {0}:{1}'.format(addr[0], str(addr[1])))
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

    b = 0
    msgs_out = 0
    msgs_in = 0
    msgs_delta = 0
    msgs_skip = 0
    authenticated = False

    proc_start = time.time()

    while(True):
        if WAIT != -1:
            rs, ws, es = select.select([conn] , [conn], [conn], WAIT)
        else:
            rs, ws, es = select.select([conn] , [conn], [conn])
        if es:
            log("Socket error")
            break;
        if rs:
            data = receive(conn, hdr.size)
            if not data:
                break
            msg_type, msg_id, msg_len = hdr.unpack(data)
            #dump("Got {0}, {1}, {2}".format(msg_type, msg_id, msg_len))
            if msg_type == MsgType.RSP:
                pass
            elif msg_type == MsgType.LOGIN:
                auth = receive(conn, msg_len)
                log("Auth {0}".format(auth))
                # Send auth OK and pin modes
                conn.sendall(hdr.pack(MsgType.RSP, msg_id, MsgStatus.OK))
                conn.sendall(hw("pm", HW_PIN, "in"))
                authenticated = True
            elif msg_type == MsgType.PING:
                log("Ping")
                # Send Pong
                conn.sendall(hdr.pack(MsgType.RSP, msg_id, MsgStatus.OK))
            elif msg_type == MsgType.HW:
                data = receive(conn, msg_len)
                # Print HW messages (just for fun :)
                draw('v')
                dump("> " + " ".join(data.split("\0")))
                msgs_in += 1
                if msgs_out >= MSG_QTY:
                    msgs_delta += 1
            else:
                log("Unknown msg type")
                break
        if ws:
            if authenticated and msgs_out < MSG_QTY:
                conn.sendall(hw("ar", HW_PIN))
                draw('.')
                b = (b + 1) % 256
                msgs_out += 1
            elif msgs_in >= MSG_QTY:
                break
        else:
            msgs_skip += 1

        time.sleep(SLEEP)

    draw("\n")
    log("Time %3.4f" % (time.time() - proc_start))
    log("Sent {0} messages".format(msgs_out))
    log("Recv {0} messages (delta: {1}, skip: {2})".format(msgs_in, msgs_delta, msgs_skip))
    conn.close()

# Main code

serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    # Set SO_REUSEADDR, this is needed to ignore WAIT state on next run
    serv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    serv.bind((HOST, PORT))
except socket.error as msg:
    log('Bind failed. Error Code: {0}, Msg: {1}'.format(str(msg[0]), msg[1]))
    sys.exit()

serv.listen(10)
log('Listening on port %d' % PORT)

# Wait for clients
#while True:
thread = Thread(target=clientthread, args=serv.accept())
thread.start()
serv.close()
thread.join()
