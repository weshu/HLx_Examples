#!/usr/bin/python

import memlib

keySizes = range(1,28)
valueSizes = keySizes[:]

keyChars = map(chr, range(97, 126))
valueChars1 = map(chr, range(65, 94))
valueChars2 = valueChars1[1:] + valueChars1[:1]
valueChars3 = valueChars1[2:] + valueChars1[:2]
valueChars4 = valueChars1[3:] + valueChars1[:3]
valueChars5 = valueChars1[4:] + valueChars1[:4]
valueChars6 = valueChars1[5:] + valueChars1[:5]
valueChars7 = valueChars1[6:] + valueChars1[:6]
valueChars8 = valueChars1[7:] + valueChars1[:7]
valueChars9 = valueChars1[8:] + valueChars1[:8]
valueChars10 = valueChars1[9:] + valueChars1[:9]
valueChars11 = valueChars1[10:] + valueChars1[:10]
valueChars12 = valueChars1[11:] + valueChars1[:11]
valueChars13 = valueChars1[12:] + valueChars1[:12]
valueChars14 = valueChars1[13:] + valueChars1[:13]
valueChars15 = valueChars1[14:] + valueChars1[:14]

keyPairs = zip(keySizes, keyChars)
keys = map(lambda (size, char): char * size, keyPairs)

def get_pairs(keys, value_chars):
	value_pairs = zip(valueSizes, value_chars)
	values_use = map(lambda (size, char): char * size, value_pairs)
	pairs = []
	for i in range(0,27):
		pairs.append( memlib.kv_pair(keys[i], values_use[i], "08080808", 42) )
	return pairs

def do_sets(keys, value_chars, testset):
	pairs = get_pairs(keys, value_chars)
	for p in pairs:
		memlib.setSuccess(p, testset)

def do_getsSuccess(keys, value_chars, testset):
	pairs = get_pairs(keys, value_chars)
	for p in pairs:
		memlib.getSuccess(p, testset)

def do_getsFail(keys, value_chars, testset):
	pairs = get_pairs(keys, value_chars)
	for p in pairs:
		memlib.getFail(p, testset)

testset = memlib.newTestset()

do_sets(keys, valueChars1, testset)
do_sets(keys, valueChars2, testset)
do_sets(keys, valueChars3, testset)
do_sets(keys, valueChars4, testset)
do_sets(keys, valueChars5, testset)
do_sets(keys, valueChars6, testset)
do_sets(keys, valueChars7, testset)
do_sets(keys, valueChars8, testset)
do_sets(keys, valueChars9, testset)
do_sets(keys, valueChars10, testset)
do_sets(keys, valueChars11, testset)
do_sets(keys, valueChars12, testset)
do_sets(keys, valueChars13, testset)
do_sets(keys, valueChars14, testset)
do_sets(keys, valueChars15, testset)
do_getsSuccess(keys, valueChars15, testset)
memlib.flush(testset)
do_getsFail(keys, valueChars15, testset)

memlib.generate("MANYSETS", testset)
memlib.generate_hls("MANYSETS", testset)
