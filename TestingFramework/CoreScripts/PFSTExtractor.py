import os

def ExtractPFSTToFile(logName, masterLogFolder):
        tableHeader = 'Placement Flow Stages Table'
        log = open(logName, 'r')
        lines = log.readlines()
        log.close()
        isHeaderFound  = False
        isTableReached = False
        lineIdx  = 0
        printStr = ''

        for line in lines:
            if (isHeaderFound == False):
                if line.find(tableHeader) != -1:
                    isHeaderFound = True
                    printStr += line

                else:
                    lineIdx += 1

                    if lineIdx == len(lines):
                        print('PFST was not found in ' + logName + '\n')
                        return False
            else:
                printStr += line

                if (line[0] == '-'):
                        if (isTableReached == False):
                                isTableReached = True
                        else:
                                break

        try:
            masterLogName = masterLogFolder + '/' + os.path.basename(logName)
            log = open(masterLogName, 'w')
            log.write(printStr)
            log.close()

        except IOError:
            print('Error while opening ' + masterLogName)
            exit(1)

def Run():
    lastLogFolder   = '../Reports/LR.cfg_2010-10-09_12-08-37'
    masterLogFolder = '../Reports/LR_master'

    if (lastLogFolder == ''):
        lastLogFolder = os.getcwd()

    if (os.path.exists(lastLogFolder) == False):
        print('folder ' + lastLogFolder + ' does not exist')
        return

    if (os.path.exists(masterLogFolder) == False):
        print('folder ' + masterLogFolder + ' does not exist')
        return

    for log in os.listdir(lastLogFolder):
        if (os.path.isfile(os.path.join(lastLogFolder, log)) and ('.log' == os.path.splitext(log)[-1])):
            ExtractPFSTToFile(lastLogFolder + '/' + log, masterLogFolder)

Run()
