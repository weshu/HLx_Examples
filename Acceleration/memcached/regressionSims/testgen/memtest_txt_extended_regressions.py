#!/usr/bin/python

import memlib

## EDIT HERE ###################################################################

keySizes = range(1,28)
#keySizes.append(128)
valueSizes = keySizes[:]
#valueSizes.append(1015)

keyChars = map(chr, range(97, 126))
valueChars = map(chr, range(65, 94))

asciiVals = [1, 12, 123, 1234, 12345, 123456, 1234567, 12345678, 123456789, 2234567891]

################################################################################

def pair2kvpair(pair):
	return memlib.kv_pair(pair[0], pair[1], "%08x" % pair[2], 42)

def seq1(keys, values, repeat):
	kv_pairs = []
	key   = keys[0]
	value = values[0]
	for flags in asciiVals:
		kv_pairs.append( memlib.kv_pair(key, value, "%08x" % flags, 42) )
	for key in keys:
		for value in values:
			flags = asciiVals[0]
			kv_pairs.append( memlib.kv_pair(key, value, "%08x" % flags, 42) )

	testset = memlib.newTestset()
	for kv_pair in kv_pairs:
		memlib.setSuccess(kv_pair, testset)
		for _ in range(repeat):
			memlib.getSuccess(kv_pair, testset)

	return testset

def seq2(keys, values):
	testset = memlib.newTestset()

	for _ in range(len(values)):
		# for more keys than values, duplicate use of values
		values_used = values
		if len(keys) > len(values):
			while(len(keys) > len(values_used)):
				values_used = values_used + values
			values_used = values_used[0:len(keys)]

		# requests
		kv_pairs = map(pair2kvpair, zip(keys, values_used, asciiVals*50))

		for kv_pair in kv_pairs:
			memlib.setSuccess(kv_pair, testset)
		for kv_pair in kv_pairs:
			memlib.getSuccess(kv_pair, testset)

		# rotation
		values = values[1:] + values[0:1]
	return testset

################################################################################

if len(keySizes) > len(keyChars):
	sys.exit("Error: Not enough key characters.")
if len(valueSizes) > len(valueChars):
	sys.exit("Error: Not enough value characters.")

keyPairs = zip(keySizes, keyChars)
valuePairs = zip(valueSizes, valueChars)

keys = map(lambda (size, char): char * size, keyPairs)
values = map(lambda (size, char): char * size, valuePairs)

memlib.generate("SEQ1-EXTENDED", seq1(keys, values, 5))
memlib.generate_hls("SEQ1-EXTENDED", seq1(keys, values, 5))
memlib.generate("SEQ2-EXTENDED", seq2(keys, values))
memlib.generate_hls("SEQ2-EXTENDED", seq2(keys, values))
memlib.generate("SEQ3-EXTENDED", seq1(keys, values, 1))
memlib.generate_hls("SEQ3-EXTENDED", seq1(keys, values, 1))
