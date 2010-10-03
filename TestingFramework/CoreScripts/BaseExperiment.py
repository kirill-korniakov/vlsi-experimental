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
    doParsePQAT = False

    def __init__(self, name, cfg, benchmarks, metrics, stages, cmdLine = ''):
        self.name = name
        self.cfg = GeneralParameters.binDir + 'cfg/' + cfg
        self.benchmarks = GeneralParameters.benchmarkCheckoutPath + benchmarks
        self.cmdLine = cmdLine
        self.metrics = metrics
        self.stages  = stages

    def CopyingConstructor(self, be):
        self.cfg         = be.cfg
        self.name        = be.name
        self.stages      = be.stages
        self.metrics     = be.metrics
        self.cmdLine     = be.cmdLine
        self.benchmarks  = be.benchmarks
        self.doParsePQAT = be.doParsePQAT

    def SetConfig(self, cfg):
        self.cfg = GeneralParameters.binDir + 'cfg/' + cfg

    def SetBenchmarksList(self, benchmarks):
        self.benchmarks = GeneralParameters.benchmarkCheckoutPath + benchmarks

    def CreateEmptyTable(self, reportTable):
        cols = ['Benchmark', END_OF_COLUMN]

        #write header of a table.
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append(self.metrics[col] + '_' + self.stages[row])
                cols.append(END_OF_COLUMN)

            cols.append(END_OF_COLUMN) #an empty column between metrics on different stages

        WriteStringToFile(cols, reportTable)

    def ParseLog(self, logName):
        parser = LogParser(logName)
        table = [[0 for col in range(len(self.metrics))] for row in range(len(self.stages))]

        for col in range(len(self.metrics)):
            for row in range(len(self.stages)):
                value = str(parser.GetFromTable(self.stages[row], self.metrics[col]))

                if (value == str(NOT_FOUND)):
                   return []

                table[row][col] = float(value.replace(',', '.'))

        return table

    def ParsePQATAndPrintTable(self, logName):
        metricsForChart   = ['HPWL', 'TNS', 'WNS']
        benchmarkFileName = os.path.dirname(logName) + '/' + os.path.basename(logName) + '.csv'
        cols = []

        for col in metricsForChart:
            cols += [col, END_OF_COLUMN]

        WriteStringToFile(cols, benchmarkFileName)

        parser    = LogParser(logName)
        currStage = 0

        while (True):
            cols = [str(currStage), END_OF_COLUMN]

            for col in range(len(metricsForChart)):
                value = str(parser.GetFromTable(str(currStage), metricsForChart[col], PQAT))

                if (value == str(NOT_FOUND)):
                    return

                cols += [value, END_OF_COLUMN]

            currStage += 1
            WriteStringToFile(cols, benchmarkFileName)

    def AddStringToTable(self, values, benchmark, reportTable):
        cols = [benchmark, END_OF_COLUMN]

        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols += [str(values[row][col]), END_OF_COLUMN]

            cols.append(END_OF_COLUMN) #an empty column between metrics on different stages

        #write metrics to the file
        WriteStringToFile(cols, reportTable)

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        values = self.ParseLog(logName)

        if (values == []):
          return [FAILED, []]

        self.AddStringToTable(values, benchmark, reportTable)

        if (self.doParsePQAT == True):
            self.ParsePQATAndPrintTable(logName)

        return [OK, values]
