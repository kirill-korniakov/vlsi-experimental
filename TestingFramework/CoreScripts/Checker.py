import os
from Logger import Logger
import CoreScripts
from CoreScripts.BaseExperiment import OK, CHANGED, NEW, FAILED, BaseExperiment
from CoreScripts.CoreFunctions import SAME, EQUAL, NOT_EQUAL, CompareValues, WriteStringToFile

class Checker(BaseExperiment):
    masterLogFolder = ""

    def __init__(self, baseExperimnet, masterLogFolder):
        BaseExperiment.CopyingConstructor(self, baseExperimnet)
        self.masterLogFolder = masterLogFolder

    def CompareTables(self, table1, table2, eps = 0.001):
        for col in range(len(self.metrics)):
            for row in range(len(self.stages)):
                compare_result = CompareValues(table1[row][col], table2[row][col])

                if (compare_result == NOT_EQUAL):
                  return(CHANGED)

        return(OK)

    def CreateEmptyTable(self, reportTable):
        cols = ["Benchmark"]

        #write header of a table.
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append("Current %s_%s" % (self.metrics[col], self.stages[row]))
                cols.append("Master %s_%s" % (self.metrics[col], self.stages[row]))

            cols.append("") #an empty column between metrics on different stages

        WriteStringToFile(cols, reportTable)

    def AddStringToTable(self, currentValues, masterValues, benchmark, reportTable):
        cols = [benchmark]

        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append(str(currentValues[row][col]))
                cols.append(str(masterValues[row][col]))

            cols.append("") #an empty column between metrics on different stages

        #write metrics to the file
        WriteStringToFile(cols, reportTable)

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        currentValues = self.ParseLog(logName)

        if (currentValues == []):
          return [FAILED, []]

        masterLogName = os.path.join(self.masterLogFolder, os.path.basename(logName))
        masterValues  = self.ParseLog(masterLogName)

        if (masterValues == []):
            logger = Logger()
            logger.Log("Experiment has not failed but master log is empty or does not exist\n")
            return [NEW, currentValues]

        self.AddStringToTable(currentValues, masterValues, benchmark, reportTable)

        if (self.doParsePQAT == True):
            self.ParsePQATAndPrintTable(logName)

        return [self.CompareTables(currentValues, masterValues), currentValues]
