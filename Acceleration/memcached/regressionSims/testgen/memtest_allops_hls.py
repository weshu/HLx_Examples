#!/usr/bin/python

import memlib

## EDIT HERE ###################################################################

keySizes = range(1,18)
keySizes.append(127)
keySizes.append(128)
valueSizes = keySizes[:]
valueSizes.append(96)

keyChars = map(chr, range(97, 126))
valueChars = map(chr, range(65, 94))

flags = [1, 4234567892]

################################################################################

if len(keySizes) > len(keyChars):
	sys.exit("Error: Not enough key characters.")
if len(valueSizes) > len(valueChars):
	sys.exit("Error: Not enough value characters.")

keyPairs = zip(keySizes, keyChars)
valuePairs = zip(valueSizes, valueChars)

keys = map(lambda (size, char): char * size, keyPairs)
values = map(lambda (size, char): char * size, valuePairs)

kv_pairs = []
for key in keys:
	for value in values:
		for flag in flags:
			kv_pairs.append( memlib.kv_pair(key, value, "%08x" % flag, 42) )
kv_pairs_last = []
for key in keys:
	v = values[-1]
	f = flags[-1]
	kv_pairs_last.append( memlib.kv_pair(key, v, "%08x" % f, 42) )

testset = memlib.newTestset()

for pair in kv_pairs_last:
	memlib.getFail(pair, testset)
for pair in kv_pairs:
	memlib.setSuccess(pair, testset)
for pair in kv_pairs_last:
	memlib.getSuccess(pair, testset)
for pair in kv_pairs_last:
	memlib.delete(pair, testset)
for pair in kv_pairs_last:
	memlib.getFail(pair, testset)
for pair in kv_pairs_last:
	memlib.setSuccess(pair, testset)
memlib.flush(testset)
for pair in kv_pairs_last:
	memlib.getFail(pair, testset)
for pair in kv_pairs_last:
	memlib.deleteFail(pair, testset)

memlib.generate_hls("ALLOPS", testset)
