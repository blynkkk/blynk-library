#!/usr/bin/env python

import os, sys, fnmatch
from subprocess import call
import json
import re

def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename

hardware = os.path.expanduser('~') + '/.arduino15/packages/'
boards = find_files(hardware, 'boards.txt')

hardware2 = '/data2/arduino-1.6.12/hardware/'
boards2 = find_files(hardware2, 'boards.txt')

boards = filter(lambda x: -1 == x.find('variants'), list(boards) + list(boards2))

fqbns = []
for b in boards:
    folders = b.replace(hardware,'').replace(hardware2,'')
    folders = folders.split('/')
    if len(folders) == 3:
        fqbn = [ folders[i] for i in [0, 1] ]
    else:
        fqbn = [ folders[i] for i in [0, 2] ]

    lastname = "#"
    with open(b) as f:
        for line in f.readlines():
            if ".name=" in line:
                line=line.strip()
                names = line.split(".name=")
                if names[0].startswith('#'):
                    continue

                lastname = names[0]
                name = names[1]
                fqbns.append({
                    'name': name,
                    'fqbn': ':'.join(fqbn + [lastname])
                })
            elif line.startswith(lastname + ".menu."):
                line = line.split('.menu.')[1]
                line = line.split('=')[0]
                line = line.split('.')
                fbqn = fqbns[-1]
                if not "opts" in fbqn:
                    fbqn["opts"] = {}
                if not line[0] in fbqn["opts"]:
                    fbqn["opts"][line[0]] = line[1]

logfile = open("./build.log","wb")
datafile = os.path.abspath('./boards-build.json')
fn = os.path.abspath('./tests/BlynkBuildTest/BlynkBuildTest.ino')

try:
    with open(datafile, 'r') as fp:
        json_data = json.load(fp)
except:
    json_data = {}
json_data.setdefault('failed', [])
json_data.setdefault('built', [])
json_data.setdefault('once_built', [])
json_data.setdefault('skip', [])

skip_boards = [
  "RedBearLab:*",
  "Microduino:*",
  "WildFire*",
  "chipKIT:*",
#  "esp8266:*",
  "stm32duino:STM32F3:discovery_f3", # no c++0x
]
skip_boards = "(" + ")|(".join(skip_boards) + ")"

data = {
  'failed': set(json_data['failed']),
  'built': set(json_data['built']),
  'once_built': set(json_data['once_built']),
}

fqbns.sort(key=lambda x: x['fqbn'] in data['built'])
'''
advanced_boards = {
  "stm32duino:STM32F1:genericSTM32F103C" : ":device_variant=STM32F103C8",
  "stm32duino:STM32F1:genericSTM32F103R" : ":device_variant=STM32F103R8",
  "stm32duino:STM32F1:genericSTM32F103T" : ":device_variant=STM32F103T8",
  "stm32duino:STM32F1:genericSTM32F103V" : ":device_variant=STM32F103VC",
  "stm32duino:STM32F1:genericSTM32F103Z" : ":device_variant=STM32F103ZC",
  "stm32duino:STM32F1:nucleo_f103rb" : ":device_variant=NucleoF103_HSI",

  "sandeepmistry:nRF5:Generic_nRF51822" : ":chip=xxaa,softdevice=none,lfclk=lfxo",
  "sandeepmistry:nRF5:RedBearLab_nRF51822" : ":version=1_5,softdevice=none",
  "sandeepmistry:nRF5:BLENano" : ":version=1_5,softdevice=none",

  "esp8266:esp8266:generic" : ":CpuFrequency=80,FlashFreq=40,FlashMode=dio,FlashSize=512K64",

  "attiny:avr:ATtinyX5" : ":cpu=attiny85,clock=internal16",
  "attiny:avr:ATtinyX4" : ":cpu=attiny84,clock=internal8",

  "konekt:sam:dash" : ":cpu_speed=speed120,check_update=check",
  "konekt:sam:dashpro" : ":cpu_speed=speed120,check_update=check",
  "konekt:sam:dashpro_beta" : ":cpu_speed=speed120,check_update=check",

  "teensy:avr:teensy36" : ":usb=serial,speed=180opt,keys=en-us",
  "teensy:avr:teensy35" : ":usb=serial,speed=120opt,keys=en-us",
  "teensy:avr:teensy31" : ":usb=serial,speed=96opt,keys=en-us",
  "teensy:avr:teensy30" : ":usb=serial,speed=96,keys=en-us",
  "teensy:avr:teensyLC" : ":usb=serial,speed=48,keys=en-us",
  "teensy:avr:teensypp2" : ":usb=serial,speed=16,keys=en-us",
  "teensy:avr:teensy2" : ":usb=serial,speed=16,keys=en-us",
  
}

for x in fqbns:
    if x['fqbn'] in advanced_boards.keys():
       x['fqbn_adv'] = x['fqbn'] + advanced_boards[x['fqbn']]
'''

advanced_boards = {
  "attiny:avr:ATtinyX5" : ":cpu=attiny85,clock=internal16",
  "attiny:avr:ATtinyX4" : ":cpu=attiny84,clock=internal8",
}

for x in fqbns:
    if x['fqbn'] in advanced_boards.keys():
        x['fqbn_adv'] = x['fqbn'] + advanced_boards[x['fqbn']]
    elif 'opts' in x:
        opts = ""
        for o in x['opts'].keys():
            opts +=  o + '=' + x['opts'][o] + ','

        x['fqbn_adv'] = x['fqbn'] + ":" + opts.strip(',')

#print json.dumps(fqbns, sort_keys=True, indent=2)
#exit(0)

for m in fqbns:

    if len(sys.argv) > 1:
        if m['fqbn'] != sys.argv[1]:
            continue
    elif re.match(skip_boards, m['fqbn']):
        continue

    print "Building:", m['name'], " (", m['fqbn'], ")", " ...",
    sys.stdout.flush()

    print >>logfile, "\n\n", "================="
    print >>logfile, "Building:", m['name'], " (", m['fqbn'], ")"
    print >>logfile, "=================", "\n"
    logfile.flush()

    if m["fqbn"].startswith("energia:"):
        os.chdir("/data2/ard-energia-1.6.10E18/")
        builder = "./energia"
    else:
        os.chdir("/data2/arduino-1.6.12/")
        builder = "./arduino"

    fqbn = m["fqbn_adv"] if "fqbn_adv" in m.keys() else m["fqbn"]
    cmd = [
        builder,
        "--verbose",
        "--verify",
        "--board", fqbn,
        fn
    ]
    #print cmd
    rc = call(cmd, stdout=logfile, stderr=logfile)
    if rc:
        print "Failed"
        data['failed'].add(m["fqbn"])
        if m["fqbn"] in data['built']:
            data['built'].remove(m["fqbn"])
    else:
        print "OK"
        data['once_built'].add(m["fqbn"])
        data['built'].add(m["fqbn"])
        if m["fqbn"] in data['failed']:
            data['failed'].remove(m["fqbn"])


    with open(datafile, 'w') as fp:
        json.dump({
          'failed': sorted(list(data['failed'])),
          'built': sorted(list(data['built'])),
          'once_built': sorted(list(data['once_built'])),
          'broken': sorted(data['once_built'].intersection(data['failed'])),
        }, fp, indent=4)

print "=================="
if len(data['built']):
    print " OK:  ", len(data['built'])
if len(data['failed']):
    print " Fail:", len(data['failed'])
