#!/usr/bin/env python

import os
import sys

import fileinput

def cmdfilter(cmd):
   for el in cmd:
       el = el.strip()
       if el == '-c': continue
       if el == '-o': continue
       if el.startswith('/tmp/'): continue
       yield el

for line in fileinput.input():
    line = line.strip('[]')
    line = line.split(',')
    line = list(cmdfilter(line))
    line = ' '.join(line) + ' -dM -E - < /dev/null | sort'
    os.system(line)
