#!/usr/bin/env python

import fnmatch
import os

metadata = {
  "Arduino_Yun.ino"             : { "board": "yun" },
  "Arduino_Zero_M0_Serial.ino"  : { "board": "zero" },
  "ENC28J60.ino"                : { "board": "nanoatmega328" },
  "ESP8266_Shield.ino"          : { "board": "megaatmega2560" },
  "RN_XV_WiFly.ino"             : { "board": "leonardo" },
  "Serial_HM10_HC08.ino"        : { "board": "leonardo" },
  "DFRobot_Bluno_BLE_Link.ino"  : { "board": "leonardo" },
  "Microduino_BLE.ino"          : { "board": "leonardo" },
  "TheAirBoard_WiFly.ino"       : { "skip": True, "board": "fio" }, # No AirBoard library
  "TheAirBoard_BLE_Link.ino"    : { "skip": True, "board": "fio" }, # No AirBoard library
  "Adafruit_Feather_32u4_BLE.ino": { "board": "feather32u4", "lib_deps": "SoftwareSerial" },
  "Blue_Pill_STM32F103C.ino"    : { "board": "bluepill_f103c8", "framework": "arduino" },
  "Seeed_EthernetV2_0.ino"      : { "skip": True }, # Breaks build
  "Arduino_Ethernet2.ino"       : { "skip": True }, # Breaks build, arduino.org

  # ESP8266
  "Sparkfun_Blynk_Board.ino"    : { "board": "nodemcuv2" },
  "ESP8266_DirectConnect.ino"   : { "board": "nodemcuv2" },
  "ESP8266_Standalone.ino"      : { "board": "esp01" },
  "ESP8266_Standalone_SmartConfig.ino"  : { "board": "nodemcuv2" },
  "ESP8266_Standalone_SSL.ino"  : { "board": "nodemcuv2" },
  "ESP8266_Standalone_Manual_IP.ino" : { "board": "esp01" },

  "ESP32_WiFi.ino"              : { "board": "nano32", "framework": "arduino" , "lib_ignore": "WiFi101" },

  "Template_ESP8266.ino"        : { "board": "nodemcuv2" , "lib_ignore": "WiFi101", "build_flags": "-DBOARD_LED_PIN_WS2812 -DUSE_TICKER"},
  "myPlant_ESP8266.ino"         : { "board": "nodemcuv2", "lib_ignore": "WiFi101", "build_flags": "-DBOARD_LED_PIN_WS2812 -DUSE_TICKER"},
  "Template_MKR1000.ino"        : { "board": "mkr1000USB", "build_flags": "-DUSE_TIMER_FIVE"},

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
  "RedBearLab_BLE_Nano.ino"     : { "skip": True, "board": "redBearLabBLENano", "framework": "arduino" }, # Not for Arduino
  "RedBearLab_BlendMicro.ino"   : { "board": "blendmicro8" },
  "RedBearLab_BLE_Mini.ino"     : { "board": "leonardo" },

  # GSM
  "SIM800_SIM900.ino"           : { "board": "leonardo" },

  #Other
  "Fishino.ino"                 : { "skip": True },  # requires extra lib
  "Energia_WiFi.ino"            : { "skip": True },
  "BBC_MicroBit.ino"            : { "board": "uno", "lib_deps": "SPI" }, #TODO
  "Simblee_BLE.ino"             : { "skip": True },
  "RFDuino_BLE.ino"             : { "board": "rfduino" },
  "TinyDuino_WiFi.ino"          : { "board": "tinyduino" },
  "WildFire_V3.ino"             : { "board": "wildfirev3" },
  "WildFire_V4.ino"             : { "skip": True },
  "chipKIT_Uno32.ino"           : { "board": "uno_pic32" },
  "LightBlue_Bean.ino"          : { "board": "lightblue-bean" },
  "Teensy3.ino"                 : { "board": "teensy31" },
  "ATtiny85.ino"                : { "board": "attiny85" },

  # Special examples
  "ESP8266.ino"                 : { "board": "nodemcuv2" },

  # No linux support
  "LinkItONE.ino"               : { "skip": True },

  # Energia
  "Energia_WiFi.ino"            : { "skip": True, "board": "lpmsp430f5529" },
  "Energia_Ethernet.ino"        : { "board": "lptm4c1294ncpdt" },
  "Energia_Serial_USB.ino"      : { "board": "lplm4f120h5qr" },
}

pio_project_options = ['lib_ignore', 'framework', 'build_flags', 'lib_deps']

  #seeedTinyBLE
  #nrf51_dk

  #nucleo_f401re
  #rfduino

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
        m['sketch_found'] = True
        extra_args = ''
        if 'board' in m:
            extra_args += "--board=" + m['board'] + " "
        if set(m.keys()) & set(pio_project_options):
            for _key in set(m.keys()) & set(pio_project_options):
                extra_args += "--project-option='{0}={1}' ".format(_key, m[_key])

        if len(extra_args):
            path += ' PLATFORMIO_CI_EXTRA_ARGS="' + extra_args.strip() + '"'
    else:
        m = {}

    if 'skip' in m:
        path = "    # - PLATFORMIO_CI_SRC=" + path
    else:
        path = "    - PLATFORMIO_CI_SRC=" + path

    print path

for ino, m in metadata.items():
    if not 'sketch_found' in m:
        print "Sketch not found:", ino
