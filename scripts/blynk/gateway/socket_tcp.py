import socket

__author__    = "Volodymyr Shymanskyy"
__copyright__ = "Copyright (c) 2015 Volodymyr Shymanskyy"
__license__   = "MIT"
__status__    = "Prototype"

class BlynkTcpClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port

    def open(self):
        self.sock = socket.create_connection((self.host, self.port))
        print("Connected to TCP:%s:%d" % (self.host, self.port))
      
    def close(self):
        self.sock.close()

    def write(self, data):
        self.sock.sendall(data)
        
    def read(self, qty):
        try:
            d = self.sock.recv(qty)
        except socket.timeout:
            return ''
        return d

    def read2(self, qty):
        d = []
        l = 0
        while l < qty:
            r = ''
            try:
                r = self.sock.recv(qty-l)
            except socket.timeout:
                continue
            if not r:
                return ''
            d.append(r)
            l += len(r)
        return ''.join(d)
