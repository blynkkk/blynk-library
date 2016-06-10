#!/usr/bin/env python

import fnmatch
import os

metadata = {
  "Arduino_Yun.ino"             : { "board": "yun" },
  "Arduino_Zero_M0_Serial.ino"  : { "board": "zero" },
  "Arduino_Due.ino"             : { "board": "due" },
  "ENC28J60.ino"                : { "board": "nanoatmega328" },
  "ESP8266_Shield.ino"          : { "board": "megaatmega2560" },
  "RN_XV_WiFly.ino"             : { "board": "leonardo" },
  "Serial_HM10_HC08.ino"        : { "board": "leonardo" },
  "TheAirBoard_WiFly.ino"       : { "board": "fio" },
  "Adafruit_Feather_32u4_BLE.ino": { "board": "feather32u4" },
  "Seeed_EthernetV2_0.ino"      : { "skip": True }, # Breaks build
  "Arduino_Ethernet2.ino"       : { "skip": True }, # Breaks build, arduino.org

  # ESP8266
  "ESP8266_DirectConnect.ino"   : { "board": "nodemcuv2" },
  "ESP8266_Standalone.ino"      : { "board": "esp01" },
  "ESP8266_Standalone_SmartConfig.ino"  : { "board": "nodemcuv2" },
  "ESP8266_Standalone_SSL.ino"  : { "board": "nodemcuv2" },

  # Digistump
  "Digistump_Digispark.ino"     : { "board": "digispark-pro" },
  "Digistump_Oak.ino"           : { "skip": True },

  # Intel
  "Intel_Edison_WiFi.ino"       : { "skip": True },
  "Intel_Galileo.ino"           : { "skip": True },
  "Arduino_101_BLE.ino"         : { "board": "genuino101" },

  # RBL
  "RedBear_Duo_WiFi.ino"        : { "skip": True },
  "RedBear_Duo_BLE.ino"         : { "skip": True },
  "RedBearLab_BLE_Nano.ino"     : { "board": "redBearLabBLENano" },
  "RedBearLab_BlendMicro.ino"   : { "board": "blendmicro8" },
  "RedBearLab_BLE_Mini.ino"     : { "board": "leonardo" },

  #Other
  "Simblee_BLE.ino"             : { "skip": True },
  "TinyDuino_WiFi.ino"          : { "board": "tinyduino" },
  "WildFire.ino"                : { "board": "wildfirev3" },
  "chipKIT_Uno32.ino"           : { "board": "uno_pic32" },
  "LightBlue_Bean.ino"          : { "board": "lightblue-bean" },
  "Teensy3.ino"                 : { "board": "teensy31" },
  "ATtiny85.ino"                : { "board": "attiny85" },

  # Special examples
  "ESP8266_ReadPin.ino"         : { "board": "nodemcuv2" },
  "ESP8266_WritePin.ino"        : { "board": "nodemcuv2" },
  "ThingSpeak.ino"              : { "board": "nodemcuv2" },

  # No linux support
  "LinkItONE.ino"               : { "skip": True },

  # Energia
  "Energia_WiFi.ino"            : { "board": "lptm4c1294ncpdt" },
  "TI_MSP430F5529_CC3100.ino"   : { "board": "lpmsp430f5529" },
  "RedBearLab_CC3200.ino"       : { "skip": True },
  "RedBearLab_WiFi_Mini.ino"    : { "skip": True },
  "TI_CC3200_LaunchXL.ino"      : { "skip": True },
  "TI_Stellaris_LaunchPad.ino"  : { "board": "lplm4f120h5qr" },
  "TI_TivaC_Connected.ino"      : { "board": "lptm4c1294ncpdt" },

  #seeedTinyBLE
  #nrf51_dk
  #nucleo_f401re
  #rfduino
  #bbcmicrobit
}

examples = []
for root, dirnames, filenames in os.walk('examples'):
    for filename in fnmatch.filter(filenames, '*.ino'):
        fn = os.path.join(root, filename)
        examples.append(fn)

examples.sort()

for fn in examples:
    path, ino = os.path.split(fn)

    if ino in metadata:
        m = metadata[ino]
        if 'board' in m:
            path = path + ' PLATFORMIO_CI_BOARDS_ARGS="--board=' + m['board'] + '"'
    else:
        m = {}

    if 'skip' in m:
        path = "    # - PLATFORMIO_CI_SRC=" + path
    else:
        path = "    - PLATFORMIO_CI_SRC=" + path

    print path
