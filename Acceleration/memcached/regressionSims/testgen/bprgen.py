#!/usr/bin/python

import sys

if len(sys.argv) < 4:
	print >> sys.stderr, "Usage:"
	print >> sys.stderr, "\tbprgen delay period duration [period count=1000]"
	sys.exit()

delay    = int(sys.argv[1])
period   = int(sys.argv[2])
duration = int(sys.argv[3])
count    = 1000

if len(sys.argv) >= 5:
	count = int(sys.argv[4])

if period < duration:
	print >> sys.stderr, "Error: period must be larger than duration."
	sys.exit()

for _ in range(delay):
	print "0"

for _ in range(count):
	diff = period - duration
	for __ in range(diff):
		print "0"
	for __ in range(duration):
		print "1"
