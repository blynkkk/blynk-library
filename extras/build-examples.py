#!/usr/bin/env python

import os, sys, fnmatch
from subprocess import call

arduino_ide_path = os.environ['ARDUINO_IDE_PATH']
energia_ide_path = os.environ['ENERGIA_IDE_PATH']

ESP8266 = { "fqbn": "esp8266:esp8266:nodemcuv2:CpuFrequency=80,FlashSize=4M3M" }

metadata = {
  "Arduino_Yun.ino"             : { "fqbn": "arduino:avr:yun" },
  "Arduino_Zero_M0_Serial.ino"  : { "fqbn": "arduino:samd:arduino_zero_native" },
  "ENC28J60.ino"                : { "fqbn": "arduino:avr:nano:cpu=atmega328" },
  "ESP8266_Shield.ino"          : { "fqbn": "arduino:avr:mega:cpu=atmega2560" },
  "RN_XV_WiFly.ino"             : { "fqbn": "arduino:avr:leonardo" },
  "Serial_HM10_HC08.ino"        : { "fqbn": "arduino:avr:leonardo" },
  "Adafruit_Feather_32u4_BLE.ino": { "fqbn": "adafruit:avr:feather32u4" },
  "Seeed_EthernetV2_0.ino"      : { "fqbn": "arduino:avr:uno" },

  # arduino.org
  "Arduino_Ethernet2.ino"       : { "skip": True },
  "Arduino_org_UNO_WiFi.ino"    : { "fqbn": "arduino:avr:unowifi" },

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

  "ESP32_WiFi.ino"              : { "fqbn": "espressif:esp32:esp32:FlashMode=qio,FlashFreq=80,FlashSize=4M" },

  "myPlant_ESP8266.ino"         : ESP8266,
  "Template_ESP8266.ino"        : ESP8266,
  "Template_MKR1000.ino"        : { "fqbn": "arduino:samd:mkr1000" },

  # TheAirBoard
  "TheAirBoard_WiFly.ino"       : { "fqbn": "arduino:avr:fio" },
  "TheAirBoard_BLE_Link.ino"    : { "fqbn": "arduino:avr:fio" },

  # Digistump
  "Digistump_Digispark.ino"     : { "fqbn": "digistump:avr:digispark-pro" },
  "Digistump_Oak.ino"           : { "fqbn": "digistump:oak:oak1:CpuFrequency=80,UploadTool=oak_ota,FlashSize=OAK,RomConfig=Full" },

  # Intel
  "Intel_Edison_WiFi.ino"       : { "fqbn": "Intel:i686:izmir_ec" },
  "Intel_Galileo.ino"           : { "fqbn": "Intel:i586:izmir_fg" },
  "Arduino_101_BLE.ino"         : { "fqbn": "Intel:arc32:arduino_101" },

  # RBL
  "RedBear_Duo_WiFi.ino"        : { "fqbn": "RedBear:STM32F2:RedBear_Duo" },
  "RedBear_Duo_BLE.ino"         : { "fqbn": "RedBear:STM32F2:RedBear_Duo" },
  "RedBearLab_BLE_Nano.ino"     : { "fqbn": "RedBear:nRF51822:nRF51822_NANO_32KB" },
  "RedBearLab_BlendMicro.ino"   : { "fqbn": "RedBear:avr:blendmicro8" },
  "RedBearLab_BLE_Mini.ino"     : { "fqbn": "arduino:avr:leonardo" },

  # GSM
  "SIM800_SIM900.ino"           : { "fqbn": "arduino:avr:leonardo" },

  #Other
  "Blue_Pill_STM32F103C.ino"    : { "fqbn": "stm32duino:STM32F1:genericSTM32F103C:device_variant=STM32F103C8" },
  "Fishino.ino"                 : { "skip": True },  # requires extra lib
  "BBC_MicroBit.ino"            : { "fqbn": "sandeepmistry:nRF5:BBCmicrobit:softdevice=s130" },
  "Simblee_BLE.ino"             : { "fqbn": "Simblee:Simblee:Simblee" },
  "RFDuino_BLE.ino"             : { "fqbn": "RFduino:RFduino:RFduino" },
  "DFRobot_Bluno_BLE_Link.ino"  : { "fqbn": "arduino:avr:leonardo" },
  "Microduino_BLE.ino"          : { "fqbn": "arduino:avr:leonardo" },
  "TinyDuino_WiFi.ino"          : { "fqbn": "arduino:avr:pro:cpu=8MHzatmega328" },
  "WildFire_V3.ino"             : { "fqbn": "WildFire- 4.0.0:avr:wildfireo3" },
  "WildFire_V4.ino"             : { "fqbn": "WildFire- 4.0.0:avr:wildfireo3" },
  "chipKIT_Uno32.ino"           : { "fqbn": "chipKIT:pic32:uno_pic32" },
  "LightBlue_Bean.ino"          : { "fqbn": "LightBlue-Bean:avr:beanplus" },
  "Teensy3.ino"                 : { "fqbn": "teensy:avr:teensy31:usb=serial,speed=96,opt=o2std,keys=en-us" },
  "ATtiny85.ino"                : { "fqbn": "attiny:avr:ATtinyX5:cpu=attiny85,clock=internal8" },

  # Special examples
  "ESP8266.ino"                 : ESP8266,
  "HandleDisconnect.ino"        : ESP8266,

  # No linux support
  "LinkItONE.ino"               : { "skip": True },

  # Energia
  "Energia_WiFi.ino"            : { "fqbn": "energia:msp430:MSP-EXP430F5529LP" },
  "Energia_BLE.ino"             : { "fqbn": "energia:msp432r:MSP-EXP432P401RR" },
  "Energia_Ethernet.ino"        : { "fqbn": "energia:tivac:EK-TM4C1294XL" },
  "Energia_Serial_USB.ino"      : { "fqbn": "energia:tivac:EK-LM4F120XL" },
}

def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename

examples = find_files('examples', '*.ino')
abs_examples = map(lambda x: os.path.abspath(x), examples)

logfile = open("./build.log","wb")

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
skipped = []

for fn in abs_examples:
    path, ino = os.path.split(fn)

    if ino in metadata:
        m = metadata[ino]
        m['sketch_found'] = True
        if "skip" in m:
            skipped.append(ino)
            continue
        if not "fqbn" in m:
            m["fqbn"] = "arduino:avr:uno"
    else:
        #continue
        m = { "fqbn": "arduino:avr:uno" }

    if len(sys.argv) > 1:
        if ino != sys.argv[1]:
            continue

    print >>logfile, "\n\n", "================="
    print >>logfile, "Building:", ino
    print >>logfile, "=================", "\n"
    logfile.flush()

    print "Building:", ino, "...",
    sys.stdout.flush()

    if m["fqbn"].startswith("energia:"):
        os.chdir(energia_ide_path)
        builder = "./energia"
    else:
        os.chdir(arduino_ide_path)
        builder = "./arduino"

    cmd = [
        builder,
        "--verbose",
        "--verify",
        "--board", m["fqbn"],
        fn
    ]
    #print cmd
    rc = call(cmd, stdout=logfile, stderr=logfile)
    if rc:
        print "Failed"
        failed.append(ino)
    else:
        print "OK"
        built.append(ino)

print "=================="
if len(failed):
    print " Failed:", failed
    sys.exit(1)
else:
    print " All", len(built), "examples OK"

for ino, m in metadata.items():
    if not 'sketch_found' in m:
        print "Sketch not found:", ino
