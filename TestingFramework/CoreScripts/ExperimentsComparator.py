import CoreFunctions
from CoreFunctions import *

import ReportCreator
from ReportCreator import *

import BaseExperiment
from BaseExperiment import *

class ExperimentsComparator:
    storage              = None
    experimentsToCompare = {} #Group of experiments. Their results will be compared
                              #experiment: {benchmark: pfst}

    def __init__(self, storage):
        self.storage              = storage
        self.experimentsToCompare = {}

    def AddExperimentToGroup(self, newExperiment):
        if (self.experimentsToCompare != {}):
            groupExp = list(self.experimentsToCompare.keys())[0]

            if (newExperiment.benchmarks != groupExp.benchmarks):
                print("Error: list files are not equal!")
                exit(1)

            if (newExperiment.metrics != groupExp.metrics):
                print("Error: metrics are not equal!")
                exit(1)

        self.experimentsToCompare[newExperiment] = {}

    def GetExperimentsResults(self):
        for experiment in list(self.experimentsToCompare.keys()):
            if (experiment in self.storage.experimentResults):
                self.experimentsToCompare[experiment] = self.storage.experimentResults[experiment].pfstTables

            else:
                print("Error: result for %s not found" % (experiment.name))

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
        metrics  = groupExp.metrics
        self.CreateEmptyTable(resultFileName, metrics)

        #------Print results-------------------------------------
        for benchmark in list(self.experimentsToCompare[groupExp].keys()):
            initialMetrics = []
            bestMetrics    = [1000000 for i in range(len(metrics))]
            bestMetricsIdx = [0 for i in range(len(metrics))]

            cols       = [benchmark, END_OF_COLUMN]
            initialIdx = len(cols) #index for metrics on 'INIT' stage

            #Reserve positions for each metric on 'INIT' stage
            for col in range(len(metrics)):
                cols.extend(["N/A", END_OF_COLUMN])

            cols.append(END_OF_COLUMN)

            for experiment in list(self.experimentsToCompare.keys()):
                resultValues  = self.experimentsToCompare[experiment][benchmark]
                finalStageIdx = len(experiment.stages) - 1

                if (resultValues == []):
                    for col in range(len(metrics)):
                        cols.extend(["N/A", END_OF_COLUMN])

                else:
                    #if 'INITIAL' metrics haven't been printed yet
                    if (initialMetrics == []):
                        #take them from the table of this experiment
                        for col in range(len(metrics)):
                            cols[initialIdx + 2 * col] = str(resultValues[0][col])
                            initialMetrics.append(resultValues[0][col])

                    #else compare 'INITIAL' metrics
                    else:
                        for col in range(len(metrics)):
                            cmpResult = CompareValues(initialMetrics[col], resultValues[0][col])

                            if (cmpResult == 'notEqual'):
                                print('Error: not equal Init metrics')

                    for col in range(len(metrics)):
                        percent = 100 * resultValues[finalStageIdx][col] / initialMetrics[col]
                        percentStr = "%.2f" % percent

                        if (percent < bestMetrics[col]):
                            bestMetrics[col]    = percent   #remember best result
                            bestMetricsIdx[col] = len(cols) #and index

                        cols.extend([percentStr, END_OF_COLUMN])

                cols.append(END_OF_COLUMN)

            for idx in bestMetricsIdx:
                if (idx > 0):
                    cols[idx] = MarkResultAsBest(cols[idx])

            WriteStringToFile(cols, resultFileName)

    def CompareExperiments(self):
        if (len(self.experimentsToCompare) > 1):
            cp = CoolPrinter()
            cp.CoolPrint("Comaparing experiments")
            reportCreator = ReportCreator("Comaparing", "Comparing")
            logFolder     = reportCreator.CreateLogFolder()
            cmpFileName   = reportCreator.GetReportTableName()
            self.GetExperimentsResults()
            self.MakeResultTable(cmpFileName)
