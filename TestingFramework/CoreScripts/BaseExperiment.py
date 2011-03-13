import Parameters
from Parameters import *

import LogParser
from LogParser import *

import CoreFunctions
from CoreFunctions import *

OK      = "Ok"
NEW     = "New"
FAILED  = "Failed"
CHANGED = "Changed"

class ExperimentResults:
    errors           = []
    resultFile       = "" #file with result table
    pfstTables       = {} #benchmark: pfst
    benchmarkResults = {} #result: [benchmarks]

    def __init__(self):
        self.errors           = []
        self.resultFile       = ""
        self.pfstTables       = {}
        self.benchmarkResults = {}

    def GetPFSTForBenchmark(self, benchmark):
        return self.pfstTables[benchmark]

    def AddError(self, error):
        self.errors.append(error)

    def AddBenchmarkResult(self, benchmark, result):
        if (not result in self.benchmarkResults.keys()):
            self.benchmarkResults[result] = []

        self.benchmarkResults[result].append(benchmark)

    def AddPFSTForBenchmark(self, benchmark, table):
        self.pfstTables[benchmark] = table

    def AsString(self):
        resultStr = ""

        for result in list(self.benchmarkResults.keys()):
            resultStr += ("%s: %s benchmarks (" % (result, len(self.benchmarkResults[result])))

            for benchmark in (self.benchmarkResults[result]):
                resultStr += ("%s; " % (benchmark))

            resultStr += ")\n"

        for error in self.errors:
            resultStr += ("%s\n" % (error))

        return (resultStr + "\n")

    def Print(self):
        print(self.AsString())

class BaseExperiment:
    name = ''
    cfg  = ''
    benchmarks = ''
    cmdArgs = [] #list of command line arguments
    metrics = []
    stages  = []
    doParsePQAT = False

    def __init__(self, name, cfg, benchmarks, metrics, stages, cmdArgs = []):
        self.name = name
        self.cfg = GeneralParameters.binDir + 'cfg//' + cfg
        self.benchmarks = GeneralParameters.benchmarkCheckoutPath + benchmarks
        self.cmdArgs = cmdArgs
        self.metrics = metrics
        self.stages  = stages

    def CopyingConstructor(self, be):
        self.cfg         = be.cfg
        self.name        = be.name
        self.stages      = be.stages
        self.metrics     = be.metrics
        self.cmdArgs     = be.cmdArgs
        self.benchmarks  = be.benchmarks
        self.doParsePQAT = be.doParsePQAT

    def SetConfig(self, cfg):
        self.cfg = GeneralParameters.binDir + 'cfg//' + cfg

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
        return (parser.ParsePFST(self.metrics, self.stages))

    def ParsePQATAndPrintTable(self, logName):
        metrics      = ['HPWL', 'TNS', 'WNS']
        parser       = LogParser(logName, PQAT)
        table        = parser.ParsePQAT(metrics)
        table        = MakeTableInPercents(table)
        PQATFileName = os.path.dirname(logName) + '//' + os.path.basename(logName) + '.csv'
        PrintTableToFile(PQATFileName, table, metrics)

    def AddStringToTable(self, values, benchmark, reportTable):
        cols = [benchmark, END_OF_COLUMN]

        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols += [str(values[row][col]), END_OF_COLUMN]
            cols.append(END_OF_COLUMN) #an empty column between metrics on different stages

        #write metrics to the file
        WriteStringToFile(cols, reportTable)

    def MakeResultTable(self, logFolder, reportTable):
        if (os.path.exists(logFolder) == False):
            print('folder ' + logFolder + 'does not exist')
            return

        reportTable = logFolder + '//' + reportTable
        self.CreateEmptyTable(reportTable)

        for log in os.listdir(logFolder):
            if (os.path.isfile(os.path.join(logFolder, log)) and ('.log' == os.path.splitext(log)[-1])):
                benchmark = os.path.splitext(log)[0]
                self.ParseLogAndFillTable(logFolder + '//' + log, benchmark, reportTable)

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        values = self.ParseLog(logName)

        if (values == []):
          return [FAILED, []]

        self.AddStringToTable(values, benchmark, reportTable)

        if (self.doParsePQAT == True):
            self.ParsePQATAndPrintTable(logName)

        return [OK, values]
