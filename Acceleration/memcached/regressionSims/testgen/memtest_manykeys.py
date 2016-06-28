#!/usr/bin/python

import memlib

testset = memlib.newTestset()

for i in range(300):
	pair = memlib.kv_pair("K%03d" % i, "V%03d" % i, "ABCDEF01", 0)
	memlib.set(pair, testset)

memlib.generate("MANYKEYS", testset)
memlib.generate_hls("MANYKEYS", testset)
