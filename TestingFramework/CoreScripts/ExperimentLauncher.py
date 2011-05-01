import subprocess
import sys
import os

import datetime
from datetime import date
import time

import BaseExperiment
from BaseExperiment import *

import ResultsStorage
from ResultsStorage import *

import Emailer
from Emailer import *

import ReportCreator
from ReportCreator import *

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

TERMINATED = "Terminated"
nTerminatedBenchmarks = 0 #TODO: check if everything is OK

class ExperimentLauncher:
    logger            = None
    emailer           = None
    experiment        = None
    resultsStorage    = None
    experimentResults = None

    def __init__(self, experiment, resultsStorage, emailer):
        self.logger            = Logger()
        self.emailer           = emailer
        self.experiment        = experiment
        self.resultsStorage    = resultsStorage
        self.experimentResults = ExperimentResults()

    def AddErrorToResults(self, error):
        self.logger.Log(error)
        self.experimentResults.AddError(error)

    def CheckParameters(self):
        #check if config file can be found
        if (not os.path.exists(self.experiment.cfg)):
            error = "Error: file '%s' not found" % (self.experiment.cfg)
            self.AddErrorToResults(error)
            return False

        #check if benchmarks list file can be found
        if (not os.path.exists(self.experiment.benchmarks)):
            error = "Error: file '%s' not found" % (self.experiment.benchmarks)
            self.AddErrorToResults(error)
            return False

        return True

    def PrepareBenchmarks(self):
        notFoundBenchmarksStr = ''
        notFoundBenchmarks    = []
        benchmarks            = (open(self.experiment.benchmarks).read()).split('\n')

        # Perform filtering of empty lines and commented by # benchmarks
        benchmarks = [x for x in benchmarks if not x.strip().startswith('#')]
        benchmarks = [x for x in benchmarks if len(x.strip())]
        self.logger.Log("\n  * %s\n" % ("\n  * ".join(benchmarks)))

        #check if all benchmarks can be found
        for i in range(len(benchmarks)):
            benchmark = os.path.dirname(os.path.abspath(self.experiment.benchmarks)) + "//" + benchmarks[i] + ".def"

            if (not os.path.exists(benchmark)):
                notFoundBenchmarks.append(benchmarks[i])

        #print and delete from list benchmarks which were not found
        if (notFoundBenchmarks != []):
            for benchmark in notFoundBenchmarks:
                benchmarks.remove(benchmark)
                notFoundBenchmarksStr += ' "' + benchmark + '";'

            error = "Error: benchmarks %s were not found!" % (notFoundBenchmarksStr)
            self.AddErrorToResults(error)

        nFoundBenchmarks = len(benchmarks)
        return benchmarks

    def CheckParametersAndPrepareBenchmarks(self):
        if (not self.CheckParameters()):
            return []

        return self.PrepareBenchmarks()

    def RunExperiment(self):
        global nTerminatedBenchmarks

        self.logger.Log("Config: %s" % self.experiment.cfg)
        self.logger.Log("Benchmarks: %s" % self.experiment.benchmarks)

        reportCreator = ReportCreator(self.experiment.name, self.experiment.cfg)
        logFolder = reportCreator.CreateLogFolder()
        reportTable = reportCreator.GetReportTableName()

        self.experiment.CreateEmptyTable(reportTable)

        benchmarks = self.CheckParametersAndPrepareBenchmarks()
        if (benchmarks == []):
            self.resultsStorage.AddExperimentResult(self.experiment, self.experimentResults)
            return

        nTerminatedBenchmarks = 33
        for benchmark in benchmarks:
            self.RunPlacer(benchmark, logFolder, reportTable)
            self.logger.Log("[%s/%s] %s is finished\n" % (benchmarks.index(benchmark) + 1, len(benchmarks), benchmark))

        self.resultsStorage.AddExperimentResult(self.experiment, self.experimentResults)

    def RunPlacer(self, benchmark, logFolder, reportTable):
        global nTerminatedBenchmarks

        self.experimentResults.AddPFSTForBenchmark(benchmark, [])
        logFileName   = logFolder + "//" + os.path.basename(benchmark) + ".log"
        fPlacerOutput = open(logFileName, 'w')
        resultValues  = []

        defFile = "--params.def=" + os.path.dirname(os.path.abspath(self.experiment.benchmarks))\
                  + "//" + benchmark + ".def"

        benchmarkDirectory = os.path.abspath(logFolder + "//" + os.path.basename(benchmark))
        pixDirectory       = os.path.abspath(logFolder + "//" + os.path.basename(benchmark) + "//pix")
        pixDirParam        = "--plotter.pixDirectory=" + pixDirectory

        milestonePixDirectory = os.path.abspath(pixDirectory + "//milestones")

        if (os.path.exists(milestonePixDirectory) != True):
            os.makedirs(milestonePixDirectory)

        milestonePixDirParam = "--plotter.milestonePixDirectory=" + milestonePixDirectory
        exeName = GeneralParameters.binDir + "itlPlaceRelease.exe"

        params = [exeName, os.path.abspath(self.experiment.cfg),\
                  defFile, pixDirParam, milestonePixDirParam]
        params.extend(self.experiment.cmdArgs)

        #HACK: ugly hack for ISPD04 benchmarks
        if self.experiment.cfg.find("ispd04") != -1:
            lefFile = "--params.lef=" + os.path.dirname(os.path.abspath(self.experiment.benchmarks))\
                      + "//" + benchmark + ".lef"

            params.append(lefFile)

        benchmarkResult = ""

        try:
            p = subprocess.Popen(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)

        except WindowsError:
            error = "Error: can not call %s" % (exeName)
            ReportErrorAndExit(error, self.logger, self.emailer)

        t_start = time.time()
        seconds_passed = 0

        renamed = False
        time.sleep(1)

        while(renamed == False and seconds_passed < GeneralParameters.maxTimeForBenchmark):
            seconds_passed = time.time() - t_start

            try:
                renamed = open(GeneralParameters.binDir + "itlPlaceRelease.exe", 'a')
                renamed.close()
                #renamed = os.rename(exeName, exeName + "_")
                #renamed = os.rename(exeName + "_", exeName)

            except IOError:
                renamed = False

        retcode = p.poll()
        self.logger.Log("Seconds passed: %.2f" % (seconds_passed))
        if retcode != 0:
            self.logger.Log("Process retcode: %s" % (retcode))

        if (retcode == None):
            self.logger.Log("Time out on %s" % (benchmark))
            nTerminatedBenchmarks += 1
            self.experimentResults.AddBenchmarkResult(benchmark, TERMINATED)

            try:
                p.terminate()
            except Exception:
                pass

            if (nTerminatedBenchmarks >= 3):
                self.AddErrorToResults("Reached maximum number of terminated benchmarks")
                self.resultsStorage.AddExperimentResult(self.experiment, self.experimentResults)
                return
        else:
            (result, resultValues) = \
                    self.experiment.ParseLogAndFillTable(logFileName, benchmark, reportTable)

            self.experimentResults.AddPFSTForBenchmark(benchmark, resultValues)
            self.experimentResults.AddBenchmarkResult(benchmark, result)

            if (result == CHANGED):
                self.experimentResults.resultFile = reportTable

        fPlacerOutput.close()
        #self.experimentResults.Print() #testing only

def test():
    pass

if __name__ == '__main__':
    test()