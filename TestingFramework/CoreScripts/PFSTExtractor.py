import os

def ExtractPFSTToFile(logName, masterLogFolder):
        tableHeader = "Placement Flow Stages Table"
        log         = open(logName, "r")
        lines       = log.readlines()
        log.close()
        isHeaderFound  = False
        isTableReached = False
        lineIdx  = 0
        printStr = ""

        for line in lines:
            if (isHeaderFound == False):
                if line.find(tableHeader) != -1:
                    isHeaderFound = True
                    printStr += line

                else:
                    lineIdx += 1

                    if lineIdx == len(lines):
                        print("PFST was not found in %s\n" % (logName))
                        return False
            else:
                printStr += line

                if (line[0] == "-"):
                        if (isTableReached == False):
                                isTableReached = True
                        else:
                                break

        try:
            masterLogName = os.path.join(masterLogFolder, os.path.basename(logName))
            log = open(masterLogName, "w")
            log.write(printStr)
            log.close()

        except IOError:
            print("Error while opening %s" % (masterLogName))
            exit(1)

def Run():
    lastLogFolder   = r"../Reports/LR.cfg_2010-10-09_12-08-37"
    masterLogFolder = r"../Reports/LR_master"

    if (lastLogFolder == ""):
        lastLogFolder = os.getcwd()

    if (os.path.exists(lastLogFolder) == False):
        print("folder %s does not exist" % (lastLogFolder))
        return

    if (os.path.exists(masterLogFolder) == False):
        print("folder %s does not exist" % (masterLogFolder))
        return

    for log in os.listdir(lastLogFolder):
        if (os.path.isfile(os.path.join(lastLogFolder, log)) and (".log" == os.path.splitext(log)[-1])):
            ExtractPFSTToFile(os.path.join(lastLogFolder, log), masterLogFolder)

Run()
