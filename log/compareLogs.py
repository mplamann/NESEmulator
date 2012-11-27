#!/usr/bin/python

import sys

if len(sys.argv) < 2:
    print "Usage:\ncompareLogs.py NESEmulator_output Nintendulator_output\n"
    sys.exit(1)

inputFilePath = sys.argv[1]
referenceFilePath = sys.argv[2]
print "Using %s and %s" % (inputFilePath, referenceFilePath)

def parseReg(regString):
    return regString.split(":")[1]

with open(inputFilePath) as inputFile:
    with open(referenceFilePath) as referenceFile:
        lineNumber = 1
        while True:
            inputLine = inputFile.readline()
            referenceLine = referenceFile.readline()
            if inputLine == '' or referenceLine == '':
                break
            inputElements = inputLine.split()
            referenceElements = referenceLine.split()

            # Reference values
            PC = referenceElements[0]
            opcode = referenceElements[1]

            registerIndex = -1
            for i,element in enumerate(referenceElements):
                if element.startswith("A:"):
                    registerIndex = i
                    break
            A = parseReg(referenceElements[i])
            X = parseReg(referenceElements[i+1])
            Y = parseReg(referenceElements[i+2])
            P = parseReg(referenceElements[i+3])
            SP = parseReg(referenceElements[i+4])
            SL = parseReg(referenceElements[-1])

            
            if inputElements[0] != PC:
                print "PC (%s, %s) mismatch on line %d\n" % (PC, inputElements[0], lineNumber)
                break

            lineNumber += 1
