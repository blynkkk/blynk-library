#!/usr/bin/env python

"""USAGE: loopback.py [options]

Options:
  --bind=S      address of server to connect             (default: any)
  --port=N      serial port, a number or a device name   (defualt: 8888)
  --sndbuf=N    SO_SNDBUF override                       (default: no override)
  --rcvbuf=N    SO_RCVBUF override                       (default: no override)
  --nodelay     enable TCP_NODELAY
  --msg=S       message content                          (default: 'message')
  --qty=N       amount of messsages                      (default: 10)
  --sleep=F     delay between sending messages           (default: 1.0)
  --freq=N      messages per second (inverse of sleep)   (default: 1)

 This is a pseudo-server that sends predefined pattern to any connected client.
 It is used to test transport behaviour and throughput.

 Author:   Volodymyr Shymanskyy
 License:  The MIT license
"""

import select, socket, struct
import os, sys, time, getopt
from threading import Thread

# Configuration options

# Parse command line options
try:
    opts, args = getopt.getopt(sys.argv[1:],
        "hb:p:",
        ["help", "bind=", "port=", "sndbuf=", "rcvbuf=", "nodelay", "sleep=", "qty=", "freq=", "msg=", "dump"])
except getopt.GetoptError:
    print >>sys.stderr, __doc__
    sys.exit(2)

# Default options
HOST = ''       # Bind to all interfaces
PORT = 8888     # Bind to port 8888
NODELAY = 0     # No TCP_NODELAY
SNDBUF = 0      # No SNDBUF override
RCVBUF = 0      # No RCVBUF override
MSG_QTY = 10    # Amount of messages
SLEEP = 1.0     # Wait some time between IO
MSG = "message" # Message
DUMP = 0

for o, v in opts:
    if o in ("-h", "--help"):
        print __doc__
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
    elif o in ("--sleep",):
        SLEEP = float(v)
    elif o in ("--freq",):
        SLEEP = 1.0/float(v)
    elif o in ("--qty",):
        MSG_QTY = int(v)
    elif o in ("--msg",):
        MSG = v
    elif o in ("--dump",):
        DUMP = 1

# Print utilities

start_time = time.time()
def log(msg):
    print "[{:7.3f}] {:}".format(float(time.time() - start_time), msg)

draw_col = 0
def draw(c):
    global draw_col
    if not DUMP:
        sys.stdout.write(c)
        draw_col = (draw_col + 1) % 120
        if draw_col:
            sys.stdout.flush()
        else:
            sys.stdout.write("\n")

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

# Threads

def readthread(conn, addr):
	global msgs_in
	while(msgs_in < MSG_QTY):
		data = receive(conn, len(MSG))
		if data != MSG:
			log("Data is wrong:" + data)
			#break
		draw('v')
		dump("> " + data)
		msgs_in += 1


def writethread(conn, addr):
    global msgs_out
    while (msgs_out < MSG_QTY):
        conn.sendall(MSG)
        draw('.')
        dump("< " + MSG)
        msgs_out += 1
        time.sleep(SLEEP)


# Main code

serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    # Set SO_REUSEADDR, this is needed to ignore WAIT state on next run
    serv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    serv.bind((HOST, PORT))
except socket.error as msg:
    log('Bind failed. Error Code: {0}, Msg: {1}'.format(str(msg[0]), msg[1]))
    sys.exit()

serv.listen(1)
log('Listening on port %d' % PORT)

# Wait for clients
#while True:
conn, addr = serv.accept()
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

proc_start = time.time()
msgs_in = 0
msgs_out = 0

wt = Thread(target=readthread,  args=(conn, addr))
rt = Thread(target=writethread, args=(conn, addr))

wt.start()
rt.start()

wt.join()
rt.join()

draw("\n")
log("Time %3.4f" % (time.time() - proc_start))
log("Sent {0} messages".format(msgs_out))
log("Recv {0} messages".format(msgs_in))
conn.close()
