#!/usr/bin/python

import memlib

## FORMATTING A KV-PAIR  #######################################################

# V1
ShortTest = memlib.kv_pair("k", "v", "DEADBEEF", 42)

# V2
Integration1 = {
	"key"        : "this-is-the-key-that-i'll-be-using-for-the-next-time-to-test-the-longest-of-all-keys",
	"value"      : "aaaaaaaabbbbbbbbccccccccddddddddeeeeeeeeffffffffgggggggghhhhhhhhiiiiiiiijjjjjjjjkkkkkkkkllllllllmmmmmmmmnnnnnnnnooooooooppppppppqqqqqqqqrrrrrrrrssssssssttttttttuuuuuuuuvvvvvvvvwwwwwwwwxxxxxxxxyyyyyyyyzzzzzzzz",
	"flags"      : "0000002a",     # 32bit, hex-encoded
	"expiration" : 13
}

## CREATING SINGLE REQUESTS ####################################################

print "Binary Set Request"
rq = memlib.binarySetRequest(Integration1, "aabbccdd")
print memlib.simulationInput(rq)

print "Binary Set Response"
rq = memlib.binarySetResponse(Integration1, "aabbccdd")
print memlib.simulationOutput(rq)

print "Binary Get Request"
rq = memlib.binaryGetRequest(Integration1, "aabbccdd")
print memlib.simulationInput(rq)

print "2 Binary Get Request w/o delay"
print memlib.simulationInput(rq, False), # the ',' avoids the newline after print.
print memlib.simulationInput(rq)

print "Binary Get Response"
rq = memlib.binaryGetResponse(Integration1, "aabbccdd")
print memlib.simulationOutput(rq)

## CREATING FULL TESTSET #######################################################

print "Two Request @ Rate 12Gbps/1Gbps"
r1 = memlib.binarySetRequest(Integration1)
r2 = memlib.binaryGetRequest(Integration1)
print memlib.requests12Gbps([r1, r2])
print memlib.requests1Gbps([r1, r2])
r1 = memlib.binarySetResponse(Integration1)
r2 = memlib.binaryGetResponse(Integration1)
print memlib.responses([r1, r2])


print "Useful functions"
print ("%08x" % 42)
print "Key".encode('hex')
