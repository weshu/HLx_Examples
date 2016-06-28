#!/usr/bin/python

import memlib

## EDIT HERE ###################################################################

keySizes = [1,3,4,5,7,8,9,10,12,13,15,16,17,24,25,28,84,128]
#valueSizes = [1,3,4,5,8,9,10,12,13,16,17,24,28,184,208,1024]
#valueSizes = [1,3,4,5,8,9,10,12,13,16,17,24,28,184,208,1015]
valueSizes = [184,389, 792, 1015, 1862, 2469, 3285, 4397, 5850, 7932, 10174, 14473, 18043, 25836, 30859, 38623, 46827, 57482, 65520]
seq1repeat = 5

keyChars = map(chr, range(97, 123))
valueChars = map(chr, range(65, 91))

## EDIT FINISHED ###############################################################

DEBUG_SEQUENCES = False
PRINT_SEQUENCES = True

################################################################################

if DEBUG_SEQUENCES:
	keySizes = [1,2,3]
	valueSizes = [1,2]

def pair2kvpair(pair):
	return memlib.kv_pair(pair[0], pair[1], "EFBEADDE", 42)

def seq1(keys, values, repeat):
	if PRINT_SEQUENCES:
		print "--- SEQUENCE 1 repeat %-3s -----------------------------------------------------" % repeat
	kv_pairs = []
	for key in keys:
		for value in values:
			kv_pairs.append( memlib.kv_pair(key, value, "EFBEADDE", 42) )

	requests = []
	responses = []
	for kv_pair in kv_pairs:
		if PRINT_SEQUENCES:
			print "Set [%d -> %d]: %s -> %s" % (len(kv_pair['key']), len(kv_pair['value']), kv_pair['key'], kv_pair['value'])
		requests.append( memlib.binarySetRequest( kv_pair , "00000000" ) )
		responses.append( memlib.binarySetResponse( kv_pair, "00000000" ) )

		for _ in range(repeat):
			if PRINT_SEQUENCES:
				print "Get [%d -> %d]: %s -> %s" % (len(kv_pair['key']), len(kv_pair['value']), kv_pair['key'], kv_pair['value'])
			requests.append( memlib.binaryGetRequest( kv_pair , "00000000"  ) )
			responses.append( memlib.binaryGetResponse( kv_pair , "00000000" ) )

	return (requests, responses)

def seq2(keys, values):
	if PRINT_SEQUENCES:
		print "--- SEQUENCE 2 -----------------------------------------------------------------"
	requests = []
	responses = []
	for _ in range(len(values)):
		# for more keys than values, duplicate use of values
		values_used = values
		if len(keys) > len(values):
			while(len(keys) > len(values_used)):
				values_used = values_used + values
			values_used = values_used[0:len(keys)]

		# requests
		kv_pairs = map(pair2kvpair, zip(keys, values_used))
		for kv_pair in kv_pairs:
			if PRINT_SEQUENCES:
				print "Set [%d -> %d]: %s -> %s" % (len(kv_pair['key']), len(kv_pair['value']), kv_pair['key'], kv_pair['value'])
			requests.append( memlib.binarySetRequest(kv_pair, "00000000") )
			responses.append( memlib.binarySetResponse(kv_pair, "00000000") )
		for kv_pair in kv_pairs:
			if PRINT_SEQUENCES:
				print "Get [%d -> %d]: %s -> %s" % (len(kv_pair['key']), len(kv_pair['value']), kv_pair['key'], kv_pair['value'])
			requests.append( memlib.binaryGetRequest(kv_pair, "00000000") )
			responses.append( memlib.binaryGetResponse(kv_pair, "00000000") )

		# rotation
		values = values[1:] + values[0:1]
	
	return (requests, responses)

################################################################################

if len(keySizes) > len(keyChars):
	sys.exit("Error: Not enough key characters.")
if len(valueSizes) > len(valueChars):
	sys.exit("Error: Not enough value characters.")

keyPairs = zip(keySizes, keyChars)
valuePairs = zip(valueSizes, valueChars)

keys = map(lambda (size, char): char * size, keyPairs)
values = map(lambda (size, char): char * size, valuePairs)

SEQ1 = seq1(keys, values, seq1repeat)
SEQ2 = seq2(keys, values)
SEQ3 = seq1(keys, values, 1)

# SEQ1
req = open("SEQ1_R12-pkt.in.long.txt", "w")
req.write( memlib.requests12Gbps(SEQ1[0]) )
req.close()
req = open("SEQ1_R1-pkt.in.long.txt", "w")
req.write( memlib.requests1Gbps(SEQ1[0]) )
req.close()
#res = open("SEQ1-pkt.out.long.txt", "w")
#res.write( memlib.responses(SEQ1[1]) )
#res.close()
res = open("SEQ1-pkt.out.long.hls.rtl.txt", "w")
res.write( memlib.responses_rtl_hls(SEQ1[1]) )
res.close()

# SEQ2
req = open("SEQ2_R12-pkt.in.long.txt", "w")
req.write( memlib.requests12Gbps(SEQ2[0]) )
req.close()
req = open("SEQ2_R1-pkt.in.long.txt", "w")
req.write( memlib.requests1Gbps(SEQ2[0]) )
req.close()
#res = open("SEQ2-pkt.out.long.txt", "w")
#res.write( memlib.responses(SEQ2[1]) )
#res.close()
res = open("SEQ2-pkt.out.long.hls.rtl.txt", "w")
res.write( memlib.responses_rtl_hls(SEQ2[1]) )
res.close()

# SEQ3
req = open("SEQ3_R12-pkt.in.long.txt", "w")
req.write( memlib.requests12Gbps(SEQ3[0]) )
req.close()
req = open("SEQ3_R1-pkt.in.long.txt", "w")
req.write( memlib.requests1Gbps(SEQ3[0]) )
req.close()
#res = open("SEQ3-pkt.out.long.txt", "w")
#res.write( memlib.responses(SEQ3[1]) )
#res.close()
res = open("SEQ3-pkt.out.long.hls.rtl.txt", "w")
res.write( memlib.responses_rtl_hls(SEQ3[1]) )
res.close()
####### Same thing for HLS outputs #######
# SEQ1
req = open("SEQ1_R12-pkt.in.long.hls.txt", "w")
req.write( memlib.requests12Gbps_hls(SEQ1[0]) )
req.close()
req = open("SEQ1_R1-pkt.in.long.hls.txt", "w")
req.write( memlib.requests1Gbps_hls(SEQ1[0]) )
req.close()
res = open("SEQ1-pkt.out.long.hls.txt", "w")
res.write( memlib.responses_hls(SEQ1[1]) )
res.close()

# SEQ2
req = open("SEQ2_R12-pkt.in.long.hls.txt", "w")
req.write( memlib.requests12Gbps_hls(SEQ2[0]) )
req.close()
req = open("SEQ2_R1-pkt.in.long.hls.txt", "w")
req.write( memlib.requests1Gbps_hls(SEQ2[0]) )
req.close()
res = open("SEQ2-pkt.out.long.hls.txt", "w")
res.write( memlib.responses_hls(SEQ2[1]) )
res.close()

# SEQ3
req = open("SEQ3_R12-pkt.in.long.hls.txt", "w")
req.write( memlib.requests12Gbps_hls(SEQ3[0]) )
req.close()
req = open("SEQ3_R1-pkt.in.long.hls.txt", "w")
req.write( memlib.requests1Gbps_hls(SEQ3[0]) )
req.close()
res = open("SEQ3-pkt.out.long.hls.txt", "w")
res.write( memlib.responses_hls(SEQ3[1]) )
res.close()
