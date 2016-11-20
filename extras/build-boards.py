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

arduino_ide_path = "/data2/arduino-1.6.12"
energia_ide_path = "/data2/ard-energia-1.6.10E18"


hardware = os.path.expanduser('~') + '/.arduino15/packages/'
boards = find_files(hardware, 'boards.txt')

hardware2 = arduino_ide_path + '/hardware/'
boards2 = find_files(hardware2, 'boards.txt')

hardware3 = os.path.expanduser('~') + '/.energia15/packages/'
boards3 = find_files(hardware3, 'boards.txt')

hardware4 = energia_ide_path + '/hardware/'
boards4 = find_files(hardware4, 'boards.txt')

boards = list(boards) + list(boards2) + list(boards3) + list(boards4)
boards = filter(lambda x: -1 == x.find('variants'), boards)

fqbns = []
for b in boards:
    folders = b.replace(hardware,'').replace(hardware2,'').replace(hardware3,'').replace(hardware4,'')
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
data = {
  'failed': set(json_data['failed']),
  'built': set(json_data['built']),
  'once_built': set(json_data['once_built']),
}

skip_boards = [
  "Microduino:*",
  "WildFire*",
  "chipKIT:*",
  "esp8266:*",
  "stm32duino:STM32F3:discovery_f3", # no c++0x
  "digistump:avr:digispark-tiny*",
  ".*STM32F103T.*",
  ".*GD32F103C.*",
  "adafruit:avr:trinket.*",
]
skip_boards = "(" + ")|(".join(skip_boards) + ")"

fqbns.sort(key=lambda x: x['fqbn'] in data['built'])

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
        os.chdir(energia_ide_path)
        builder = "./energia"
    else:
        os.chdir(arduino_ide_path)
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
