import Parameters
from Parameters import *

NOT_FOUND = -1.0

class LogParser:
    logName = ''

    def __init__(self, logName, parameters = LogParserParameters()):
        self.logName = logName
        self.parameters = parameters

    def GetFromPFST(self, stageTag, colIdx):
        log = open(self.logName, 'r')
        lines = log.readlines()
        log.close()

        #move to the table
        lineIdx = 0
        for line in lines:
            if line.find(self.parameters.tableHeader) != -1:
                break
            else:
                lineIdx = lineIdx + 1

        if lineIdx == len(lines):
            print('Table not found')
            return NOT_FOUND

        #parse stages
        lineIdx = lineIdx + 3
        while (lines[lineIdx].find(self.parameters.borderPattern) == -1):
            if lines[lineIdx].find(stageTag) != -1:
                ll = lines[lineIdx].split()
                return ll[colIdx]
            lineIdx = lineIdx + 1
            if (lineIdx == len(lines)):
                return NOT_FOUND