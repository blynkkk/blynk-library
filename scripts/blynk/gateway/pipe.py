from __future__ import print_function
import sys
import threading

__author__    = "Volodymyr Shymanskyy"
__copyright__ = "Copyright (c) 2015 Volodymyr Shymanskyy"
__license__   = "MIT"
__status__    = "Prototype"

if (sys.version_info > (3, 0)):
    def hexdump(s):
        return ":".join("{:02x}".format(c) for c in s)
else:
    def hexdump(s):
        return ":".join("{:02x}".format(ord(c)) for c in s)

class BlynkPipe:
    def __init__(self, s1, s2):
        self.s1 = s1
        self.s2 = s2

    def start(self):
        self.s1.open()
        self.s2.open()

        print("Starting pipe...")
        self.alive = True
        self.rt = threading.Thread(target=self.reader)
        self.rt.setDaemon(1)
        self.rt.start()

        self.wt = threading.Thread(target=self.writer)
        self.wt.setDaemon(1)
        self.wt.start()
        
    def stop(self):
        print("Stopping pipe...")
        self.alive = False
        self.s1.close()
        self.s2.close()
        self.wait()

    def wait(self):
        self.wt.join()
        self.rt.join()

    def reader(self):
        while self.alive:
            try:
                d = self.s1.read(1024)
                if len(d):
                    #print("1>2", hexdump(d))
                    self.s2.write(d)
            except Exception as e:
                print(e)
                break
        self.alive = False

    def writer(self):
        while self.alive:
            try:
                d = self.s2.read(1024)
                if len(d):
                    #print("2>1", hexdump(d))
                    self.s1.write(d)
            except Exception as e:
                print(e)
                break
        self.alive = False
