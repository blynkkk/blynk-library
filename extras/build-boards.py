#!/usr/bin/env python

import os, sys, fnmatch
from subprocess import call
import pprint

pp = pprint.PrettyPrinter(indent=4, width=240)

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

    with open(b) as f:
        for line in f.readlines():
            if ".name=" in line:
                line=line.strip()
                names = line.split(".name=")
                if names[0].startswith('#'):
                    continue

                name = names[1]
                fqbns.append({
                    'name': name,
                    'fqbn': ':'.join(fqbn + [names[0]])
                })

logfile = open("./build.log","wb")

failed = []
built = []
skipped = []

fn = os.path.abspath('./tests/BlynkFootprintTest/BlynkFootprintTest.ino')

for m in fqbns:

    if len(sys.argv) > 1:
        if m['fqbn'] != sys.argv[1]:
            continue

    print >>logfile, "\n\n", "================="
    print >>logfile, "Building:", m['name'], " (", m['fqbn'], ")"
    print >>logfile, "=================", "\n"
    logfile.flush()

    print "Building:", m['name'], " (", m['fqbn'], ")", " ...",
    sys.stdout.flush()

    if m["fqbn"].startswith("energia:"):
        os.chdir("/data2/ard-energia-1.6.10E18/")
        builder = "./energia"
    else:
        os.chdir("/data2/arduino-1.6.12/")
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
        failed.append(m["fqbn"])
    else:
        print "OK"
        built.append(m["fqbn"])

print "=================="
if len(failed):
    print " Failed:", failed
    sys.exit(1)
else:
    print " All", len(built), "examples OK"
