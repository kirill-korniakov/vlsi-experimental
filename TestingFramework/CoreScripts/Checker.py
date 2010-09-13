import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Checker(BaseExperiment):
    masterLogFolder = ''

    def __init__(self, baseExperimnet, masterLogFolder):
        BaseExperiment.CopyingConstructor(self, baseExperimnet)
        self.masterLogFolder = masterLogFolder

    def CompareTables(self, table1, table2, eps = 0.001):
        for col in range(len(self.metrics)):
            for row in range(len(self.stages)):
                compare_result = CompareValues(table1[row][col], table2[row][col])

                if (compare_result == 'notEqual'):
                  return(CHANGED)

        return(OK)

    def CreateEmptyTable(self, reportTable):
        cols = ['Benchmark']
        cols.append(END_OF_COLUMN)

        #write header of a table.
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append('Current ' + self.metrics[col] + '_' + self.stages[row])
                cols.append(END_OF_COLUMN)
                cols.append('Master '  + self.metrics[col] + '_' + self.stages[row])
                cols.append(END_OF_COLUMN)

            cols.append(END_OF_COLUMN) #an empty column between metrics on different stages

        WriteStringToFile(cols, reportTable)

    def AddStringToTable(self, currentValues, masterValues, benchmark, reportTable):
        cols = [benchmark]
        cols.append(END_OF_COLUMN)

        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append(str(currentValues[row][col]))
                cols.append(END_OF_COLUMN)
                cols.append(str(masterValues[row][col]))
                cols.append(END_OF_COLUMN)

            cols.append(END_OF_COLUMN) #an empty column between metrics on different stages

        #write metrics to the file
        WriteStringToFile(cols, reportTable)

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        currentValues = self.ParseLog(logName, benchmark)

        if (currentValues == []):
          return FAILED

        masterLogName = self.masterLogFolder + "/" + os.path.basename(logName)
        masterValues  = self.ParseLog(masterLogName, benchmark)

        if (masterValues == []):
            print('experiment has not failed but master log is empty\n')
            return NEW

        self.AddStringToTable(currentValues, masterValues, benchmark, reportTable)
        self.PrintBenchmarkTable(values, benchmark, reportTable)
        return self.CompareTables(currentValues, masterValues)