#!/usr/bin/env python

import os, sys, fnmatch
from subprocess import call

def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename

metadata = {
  "Arduino_Ethernet.ino"        : { },
  "Arduino_Ethernet_Manual.ino" : { },
  "Arduino_Ethernet2.ino"       : { "skip": True }, # Arduino.org?
  "Seeed_EthernetV2_0.ino"      : { "skip": True }, # TODO: Bug

  "Arduino_Serial_USB.ino"      : { },
  "Arduino_SoftwareSerial.ino"  : { },
  "Arduino_WiFi.ino"            : { },
  "Arduino_WiFi_Shield_101.ino" : { },
  "Arduino_WiFi_Shield_101_SSL.ino" : { },
  "Arduino_Yun.ino"             : { "fqbn": "arduino:avr:yun" },
  "Arduino_Zero_M0_Serial.ino"  : { "fqbn": "arduino:samd:arduino_zero_native" },
  "CC3000.ino"                  : { },
  "ENC28J60.ino"                : { "fqbn": "arduino:avr:nano:cpu=atmega328" },
  "ESP8266_Shield_HardSer.ino"  : { "fqbn": "arduino:avr:mega:cpu=atmega2560" },
  "ESP8266_Shield_SoftSer.ino"  : { },
  "ESP8266_DirectConnect.ino"   : { "fqbn": "esp8266:esp8266:nodemcuv2" },
  "ESP8266_Standalone.ino"      : { "fqbn": "esp8266:esp8266:nodemcuv2" },
  "ESP8266_Standalone_SSL.ino"  : { "fqbn": "esp8266:esp8266:nodemcuv2" },
  "Intel_Edison_WiFi.ino"       : { "fqbn": "Intel:i686:izmir_ec" },
  "RN_XV_WiFly.ino"             : { "fqbn": "arduino:avr:leonardo" },
  "RedBear_Duo_WiFi.ino"        : { "fqbn": "RedBear:STM32F2:RedBear_Duo" },
  "TinyDuino_WiFi.ino"          : { "fqbn": "arduino:avr:pro:cpu=8MHzatmega328" },
  "User_Defined_Connection.ino" : { },
  "WildFire.ino"                : { "fqbn": "WickedDevice:avr:wildfireo3" },

  # No linux support
  "LightBlueBeanRGB.ino"        : { "skip": True },
  "LinkItONE.ino"               : { "skip": True },

  # Energia
  "Energia_WiFi.ino"            : { "skip": True },
  "RedBearLab_CC3200.ino"       : { "skip": True },
  "RedBearLab_WiFi_Mini.ino"    : { "skip": True },
  "TI_CC3200_LaunchXL.ino"      : { "skip": True },
  "TI_Stellaris_LaunchPad.ino"  : { "skip": True },
  "TI_TivaC_Connected.ino"      : { "skip": True },
}

examples = find_files('examples', '*.ino')
abs_examples = map(lambda x: os.path.abspath(x), examples)

os.chdir("/data/arduino-1.6.7/")
'''
builder = "./arduino-builder"

builder_args = [
  "-verbose",

  "-hardware", "./hardware",
  "-hardware", os.path.expanduser("~/.arduino15/packages/"),
  "-hardware", "/data2/sketchbook/hardware",

  "-tools",    "./hardware/tools",
  "-tools",    os.path.expanduser("~/.arduino15/packages/"),
  "-tools",    "./tools-builder",

  "-libraries",os.path.expanduser("~/.arduino15/packages/"),
  "-libraries","./libraries",
  "-libraries","/data2/sketchbook/libraries/",
]
'''

failed = []
built = []

for fn in abs_examples:
    path, ino = os.path.split(fn)
    print 'Building example:', ino
    if ino in metadata:
        m = metadata[ino]
        if "skip" in m:
            continue
        if not "fqbn" in m:
            m["fqbn"] = "arduino:avr:uno"
    else:
        #continue
        m = { "fqbn": "arduino:avr:uno" }
    
    #cmd = [builder] + builder_args + ["-fqbn", m["fqbn"]] + [fn]
    cmd = [
        "./arduino",
        #"--verbose",
        "--verify",
        "--board", m["fqbn"],
        fn
    ]
    #print cmd
    rc = call(cmd)
    if rc:
        failed.append(ino)
    else:
        built.append(ino)

print "=================="
if len(failed):
    print " Failed:", failed
    sys.exit(1)
else:    
    print " All", len(built), "examples OK"
