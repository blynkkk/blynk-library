#!/usr/bin/env python

import fnmatch
import os

ESP8266 = { "board": "nodemcuv2" }
ESP32 =   { "board": "esp32dev", "framework": "arduino" } # "lib_deps": "ESP32 BLE Arduino" }

metadata = {
  "Arduino_Yun.ino"             : { "board": "yun" },
  "Arduino_Zero_M0_Serial.ino"  : { "board": "zero" },
  "ENC28J60.ino"                : { "board": "nanoatmega328" },
  "ESP8266_Shield.ino"          : { "board": "megaatmega2560" },
  "RN_XV_WiFly.ino"             : { "board": "leonardo" },
  "Serial_HM10_HC08.ino"        : { "board": "leonardo" },
  "Adafruit_Feather_32u4_BLE.ino": { "board": "feather32u4", "lib_deps": "SoftwareSerial" },
  "Seeed_EthernetV2_0.ino"      : { "skip": True }, # Breaks build

  # arduino.org
  "Arduino_Ethernet2.ino"       : { "skip": True },

  # ESP8266
  "Sparkfun_Blynk_Board.ino"    : ESP8266,
  "NodeMCU.ino"                 : ESP8266,
  "ESP8266_DirectConnect.ino"   : ESP8266,
  "ESP8266_Standalone.ino"      : ESP8266,
  "ESP8266_Standalone_Manual_IP.ino"  : ESP8266,
  "ESP8266_Standalone_SmartConfig.ino"  : ESP8266,
  "ESP8266_Standalone_SSL.ino"  : ESP8266,

  "ESP8266_Station.ino"         : ESP8266,
  "ESP8266_AccessPoint.ino"     : ESP8266,
  "ESP8266_WiFi.ino"            : ESP8266,

  "myPlant_ESP8266.ino"         : { "board": "nodemcuv2", "build_flags": "-DBOARD_LED_PIN_WS2812 -DUSE_TICKER"},
  "Template_ESP8266.ino"        : { "board": "nodemcuv2" , "build_flags": "-DBOARD_LED_PIN_WS2812 -DUSE_TICKER"},
  "Template_MKR1000.ino"        : { "board": "mkr1000USB", "build_flags": "-DUSE_TIMER_FIVE", "lib_deps": "WiFi101"},

  "Arduino_MKR1000.ino"         : { "board": "mkr1000USB", "lib_deps": "WiFi101" },
  "Adafruit_Feather_M0_WiFi.ino": { "board": "adafruit_feather_m0", "lib_deps": "SPI, WiFi101" },
  "Arduino_WiFi_Shield_101.ino" : { "board": "uno", "lib_deps": "WiFi101" },
  "WizFi310.ino"                : { "board": "uno", "lib_deps": "WizFi310" },
  "Arduino_org_UNO_WiFi.ino"    : { "board": "uno", "lib_deps": "WiFi Link", "build_flags": "-DESP_CH_SPI" },

  # ESP32
  "ESP32_WiFi.ino"              : ESP32,
  "ESP32_BLE.ino"               : ESP32,

  # TheAirBoard
  "TheAirBoard_WiFly.ino"       : { "skip": True, "board": "fio" }, # No AirBoard library
  "TheAirBoard_BLE_Link.ino"    : { "skip": True, "board": "fio" }, # No AirBoard library

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
  "Blue_Pill_STM32F103C.ino"    : { "board": "bluepill_f103c8", "framework": "arduino" },
  "Fishino.ino"                 : { "skip": True },  # requires extra lib
  "BBC_MicroBit.ino"            : { "board": "uno", "lib_deps": "SPI" }, #TODO
  "Simblee_BLE.ino"             : { "skip": True },
  "RFDuino_BLE.ino"             : { "board": "rfduino" },
  "DFRobot_Bluno_BLE_Link.ino"  : { "board": "leonardo" },
  "Microduino_BLE.ino"          : { "board": "leonardo" },
  "TinyDuino_WiFi.ino"          : { "board": "tinyduino", "lib_deps": "Adafruit CC3000 Library" },
  "CC3000.ino"                  : { "board": "uno", "lib_deps": "Adafruit CC3000 Library" },
  "WildFire_V3.ino"             : { "board": "wildfirev3", "lib_deps": "WildFire-CC3000"  },
  "WildFire_V4.ino"             : { "skip": True },
  "chipKIT_Uno32.ino"           : { "board": "uno_pic32" },
  "LightBlue_Bean.ino"          : { "board": "lightblue-bean" },
  "Teensy3.ino"                 : { "board": "teensy31" },
  "ATtiny85.ino"                : { "board": "attiny85" },
  "DHT11.ino"                   : { "board": "uno", "lib_deps": "Adafruit Unified Sensor" },

  # Special examples
  "ESP8266.ino"                 : ESP8266,
  "HandleDisconnect.ino"        : ESP8266,


  # No linux support
  "LinkItONE.ino"               : { "skip": True },

  # Energia
  "Energia_WiFi.ino"            : { "skip": True, "board": "lpmsp430f5529" },
  "Energia_BLE.ino"             : { "skip": True, "board": "..." },
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
