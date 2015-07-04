#!/usr/bin/env python
from __future__ import print_function

#sudo hciconfig hci0 reset
#sudo hciconfig hci0 up


#         UART Service UUID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
#    TX Characteristic UUID: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E
#    RX Characteristic UUID: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E



import bluepy.btle as btle
import struct
import os, sys, time, getopt
#from threading import Thread


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
    log("< " + " ".join(map(str, args)))
    # Prepend HW command header
    return hdr.pack(MsgType.HW, 1, len(data)) + data

start_time = time.time()
def log(msg):
    print("[{:7.3f}] {:}".format(float(time.time() - start_time), msg))

class Nrf8001(btle.DefaultDelegate):
    def __init__(self, mac):
        self.conn = btle.Peripheral(mac, 'random')
        self.conn.setDelegate(self)
        print('connected')

        # Turn on notificiations
        self.conn.writeCharacteristic(0x0e, '\x01\x00', False)

        i = 0
        while True:
            #data = str(i)
            #print('<<', data)
            #
            self.conn.waitForNotifications(1)
            self.writeRaw(hw("ar", "A1"))
            #i = i+1

        self.conn.disconnect()
    def writeRaw(self, data):
        self.conn.writeCharacteristic(0x0b, data, False)

    def handleNotification(self, cHandle, data):
        msg_type, msg_id, msg_len = hdr.unpack(data[:5])
        #dump("Got {0}, {1}, {2}".format(msg_type, msg_id, msg_len))
        if msg_type == MsgType.RSP:
            pass
        elif msg_type == MsgType.LOGIN:
            #auth = receive(conn, msg_len)
            #log("Auth {0}".format(auth))
            # Send auth OK and pin modes
            self.writeRaw(hdr.pack(MsgType.RSP, msg_id, MsgStatus.OK))
            self.writeRaw(hw("pm", 1, "in"))
        elif msg_type == MsgType.PING:
            log("Ping")
            # Send Pong
            self.writeRaw(hdr.pack(MsgType.RSP, msg_id, MsgStatus.OK))
        elif msg_type == MsgType.HW:
            data = data[5:]
            #data = receive(conn, msg_len)
            # Print HW messages (just for fun :)
            #draw('v')
            log("> " + " ".join(data.split("\0")))
            #msgs_in += 1
        else:
            log("Unknown msg type")

if __name__ == '__main__':
    Nrf8001('D8:C3:17:46:68:3C')
