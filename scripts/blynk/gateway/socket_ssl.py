from __future__ import print_function
import socket
import ssl

from .socket_tcp import BlynkTcpClient

__author__    = "Volodymyr Shymanskyy"
__copyright__ = "Copyright (c) 2015 Volodymyr Shymanskyy"
__license__   = "MIT"
__status__    = "Prototype"

class BlynkSslClient(BlynkTcpClient):
    def __init__(self, host, port, ca_certs=None, certfile=None, keyfile=None, password=None):
        self.host = host
        self.port = port
        self.ca_certs = ca_certs
        self.certfile = certfile
        self.keyfile  = keyfile
        self.password = password

    def open(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        ctx = ssl.SSLContext(ssl.PROTOCOL_TLSv1_2)
        if self.certfile:
            try:
                ctx.load_cert_chain(
                    certfile = self.certfile,
                    keyfile  = self.keyfile,
                    password = self.password
                )
            except:
                print("Could not load certificate (invalid password?)")
        if self.ca_certs:
            ctx.verify_mode = ssl.CERT_REQUIRED
            ctx.load_verify_locations(self.ca_certs)
        
        #ctx.set_ciphers('ECDHE-RSA-AES256-GCM-SHA384')
        ctx.set_ciphers('ECDHE-RSA-AES256-SHA')
        
        self.sock = ctx.wrap_socket(s)
        self.sock.connect((self.host, self.port))
        
        print("Connected to SSL:%s:%d" % (self.host, self.port))
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
