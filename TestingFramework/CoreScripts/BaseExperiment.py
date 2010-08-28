import Parameters
from Parameters import *

import LogParser
from LogParser import *

OK      = 'Ok'
NEW     = 'New'
FAILED  = 'Failed'
CHANGED = 'Changed'

class BaseExperiment:
    name = ''
    cfg  = ''
    benchmarks = ''
    cmdLine = ''
    metrics = []
    stages  = []

    def __init__(self, name, cfg, benchmarks, metrics, stages, cmdLine = ''):
        self.name = name
        self.cfg = GeneralParameters.binDir + 'cfg/' + cfg
        self.benchmarks = GeneralParameters.benchmarkCheckoutPath + benchmarks
        self.cmdLine = cmdLine
        self.metrics = metrics
        self.stages  = stages

    def CreateEmptyTable(self, reportTable):
        printStr = 'Benchmark;'
        
        #write header of a table.
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                printStr += self.metrics[col] + '_' + self.stages[row] + ';'
                
            printStr += ';' #an empty column between metrics on different stages
                
        printStr += '\n'
        resultFile = open(reportTable, 'a')
        resultFile.write(printStr.replace('.', ','))
        resultFile.close()

    def ParseLog(self, logName, benchmark):
        parser = LogParser(logName)
        table = [[0 for col in range(len(self.metrics))] for row in range(len(self.stages))]
        
        for col in range(len(self.metrics)):
            for row in range(len(self.stages)):
                value = str(parser.GetFromPFST(self.stages[row], self.metrics[col]))
                
                if (value == str(NOT_FOUND)):
                   return []

                table[row][col] = float(value.replace(',', '.'))

        return table

    def FillTable(self, values, benchmark, reportTable):
        printStr = benchmark + ';'
        
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                printStr += str(values[row][col]) + ';'

            printStr += ';' #an empty column between metrics on different stages

        printStr += '\n'
        print(printStr)

        #write metrics to the file
        resultFile = open(reportTable, 'a')
        resultFile.write(printStr.replace('.', ','))
        resultFile.close()
    
    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        values = self.ParseLog(logName, benchmark)
        
        if (values == []):
          return FAILED
          
        self.FillTable(values, benchmark, reportTable)
        return OK
