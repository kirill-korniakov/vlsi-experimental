from CoreFunctions import Logger

import ReportCreator
from ReportCreator import *

import BaseExperiment
from BaseExperiment import *

class ExperimentsComparator:
    logger = None
    storage = None
    experimentsToCompare = {} #Group of experiments. Their results will be compared
                              #experiment: {benchmark: pfst}

    def __init__(self, storage):
        self.storage = storage
        self.experimentsToCompare = {}

    def AddExperimentToGroup(self, newExperiment):
        if (self.experimentsToCompare != {}):
            groupExp = list(self.experimentsToCompare.keys())[0]

            if (newExperiment.benchmarks != groupExp.benchmarks):
                self.logger.Log("Error: list files are not equal!")
                exit(1)

            if (newExperiment.metrics != groupExp.metrics):
                self.logger.Log("Error: metrics are not equal!")
                exit(1)

        self.experimentsToCompare[newExperiment] = {}

    def GetExperimentsResults(self):
        for experiment in list(self.experimentsToCompare.keys()):
            if (experiment in self.storage.experimentResults):
                self.experimentsToCompare[experiment] = self.storage.experimentResults[experiment].pfstTables
            else:
                self.logger.Log("Error: results for %s not found" % (experiment.name))

    def CreateEmptyTable(self, resultFileName, metrics):
        #Create header of the table
        #---First string of the header------------------
        cols = [END_OF_COLUMN, "INIT"]

        for col in range(len(metrics)):
            cols.append(END_OF_COLUMN)

        for experiment in list(self.experimentsToCompare.keys()):
            cols.extend([END_OF_COLUMN, experiment.name])

            for col in range(len(metrics)):
                cols.append(END_OF_COLUMN)

        WriteStringToFile(cols, resultFileName)

        #---Second string of the header------------------
        cols = ['benchmark', END_OF_COLUMN]

        for row in range(len(metrics)):
            cols.extend([metrics[row], END_OF_COLUMN])

        cols.append(END_OF_COLUMN)

        for col in range(len(self.experimentsToCompare.keys())):
            for row in range(len(metrics)):
                cols.extend([metrics[row] + '%', END_OF_COLUMN])

            cols.append(END_OF_COLUMN)

        WriteStringToFile(cols, resultFileName)

    def MakeResultTable(self, resultFileName):
        groupExp = list(self.experimentsToCompare.keys())[0]
        self.CreateEmptyTable(resultFileName, groupExp.metrics)

        benchmarks = list(self.experimentsToCompare[groupExp].keys())
        for benchmark in benchmarks:
            newTableString = self.CreateTableString(benchmark, groupExp.metrics)
            WriteStringToFile(newTableString, resultFileName)

    def CreateTableString(self, benchmark, metrics):
            initialMetrics = []
            bestValues    = [1000000 for i in range(len(metrics))]
            bestValuesIdx = [0 for i in range(len(metrics))]

            tableStringContent = [benchmark, END_OF_COLUMN]
            initialIdx = len(tableStringContent) #index for metrics values on 'INIT' stage

            #Reserve positions for each metric on 'INIT' stage
            for col in range(len(metrics)):
                tableStringContent.extend(["N/A", END_OF_COLUMN])

            tableStringContent.append(END_OF_COLUMN)

            for experiment in list(self.experimentsToCompare.keys()):
                resultValues  = self.experimentsToCompare[experiment][benchmark]
                finalStageIdx = len(experiment.stages) - 1

                if (resultValues == []):
                    for col in range(len(metrics)):
                        tableStringContent.extend(["N/A", END_OF_COLUMN])
                else:
                    if (initialMetrics == []):
                        #if 'INITIAL' metrics haven't been printed yet
                        #take them from the table of this experiment
                        for col in range(len(metrics)):
                            tableStringContent[initialIdx + 2 * col] = str(resultValues[0][col])
                            initialMetrics.append(resultValues[0][col])
                    else:
                        #else compare 'INITIAL' metrics
                        for col in range(len(metrics)):
                            cmpResult = CompareValues(initialMetrics[col], resultValues[0][col])
                            if (cmpResult == 'notEqual' and metrics[col] != 'Time'):
                                self.logger.Log('Error: not equal Init metrics')

                    for col in range(len(metrics)):
                        percent = 100 * resultValues[finalStageIdx][col] / initialMetrics[col]
                        percentStr = "%.2f" % percent

                        if (percent < bestValues[col]):
                            bestValues[col] = percent #remember the best result
                            bestValuesIdx[col] = len(tableStringContent) #and its index

                        tableStringContent.extend([percentStr, END_OF_COLUMN])

                tableStringContent.append(END_OF_COLUMN)

            for idx in bestValuesIdx:
                if (idx > 0):
                    tableStringContent[idx] = MarkResultAsBest(tableStringContent[idx])

            return tableStringContent

    def CompareExperiments(self):
        if (len(self.experimentsToCompare) > 1):
            self.logger = Logger()
            self.logger.CoolLog("Comparing experiments")
            reportCreator = ReportCreator("Comparing", "Comparing " + GetTimeStamp())
            logFolder     = reportCreator.CreateLogFolder()
            cmpFileName   = reportCreator.GetReportTableName()
            self.GetExperimentsResults()
            self.MakeResultTable(cmpFileName)
            self.logger.Log("Comparing finished")
