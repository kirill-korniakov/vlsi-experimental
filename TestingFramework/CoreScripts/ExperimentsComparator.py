from CoreFunctions import Logger

import ReportCreator
from ReportCreator import *

import BaseExperiment
from BaseExperiment import *

referenceExperimentIdx = 0

class ExperimentsComparator:
    global referenceExperimentIdx

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

    def PrintLabelAndBlanks(self, cols, label, nMetrics):
        cols.extend([END_OF_COLUMN, label])
        for col in nMetrics:
            cols.append(END_OF_COLUMN)

    def PrintTableHeader(self, resultFileName, metrics):
        nMetrics = range(len(metrics))

        # First line of the table header
        cols = []

        self.PrintLabelAndBlanks(cols, "INIT", nMetrics)

        referenceExperiment = list(self.experimentsToCompare.keys())[referenceExperimentIdx]
        self.PrintLabelAndBlanks(cols, referenceExperiment.name, nMetrics)

        for experiment in self.experimentsToCompare.keys():
            if (list(self.experimentsToCompare.keys()).index(experiment) == referenceExperimentIdx):
                continue
            self.PrintLabelAndBlanks(cols, experiment.name, nMetrics)

        WriteStringToFile(cols, resultFileName)

        # Second line of the table header
        cols = ['benchmark', END_OF_COLUMN]

        for row in nMetrics:
            cols.extend([metrics[row], END_OF_COLUMN])
        cols.append(END_OF_COLUMN)

        for row in nMetrics:
            cols.extend([metrics[row], END_OF_COLUMN])
        cols.append(END_OF_COLUMN)

        for experiment in self.experimentsToCompare.keys():
            if list(self.experimentsToCompare.keys()).index(experiment) == referenceExperimentIdx:
                continue
            for row in nMetrics:
                if (metrics[row] != 'Time'):
                    cols.extend([metrics[row] + '%', END_OF_COLUMN])

                else:
                    cols.extend([metrics[row], END_OF_COLUMN])

            cols.append(END_OF_COLUMN)

        WriteStringToFile(cols, resultFileName)

    def MakeResultTable(self, resultFileName):
        groupExp = list(self.experimentsToCompare.keys())[0]
        self.PrintTableHeader(resultFileName, groupExp.metrics)

        benchmarks = self.experimentsToCompare[groupExp].keys()
        for benchmark in benchmarks:
            newTableString = self.CreateNewTableLine(benchmark, groupExp.metrics)
            WriteStringToFile(newTableString, resultFileName)

    def CreateNewTableLine(self, benchmark, metrics):
            nMetrics = range(len(metrics))
            bestValues = [1000000 for i in nMetrics]
            bestValuesIdx = [0 for i in nMetrics]

            newTableLine = [benchmark, END_OF_COLUMN]

            # INIT metrics
            initialMetrics = []
            resultValues = list(self.experimentsToCompare.values())[0][benchmark]
            for metricIdx in nMetrics:
                value = resultValues[0][metricIdx]
                newTableLine.extend([str(value), END_OF_COLUMN])
                initialMetrics.append(value)
            newTableLine.append(END_OF_COLUMN)

            # reference metrics
            referenceMetrics = []
            experiment = list(self.experimentsToCompare.keys())[referenceExperimentIdx]
            resultValues = self.experimentsToCompare[experiment][benchmark]
            for metricIdx in nMetrics:
                finalStageIdx = len(experiment.stages) - 1
                value = resultValues[finalStageIdx][metricIdx]
                newTableLine.extend([str(value), END_OF_COLUMN])
                referenceMetrics.append(value)
            newTableLine.append(END_OF_COLUMN)

            # normal experiments
            for experiment in self.experimentsToCompare.keys():
                if list(self.experimentsToCompare.keys()).index(experiment) == referenceExperimentIdx:
                    continue

                resultValues = self.experimentsToCompare[experiment][benchmark]
                if (resultValues == []):
                    for metricIdx in nMetrics:
                        newTableLine.extend(["N/A", END_OF_COLUMN])
                    newTableLine.append(END_OF_COLUMN)
                    continue

                #compare 'INITIAL' metrics
                for metricIdx in nMetrics:
                    cmpResult = CompareValues(initialMetrics[metricIdx], resultValues[0][metricIdx])
                    if (cmpResult == NOT_EQUAL and metrics[metricIdx] != 'Time'):
                        self.logger.Log('Error: not equal INIT metrics')

                finalStageIdx = len(experiment.stages) - 1
                for metricIdx in nMetrics:
                    valueStr = resultValues[finalStageIdx][metricIdx]

                    if (metrics[metricIdx] != 'Time'):
                        percent = 100 * resultValues[finalStageIdx][metricIdx] / referenceMetrics[metricIdx]
                        valueStr = "%.2f" % percent

                    if (percent < bestValues[metricIdx]):
                        bestValues[metricIdx] = percent #remember the best result
                        bestValuesIdx[metricIdx] = len(newTableLine) #and its index

                    newTableLine.extend([valueStr, END_OF_COLUMN])

                newTableLine.append(END_OF_COLUMN)

            #for idx in bestValuesIdx:
            #    if (idx > 0):
            #        newTableLine[idx] = MarkResultAsBest(newTableLine[idx])

            return newTableLine

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
