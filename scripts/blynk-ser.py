#!/usr/bin/env python

"""USAGE: blynk-ser.py [options]

Options:
      --server      address of server to connect           (default cloud.blynk.cc:8442)
  -p, --port=PORT   serial port, a number or a device name (defualt 0)
  -b, --baud=BAUD   baudrate                               (default 9600)
  -r, --rtscts      enable RTS/CTS flow control            (default off)
  -x, --xonxoff     enable software flow control           (default off)

Note: no security measures are implemeted. Anyone can remotely connect
to this service over the network.
Only one connection at once is supported. If the connection is terminaed
it waits for the next connect.
"""

import sys, os, time, serial, threading, getopt, socket

try:
    True
except NameError:
    True = 1
    False = 0

class Redirector:
    def __init__(self, serial, socket):
        self.serial = serial
        self.socket = socket

    def shortcut(self):
        """connect the serial port to the tcp port by copying everything
           from one side to the other"""
        self.alive = True
        self.thread_read = threading.Thread(target=self.reader)
        self.thread_read.setDaemon(1)
        self.thread_read.start()
        self.writer()

    def reader(self):
        """loop forever and copy serial->socket"""
        while self.alive:
            try:
                data = self.serial.read(1)              #read one, blocking
                n = self.serial.inWaiting()             #look if there is more
                if n:
                    data = data + self.serial.read(n)   #and get as much as possible
                if data:
                    #print '<',data
                    self.socket.sendall(data)           #send it over TCP
            except socket.error, msg:
                print msg
                #probably got disconnected
                break
        self.alive = False

    def writer(self):
        """loop forever and copy socket->serial"""
        while self.alive:
            try:
                data = self.socket.recv(1024)
                if not data:
                    break
                #print '>',data
                self.serial.write(data)                 #get a bunch of bytes and send them
            except socket.error, msg:
                print msg
                #probably got disconnected
                break
        self.alive = False
        self.thread_read.join()

    def stop(self):
        """Stop copying"""
        if self.alive:
            self.alive = False
            self.thread_read.join()

if __name__ == '__main__':
    ser = serial.Serial()

    #parse command line options
    try:
        opts, args = getopt.getopt(sys.argv[1:],
                "hp:b:rxs",
                ["help", "port=", "baud=", "rtscts", "xonxoff", "server="])
    except getopt.GetoptError:
        # print help information and exit:
        print >>sys.stderr, __doc__
        sys.exit(2)

    server      = 'cloud.blynk.cc:8442'
    ser.port    = 0
    ser.baudrate = 9600
    ser.rtscts  = False
    ser.xonxoff = False
    ser.timeout = 1     #required so that the reader thread can exit

    for o, a in opts:
        if o in ("-h", "--help"):   #help text
            print __doc__
            sys.exit()
        elif o in ("-p", "--port"):   #specified port
            try:
                ser.port = int(a)
            except ValueError:
                ser.port = a
        elif o in ("-b", "--baud"):   #specified baudrate
            try:
                ser.baudrate = int(a)
            except ValueError:
                raise ValueError, "Baudrate must be a number"
        elif o in ("-r", "--rtscts"):
            ser.rtscts = True
        elif o in ("-x", "--xonxoff"):
            ser.xonxoff = True
        elif o in ("--server"):
            server = a

    try:
        print 'Opening port', ser.port
        ser.open()
    except serial.SerialException, msg:
        #print "Could not open serial port %s: %s" % (ser.portstr, msg)
        sys.exit(1)

    # Reset Arduino
    ser.setDTR(0)
    time.sleep(0.1)
    ser.flushInput()
    ser.setDTR(1)

    print 'Connecting to', server
    while 1:
        try:
            cln = socket.create_connection(server.rsplit(':',2))
            print 'Connected.'
            #enter console->serial loop
            r = Redirector(ser, cln)
            r.shortcut()
            print 'Disconnected.'
            cln.close()
        except socket.error, msg:
            print msg
        time.sleep(2);
        print 'Reconnecting to ', server

    print "Exit."
