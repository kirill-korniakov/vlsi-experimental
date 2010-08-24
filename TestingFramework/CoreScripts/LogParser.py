import Parameters
from Parameters import *

NOT_FOUND = -1.0

class LogParser:
    logName = ''

    def __init__(self, logName, parameters = LogParserParameters()):
        self.logName = logName
        self.parameters = parameters

    def GetFromPFST(self, stageTag, metricTag):
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
            print(self.parameters.tableHeader + ' not found in log file')
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
        while lines[lineIdx].find(self.parameters.borderPattern) == -1:
            if lines[lineIdx].find(stageTag) != -1:
                ll = lines[lineIdx].split()
                return ll[colIdx]
            lineIdx = lineIdx + 1
            if lineIdx == len(lines):
                print('Stage ' + stageTag + ' not found')
                return NOT_FOUND

    def GetStageTag(self):
        log = open(self.logName, 'r')
        lines = log.readlines()
        log.close()
        lineIdx = 0
        for line in lines:
            if line.find(self.parameters.tableHeader) != -1:

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
                while ((lines[i].find(self.parameters.borderPattern) == -1) and (lines[i].find("Thats All") == -1)):
                    columCount = columCount + 1
                    i = i + 1
                
                tagList = ['' for i in range(columCount)]
                while ((lines[lineIdx].find(self.parameters.borderPattern) == -1) and (lines[i].find("Thats All") == -1)):
                    ll = lines[lineIdx].split()
                    tagList[tagId] = ll[colIdx]
                    tagId = tagId + 1
                    lineIdx = lineIdx + 1
                
                res = [tagId, tagList]
                return res
            else:
                lineIdx = lineIdx + 1
        if lineIdx == len(lines):
            print(self.parameters.tableHeader + ' not found in log file')
            return NOT_FOUND
        
