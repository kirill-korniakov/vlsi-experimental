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

    #returns list: [status, errors]
    #status: True if parameters are ok, False otherwise
    #errors: description of errors if happen, '' if not
    def CheckParameters(self):
        #check if config file can be found
        if (not os.path.exists(self.cfg)):
            return ([False, 'file "' + self.cfg + '" not found'])

        #check if benchmarks list file can be found
        if (not os.path.exists(self.benchmarks)):
            return ([False, 'file "' + self.benchmarks + '" not found'])

        return ([True, ''])

    #returns list: [status, notFoundBenchmarks, benchmarks]
    #status: True if at least one benchmark was found, False otherwise
    #notFoundBenchmarksStr: string with benchmarks which were not found
    #benchmarks: list of benchmarks which were found
    def PrepareBenchmarks(self):
        notFoundBenchmarksStr = ''
        notFoundBenchmarks    = []
        benchmarks            = (open(self.benchmarks).read()).split('\n')

        # Perform filtering of empty lines and commented by # benchmarks
        benchmarks = [x for x in benchmarks if not x.strip().startswith('#')]
        benchmarks = [x for x in benchmarks if len(x.strip())]

        print("Benchmarks:\n" + ", ".join(benchmarks))
        print("\n")

        #check if all benchmarks can be found
        for i in range(len(benchmarks)):
            benchmark = os.path.dirname(os.path.abspath(self.benchmarks)) + "/" + benchmarks[i] + ".def"

            if (not os.path.exists(benchmark)):
                notFoundBenchmarks.append(benchmarks[i])

        #print and delete from list benchmarks which were not found
        if (notFoundBenchmarks != []):
            for benchmark in notFoundBenchmarks:
                benchmarks.remove(benchmark)
                notFoundBenchmarksStr += ' "' + benchmark + '";'

            notFoundBenchmarksStr += ' were not found!'
            print('Error: benchmarks ' + notFoundBenchmarksStr)

        nFoundBenchmarks = len(benchmarks)
        status           = (nFoundBenchmarks > 0)
        return (status, notFoundBenchmarksStr, benchmarks)

    #returns list: [status, errors, benchmarks]
    #status: True if parameters are ok, False otherwise
    #errors: description of errors if happen, '' if not
    #benchmarks: list of benchmarks which were found
    def CheckParametersAndPrepareBenchmarks(self):
        chekResult = self.CheckParameters()

        if (not chekResult[0]):
            chekResult.append([])
            return (chekResult)

        return self.PrepareBenchmarks()

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
        parser       = LogParser(logName)
        table        = parser.ParsePQAT(metrics)
        PQATFileName = os.path.dirname(logName) + '/' + os.path.basename(logName) + '.csv'
        PrintTableToFile(PQATFileName, table, metrics)

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
