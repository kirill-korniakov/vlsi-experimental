import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

NOT_FOUND = -1.0
PFST = 'PFST'
PQAT = 'PQAT'

class LogParser:
    logName = ''

    def __init__(self, logName, parameters = LogParserParameters()):
        self.logName = logName
        self.parameters = parameters

    def GetFromTable(self, stageTag, metricTag, tableType = PFST):
        log = open(self.logName, 'r')
        lines = log.readlines()
        log.close()

        #move to the table
        lineIdx       = 0
        tableHeader   = ''
        borderPattern = self.parameters.PFSTBorderPattern

        if (tableType == PFST):
            tableHeader = self.parameters.PFSTTableHeader

        else:
            tableHeader = self.parameters.PQATTableHeader

        for line in lines:
            if line.find(tableHeader) != -1:
                break
            else:
                lineIdx = lineIdx + 1

        if lineIdx == len(lines):
            print(tableHeader + ' not found in log file')
            return NOT_FOUND

        #find colIdx
        metrics = lines[lineIdx+1].split()
        colIdx = 0
        for metric in metrics:
            if metrics[colIdx].find(metricTag) == 0:
                break
            else:
                colIdx = colIdx + 1
        if colIdx == len(metrics):
            print('Tag ' + metricTag + ' not found')
            return NOT_FOUND

        #parse stages
        lineIdx = lineIdx + 3
        while ((lines[lineIdx].find(borderPattern) == -1) and (lines[lineIdx] != '\n')):
            if lines[lineIdx].find(' ' + stageTag + ' ') != -1:
                ll = lines[lineIdx].split()
                return ll[colIdx]
            lineIdx = lineIdx + 1
            if lineIdx == len(lines):
                print('Stage ' + stageTag + ' not found')
                return NOT_FOUND

        return NOT_FOUND

    def ParsePQAT(self, metrics):
        currStage = 0
        table = {}

        while (True):
            table[currStage] = []

            for col in range(len(metrics)):
                value = str(self.GetFromTable(str(currStage), metrics[col], PQAT))
                value = float(value.replace(',', '.'))

                if (value == NOT_FOUND):
                    del table[currStage] #delete empty field
                    return table

                if (currStage > 0):
                    if (table[0][col] > 0):
                        value = value * 100 / table[0][col]

                table[currStage].append(value)

            currStage += 1

        return table

    def ParsePQATAndPrintTable(self, metrics):
        benchmarkFileName = os.path.dirname(self.logName) + '/' + os.path.basename(self.logName) + '.csv'
        cols = ['stage', END_OF_COLUMN]

        for col in metrics:
            cols += [col, END_OF_COLUMN]

        WriteStringToFile(cols, benchmarkFileName)
        table = self.ParsePQAT(metrics)

        for currStage in table.keys():
            cols = [str(currStage), END_OF_COLUMN]

            for col in range(len(metrics)):
                cols += [str(table[currStage][col]), END_OF_COLUMN]

            WriteStringToFile(cols, benchmarkFileName)

        return table

    def GetStageTag(self):
        log = open(self.logName, 'r')
        lines = log.readlines()
        log.close()
        lineIdx = 0
        linesCount = 0
        for line in lines:
            linesCount = linesCount + 1

        for line in lines:
            if line.find(tableHeader) != -1:

                metrics = lines[lineIdx+1].split()
                colIdx = 0
                metricTag = 'Tag';
                for metric in metrics:
                    if metrics[colIdx].find(metricTag) == 0:
                        break
                    else:
                        colIdx = colIdx + 1
                if colIdx == len(metrics):
                    print('Tag ' + metricTag + ' not found')
                    return NOT_FOUND

                lineIdx = lineIdx + 3
                tagId = 0

                columCount = 0;
                i = lineIdx
                while ((lines[i].find(self.parameters.borderPattern) == -1) and (lines[i].find("Thats All") == -1) and (i <= linesCount)):
                    columCount = columCount + 1
                    i = i + 1

                tagList = ['' for i in range(columCount)]
                while ((lines[lineIdx].find(self.parameters.borderPattern) == -1) and (lines[i].find("Thats All") == -1) and (i <= linesCount)):
                    ll = lines[lineIdx].split()
                    tagList[tagId] = ll[colIdx]
                    tagId = tagId + 1
                    lineIdx = lineIdx + 1

                res = [tagId, tagList]
                return res
            else:
                lineIdx = lineIdx + 1
        if lineIdx == len(lines):
            print(tableHeader + ' not found in log file')
            return NOT_FOUND

