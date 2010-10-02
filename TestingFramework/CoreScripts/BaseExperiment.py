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

    def CopyingConstructor(self, be):
        self.cfg        = be.cfg
        self.name       = be.name
        self.stages     = be.stages
        self.metrics    = be.metrics
        self.cmdLine    = be.cmdLine
        self.benchmarks = be.benchmarks

    def SetConfig(self, cfg):
        self.cfg = GeneralParameters.binDir + 'cfg/' + cfg

    def SetBenchmarksList(self, benchmarks):
        self.benchmarks = GeneralParameters.benchmarkCheckoutPath + benchmarks

    def CreateEmptyTable(self, reportTable):
        cols = ['Benchmark']
        cols.append(END_OF_COLUMN)

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

    def ParsePQAT(self, logName):
        parser = LogParser(logName)
        metricsForChart = ['HPWL', 'TNS']
        value = str(parser.GetFromTable('10', 'HPWL', PQAT))
        print(value)

    def PrintBenchmarkTable(self, values, benchmark, reportTable):
        return 0

    def AddStringToTable(self, values, benchmark, reportTable):
        cols = [benchmark]
        cols.append(END_OF_COLUMN)

        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append(str(values[row][col]))
                cols.append(END_OF_COLUMN)

            cols.append(END_OF_COLUMN) #an empty column between metrics on different stages

        #write metrics to the file
        WriteStringToFile(cols, reportTable)

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        values = self.ParseLog(logName)

        if (values == []):
          return [FAILED, []]

        self.AddStringToTable(values, benchmark, reportTable)
        self.PrintBenchmarkTable(values, benchmark, reportTable)
        self.ParsePQAT(logName)
        return [OK, values]
