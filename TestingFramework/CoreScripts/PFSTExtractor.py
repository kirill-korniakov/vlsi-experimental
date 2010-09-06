import os

NOT_FOUND = -1.0

def ExtractPFSTToFile(logName):
        tableHeader = 'Placement Flow Stages Table'
        
        log = open(logName, 'r')
        lines = log.readlines()
        log.close()
        isHeaderFound  = False
        isTableReached = False
        lineIdx = 0
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
                        return NOT_FOUND
            else:
                printStr += line

                if (line[0] == '-'):
                        if (isTableReached == False):
                                isTableReached = True
                        else:
                                break
                
        log = open(logName, 'w')
        log.write(printStr)
        log.close()

def Run(logFolder = ''):
        if (logFolder == ''):
            logFolder = os.getcwd()
        
        for log in os.listdir(logFolder):
                if (os.path.isfile(os.path.join(logFolder, log)) and ('.log' == os.path.splitext(log)[-1])):
                        ExtractPFSTToFile(log)
Run()
