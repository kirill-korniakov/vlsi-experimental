import Parameters
from Parameters import *

import LogParser
from LogParser import *

import CoreFunctions
from CoreFunctions import *

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
        cols = ['Benchmark']
        cols.append(EMPTYSITE)
        
        #write header of a table.
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append(self.metrics[col] + '_' + self.stages[row])
                cols.append(EMPTYSITE)
                
            cols.append(EMPTYSITE) #an empty column between metrics on different stages

        WriteStringToFile(cols, reportTable)

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
        cols = [benchmark]
        cols.append(EMPTYSITE)
        
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append(str(values[row][col]))
                cols.append(EMPTYSITE)

            cols.append(EMPTYSITE) #an empty column between metrics on different stages

        #write metrics to the file
        WriteStringToFile(cols, reportTable)

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        values = self.ParseLog(logName, benchmark)
        
        if (values == []):
          return FAILED
          
        self.FillTable(values, benchmark, reportTable)
        return OK
