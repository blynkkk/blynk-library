import time
import serial

__author__    = "Volodymyr Shymanskyy"
__copyright__ = "Copyright (c) 2015 Volodymyr Shymanskyy"
__license__   = "MIT"
__status__    = "Prototype"

class BlynkSerial:
    def __init__(self, port=0, baud=9600):
        self.ser = serial.Serial()
        self.ser.port     = port
        self.ser.baudrate = baud
        self.ser.rtscts   = False
        self.ser.xonxoff  = False
        self.ser.timeout  = 1     #required so that the reader thread can exit
      
    def open(self):
        self.ser.open()
        
        # Reset Arduino
        self.ser.setDTR(0)
        time.sleep(0.1)
        self.ser.flushInput()
        self.ser.setDTR(1)
        print("Connected to SER:%s" % (self.ser.port))
      
    def close(self):
        self.ser.close()
    def write(self, data):
        self.ser.write(data)
    def read(self, qty):
        return self.ser.read(qty)
