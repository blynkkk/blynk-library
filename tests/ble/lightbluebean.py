#!/usr/bin/env python
from __future__ import print_function

#sudo hciconfig hci0 reset
#sudo hciconfig hci0 up


from bluepy.btle import UUID, Peripheral, DefaultDelegate

import struct
import os, sys, time, getopt
#from threading import Thread

def hexdump(s):
    return ":".join("{:02x}".format(ord(c)) for c in s)

def _LBN_UUID(val):
    return UUID("%08X-c5b1-4b44-b512-1370f02d74de" % (0xa495ff00+val))

def crc16(data, crc=0):
    msb = crc >> 8
    lsb = crc & 255
    for c in data:
        x = ord(c) ^ msb
        x ^= (x >> 4)
        msb = (lsb ^ (x >> 3) ^ (x << 4)) & 255
        lsb = (x ^ (x << 5)) & 255
    return (msb << 8) + lsb

class LightBlueBean(DefaultDelegate):
    # https://github.com/PunchThrough/bean-documentation/blob/master/app_message_types.md
    MSG_ID_SERIAL_DATA        = 0x0000
    MSG_ID_BT_SET_ADV         = 0x0500
    MSG_ID_BT_SET_CONN        = 0x0502
    MSG_ID_BT_SET_LOCAL_NAME  = 0x0504
    MSG_ID_BT_SET_PIN         = 0x0506
    MSG_ID_BT_SET_TX_PWR      = 0x0508
    MSG_ID_BT_GET_CONFIG      = 0x0510
    MSG_ID_BT_ADV_ONOFF       = 0x0512
    MSG_ID_BT_SET_SCRATCH     = 0x0514
    MSG_ID_BT_GET_SCRATCH     = 0x0515
    MSG_ID_BT_RESTART         = 0x0520
    MSG_ID_GATING             = 0x0550
    MSG_ID_BL_CMD             = 0x1000
    MSG_ID_BL_FW_BLOCK        = 0x1001
    MSG_ID_BL_STATUS          = 0x1002
    MSG_ID_CC_LED_WRITE       = 0x2000
    MSG_ID_CC_LED_WRITE_ALL   = 0x2001
    MSG_ID_CC_LED_READ_ALL    = 0x2002
    MSG_ID_CC_LED_DATA        = 0x2082
    MSG_ID_CC_ACCEL_READ      = 0x2010
    MSG_ID_CC_ACCEL_DATA      = 0x2090
    MSG_ID_CC_TEMP_READ       = 0x2011
    MSG_ID_CC_TEMP_DATA       = 0x2091
    MSG_ID_CC_BATT_READ       = 0x2015
    MSG_ID_CC_BATT_DATA       = 0x2095
    MSG_ID_AR_SET_POWER       = 0x3000
    MSG_ID_AR_GET_CONFIG      = 0x3006
    MSG_ID_DB_LOOPBACK        = 0xFE00
    MSG_ID_DB_COUNTER         = 0xFE01
    
    def __init__(self, mac):
        self.conn = Peripheral(mac)
        self.conn.setDelegate(self)
        self.count = 0
        print('connected')
        
        self.service = self.conn.getServiceByUUID(_LBN_UUID(0x10))
        self.serial = self.service.getCharacteristics(_LBN_UUID(0x11)) [0]
        
        #print(self.serial.propertiesToString())

        # Turn on notificiations
        self.conn.writeCharacteristic(0x2f, '\x01\x00', False)
        
        i = 0
        while True:
            #print(self.serial.read())
            self.write("a" * 60)
            #self.write("a" * 5)
            #self.sendCmd(LightBlueBean.MSG_ID_CC_ACCEL_READ)
            
            self.conn.waitForNotifications(1)
            time.sleep(1)
        self.conn.disconnect()
        
    def write(self, data):
        self.sendCmd(LightBlueBean.MSG_ID_SERIAL_DATA, data)

    def sendCmd(self, cmd, data = ""):
        # https://github.com/PunchThrough/bean-documentation/blob/master/serial_message_protocol.md
        gst = struct.pack("!BxH", len(data)+2, cmd) + data
        crc = struct.pack("<H", crc16(gst, 0xFFFF))
        gst += crc
        
        gt_qty = len(gst)/19
        if len(gst) % 19 != 0:
            gt_qty += 1
        
        #amnt = len(gst) / gt_qty
        optimal_packet_size = 19
        
        for ch in xrange(0, gt_qty):
            data = gst[:optimal_packet_size]
            gst = gst[optimal_packet_size:]
            
            gt = 0
            if ch == 0:
                gt = 0x80
            gt |= self.count << 5
            gt |= gt_qty - ch - 1
            
            gt = struct.pack("B", gt) + data
        
            #print("<", hexdump(gt))
            self.serial.write(gt)
            #time.sleep(0.1)
        
        self.count = (self.count + 1) % 4

    def writeRaw(self, data):
        self.conn.writeCharacteristic(0x0b, data, False)

    def handleNotification(self, cHandle, data):
        #print(">", hexdump(data))
        gt = struct.unpack("B", data[0]) [0]
        gt_frst = gt & 0x80
        gt_cntr = gt & 0x60
        gt_left = gt & 0x1F
        
        if gt_frst:
            self.buffin = ""
            
        self.buffin += data[1:]
        
        if gt_left == 0:
            crc_ = crc16(self.buffin[:-2], 0xFFFF)
            dlen, cmd = struct.unpack("!BxH", self.buffin[:4])
            crc = struct.unpack("<H", self.buffin[-2:]) [0]
            if crc != crc_:
                print("CRC check failure")
                return
            print(self.buffin[4:-2])

if __name__ == '__main__':
    LightBlueBean('C4:BE:84:E5:20:60')
