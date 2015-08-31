from __future__ import print_function

import threading
import struct
import os, sys, time, getopt
import binascii

from .bluepy.btle import UUID, Peripheral, DefaultDelegate

try:
    from intelhex import IntelHex
except:
    pass

__author__    = "Volodymyr Shymanskyy"
__copyright__ = "Copyright (c) 2015 Volodymyr Shymanskyy"
__license__   = "MIT"
__status__    = "Prototype"

if (sys.version_info > (3, 0)):
    def hexdump(s):
        return ":".join("{:02x}".format(c) for c in s)
      
    def crc16(data, crc=0):
        msb = crc >> 8
        lsb = crc & 255
        for c in data:
            x = c ^ msb
            x ^= (x >> 4)
            msb = (lsb ^ (x >> 3) ^ (x << 4)) & 255
            lsb = (x ^ (x << 5)) & 255
        return (msb << 8) + lsb

else:
    def hexdump(s):
        return ":".join("{:02x}".format(ord(c)) for c in s)
      
    def crc16(data, crc=0):
        msb = crc >> 8
        lsb = crc & 255
        for c in data:
            x = ord(c) ^ msb
            x ^= (x >> 4)
            msb = (lsb ^ (x >> 3) ^ (x << 4)) & 255
            lsb = (x ^ (x << 5)) & 255
        return (msb << 8) + lsb

def chunkstring(string, length):
    return (string[0+i:length+i] for i in range(0, len(string), length))

def _LBN_UUID(val):
    return UUID("%08X-c5b1-4b44-b512-1370f02d74de" % (0xa495ff00+val))



class BlynkLightBlueBean(DefaultDelegate):
    # https://github.com/PunchThrough/bean-documentation/blob/master/app_message_types.md
    # https://bitbucket.org/punchthroughdesign/bean-application-message-definitions/src/572dcf7898b320e4c6ff327c94ae3a90e55c64a9/AppMessages.h?at=master
    MSG_ID_SERIAL_DATA           = 0x0000
    MSG_ID_BT_SET_ADV            = 0x0500
    MSG_ID_BT_SET_CONN           = 0x0502
    MSG_ID_BT_SET_LOCAL_NAME     = 0x0504
    MSG_ID_BT_SET_PIN            = 0x0506
    MSG_ID_BT_SET_TX_PWR         = 0x0508
    MSG_ID_BT_GET_CONFIG         = 0x0510
    MSG_ID_BT_SET_CONFIG         = 0x0511
    MSG_ID_BT_ADV_ONOFF          = 0x0512
    MSG_ID_BT_SET_SCRATCH        = 0x0514
    MSG_ID_BT_GET_SCRATCH        = 0x0515
    MSG_ID_BT_RESTART            = 0x0520
    MSG_ID_BT_DISCONNECT         = 0x0521
    MSG_ID_BT_GET_STATES         = 0x0530
    MSG_ID_BT_SET_CONFIG_NOSAVE  = 0x0540
    MSG_ID_BT_END_GATE           = 0x0550
    MSG_ID_BL_CMD_START          = 0x1000
    MSG_ID_BL_FW_BLOCK           = 0x1001
    MSG_ID_BL_STATUS             = 0x1082
    MSG_ID_BL_GET_META           = 0x1003
    MSG_ID_CC_LED_WRITE          = 0x2000
    MSG_ID_CC_LED_WRITE_ALL      = 0x2001
    MSG_ID_CC_LED_READ_ALL       = 0x2002
    MSG_ID_CC_ACCEL_READ         = 0x2010
    MSG_ID_CC_TEMP_READ          = 0x2011
    MSG_ID_CC_BATT_READ          = 0x2015
    MSG_ID_CC_POWER_ARDUINO      = 0x2020
    MSG_ID_CC_GET_AR_POWER       = 0x2021
    MSG_ID_CC_ACCEL_GET_RANGE    = 0x2030
    MSG_ID_CC_ACCEL_SET_RANGE    = 0x2035
    MSG_ID_CC_ACCEL_WRITE_REG    = 0x2040
    MSG_ID_CC_ACCEL_READ_REG     = 0x2041
    MSG_ID_CC_WAKE_ON_ACCEL      = 0x2050
    MSG_ID_AR_SLEEP              = 0x3000
    MSG_ID_AR_WAKE_ON_CONNECT    = 0x3010
    MSG_ID_ERROR_CC              = 0x4000
    MSG_ID_DB_LOOPBACK           = 0xFE00
    MSG_ID_DB_COUNTER            = 0xFE01
    MSG_ID_DB_E2E_LOOPBACK       = 0xFE02
    MSG_ID_DB_PTM                = 0xFE03
    
    def __init__(self, mac):
        self.mac = mac
        self.conn = None
        self.lock = threading.Lock()

    def open(self):
        if not self.conn:
            print('Connecting to BLE [', self.mac, '] ...')
            self.conn = Peripheral(self.mac)
            self.conn.setDelegate(self)
            self.count = 0
            self.buffin = [None]*10
            self.got1 = False
            self.serialin = b''

            self.service = self.conn.getServiceByUUID(_LBN_UUID(0x10))
            self.serial = self.service.getCharacteristics(_LBN_UUID(0x11)) [0]
            
            # Turn on notificiations
            self.conn.writeCharacteristic(0x2f, b'\x01\x00', False)
            # Connection workaround
            #self.conn.waitForNotifications(20)
            
            self.sendCmd(BlynkLightBlueBean.MSG_ID_BT_END_GATE)
      
    def close(self):
        self.conn.disconnect()

    def write(self, data):
        self.lock.acquire()
        for d in chunkstring(data, 64):
            self.sendCmd(BlynkLightBlueBean.MSG_ID_SERIAL_DATA, d)
        self.lock.release()
    
    def read(self, qty):
        waited = False
        self.lock.acquire()
        if len(self.serialin) == 0:
            self.conn.waitForNotifications(0.1)
            waited = True

        if qty > len(self.serialin):
            qty = len(self.serialin)
        
        d = self.serialin[:qty]
        self.serialin = self.serialin[qty:]
        self.lock.release()
        if waited:
            time.sleep(0.01)
        return d

    def sendCmd(self, cmd, data = ""):
        # https://github.com/PunchThrough/bean-documentation/blob/master/serial_message_protocol.md
        gst = struct.pack("!BxH", len(data)+2, cmd) + data
        crc = struct.pack("<H", crc16(gst, 0xFFFF))
        gst += crc
        
        gt_qty = len(gst) // 19
        if len(gst) % 19 != 0:
            gt_qty += 1
        
        #amnt = len(gst) / gt_qty
        optimal_packet_size = 19
        
        for ch in range(0, gt_qty):
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
        self.conn.writeCharacteristic(0x0b, data.encode('ascii'), False)

    def handleNotification(self, cHandle, data):
        #print(">", hexdump(data))
        gt = struct.unpack("B", data[:1]) [0]
        #gt_cntr = gt & 0x60
        gt_left = gt & 0x1F
        
        if gt & 0x80:
            self.got1 = True
            self.buffin = self.buffin[:gt_left+1]
            
        self.buffin[gt_left] = data[1:]
        
        if self.got1 and not self.buffin.count(None):
            #print("Got ", len(self.buffin), "packets")
            self.buffin.reverse()
            self.buffin = b''.join(self.buffin)
            
            crc_ = crc16(self.buffin[:-2], 0xFFFF)
            dlen, cmd = struct.unpack("!BxH", self.buffin[:4])
            crc = struct.unpack("<H", self.buffin[-2:]) [0]
            if crc == crc_:
                if cmd == BlynkLightBlueBean.MSG_ID_SERIAL_DATA:
                    self.serialin += self.buffin[4:-2]
                elif cmd == BlynkLightBlueBean.MSG_ID_BL_STATUS:
                    self.got_upload_satus = True
                else:
                    print("cmd %d>" % cmd, hexdump(self.buffin[4:-2]))

            else:
                print("CRC check failure")
            
            self.buffin = [None]*10
            self.got1 = False

    def uploadHex(self, hex_file):
        data = IntelHex(hex_file).tobinstr()
        print('Hex size:', len(data))
        chunks = list(chunkstring(data, 64))
        chunks_qty = len(chunks)
        print('Chunks:', len(chunks))

        name = "Hooray"
        
        while len(name) < 20:
            name += " "
        
        self.got_upload_satus = False
        #size:32, crc:32, time:32, namelen:8, name
        self.sendCmd(BlynkLightBlueBean.MSG_ID_BL_CMD_START, struct.pack("<LLLB",
            len(data),
            binascii.crc32(data) & 0xFFFFFFFF,
            int(time.time()),
            len(name)
        ) + name.encode('ascii'))
        
        while not self.got_upload_satus:
            self.conn.waitForNotifications(1)
        
        for i in range(0, chunks_qty):
            self.sendCmd(BlynkLightBlueBean.MSG_ID_BL_FW_BLOCK, chunks[i])
            print('Upload:', (100 * i) // chunks_qty, '%', end='\r')
            self.conn.waitForNotifications(0.1)
