#!/usr/bin/env python
from __future__ import print_function

import time
import blynk.gateway
from blynk.gateway.uart import BlynkSerial
from blynk.gateway.socket_tcp import BlynkTcpClient
from blynk.gateway.socket_ssl import BlynkSslClient
from blynk.gateway.pipe import BlynkPipe

ser = BlynkSerial('/dev/ttyUSB1', 9600)

# SSL:
cln = BlynkSslClient('cloud.blynk.cc', 8441, './certs/server.crt')

# TCP:
#cln = BlynkTcpClient('cloud.blynk.cc', 8442)

# 2WAY SSL:
#cln = BlynkSslClient('cloud.blynk.cc', 8443, './certs/server.crt','./certs/client.crt','./certs/client.pem','password')

p = BlynkPipe(ser, cln)
p.start()
try:
    print("Press Ctrl+C to quit.")
    while (True):
        time.sleep(10)
except KeyboardInterrupt:
    pass
p.stop()
print("Finished.")
