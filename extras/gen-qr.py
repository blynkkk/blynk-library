# coding: utf-8
import sys
import pyqrcode
import gzip, zlib
import base64
import json
import re

#data = sys.argv[1]

data = '''{"boardType":"Generic Board","createdAt":1453325389643,"id":17,"isActive":false,"isShared":false,"keepScreenOn":false,"name":"webpage","updatedAt":1453326512172,"widgets":[{"color":616861439,"pushMode":false,"pin":1,"pinType":"VIRTUAL","height":2,"id":1,"label":"","type":"BUTTON","value":"0","width":2,"x":0,"y":0},{"color":-26804225,"splitMode":false,"pins":[{"max":255,"min":0,"pin":0,"pinType":"VIRTUAL","pwmMode":false,"rangeMappingOn":false},{"max":255,"min":0,"pin":0,"pinType":"VIRTUAL","pwmMode":false,"rangeMappingOn":false},{"max":255,"min":0,"pin":0,"pinType":"VIRTUAL","pwmMode":false,"rangeMappingOn":false}],"height":3,"id":2,"type":"RGB","value":"-104705","width":4,"x":2,"y":0},{"notifyWhenOffline":false,"priority":"high","height":1,"id":3,"type":"NOTIFICATION","width":2,"x":0,"y":2}]}'''

# Compact JSON
#data = json.loads(data)
#data = json.dumps(data, separators=(',',':'))

# Compress
data = str.encode(data, 'utf-8')

data = zlib.compress(data)
#data = gzip.compress(data)
data = base64.b64encode(data)
data = bytes.decode(data)

# Add header
data = 'bp1' + data

# Generate QR
url = pyqrcode.create(data, error='M', version=None, mode='binary')
data = url.text(quiet_zone=4)
data = data.replace("0","  ")
data = data.replace("1","██")

# Clean trailing spaces
data = re.sub(r'\s+$', '', data, 0, re.M)

print(data)
#print('\x1b[6;30;47m' + data + '\x1b[0m')
