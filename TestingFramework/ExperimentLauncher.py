import BaseExperiment
from BaseExperiment import *

import subprocess
import sys
import os

import datetime
from datetime import date
import time

from Emailer import *
from SvnWorker import *
from ReportCreator import *

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

#TERMINATED = 'Terminated'

class ExperimentLauncher:
    experiment
    resultsStorage
    experimentResults = ExperimentResults()

    def __init__(self, experiment, resultsStorage):
        self.experiment     = experiment
        self.resultsStorage = resultsStorage

    def AddErrorToResults(self, error):
        print(error)
        experimentResults.AddError(error)

    def CheckParameters(self):
        #check if config file can be found
        if (not os.path.exists(experiment.cfg)):
            error = "Error: file '%s' not found" % (experiment.cfg)
            AddErrorToResults(error)
            return False

        #check if benchmarks list file can be found
        if (not os.path.exists(experiment.benchmarks)):
            error = "Error: file '%s' not found" % (experiment.benchmarks)
            AddErrorToResults.AddError(error)
            return False

        return True

    #returns list: [status, notFoundBenchmarks, benchmarks]
    #status: True if at least one benchmark was found, False otherwise
    #notFoundBenchmarksStr: string with benchmarks which were not found
    #benchmarks: list of benchmarks which were found
    def PrepareBenchmarks(self):
        notFoundBenchmarksStr = ''
        notFoundBenchmarks    = []
        benchmarks            = (open(experiment.benchmarks).read()).split('\n')

        # Perform filtering of empty lines and commented by # benchmarks
        benchmarks = [x for x in benchmarks if not x.strip().startswith('#')]
        benchmarks = [x for x in benchmarks if len(x.strip())]
        print("Benchmarks:\n%s\n" % (", ".join(benchmarks)))

        #check if all benchmarks can be found
        for i in range(len(benchmarks)):
            benchmark = os.path.dirname(os.path.abspath(experiment.benchmarks)) + "//" + benchmarks[i] + ".def"

            if (not os.path.exists(benchmark)):
                notFoundBenchmarks.append(benchmarks[i])

        #print and delete from list benchmarks which were not found
        if (notFoundBenchmarks != []):
            for benchmark in notFoundBenchmarks:
                benchmarks.remove(benchmark)
                notFoundBenchmarksStr += ' "' + benchmark + '";'

            error = "Error: benchmarks %s were not found!" % (notFoundBenchmarksStr)
            AddErrorToResults.AddError(error)

        nFoundBenchmarks = len(benchmarks)
        status           = (nFoundBenchmarks > 0)
        return benchmarks

    def CheckParametersAndPrepareBenchmarks(self):
        if (not CheckParameters()):
            return []

        return self.PrepareBenchmarks()

    def RunExperiment():
        print('Config: %s' % experiment.cfg)
        print('List:   %s' % experiment.benchmarks)

        reportCreator = ReportCreator(experiment.name, experiment.cfg)
        logFolder     = reportCreator.CreateLogFolder()
        reportTable   = reportCreator.GetReportTableName()
        experiment.CreateEmptyTable(reportTable)

        benchmarks = self.CheckParametersAndPrepareBenchmarks()

        if (benchmarks == []):
            experimentResults.result = FAILED
            return (reportTable)

        ##self.experimentResult = OK
        nTerminatedBenchmarks = 0

        for benchmark in benchmarks:
            logFileName   = logFolder + "//" + os.path.basename(benchmark) + ".log"
            fPlacerOutput = open(logFileName, 'w')
            resultValues  = []

            defFile = "--params.def=" + os.path.dirname(os.path.abspath(experiment.benchmarks))\
                      + "//" + benchmark + ".def"

            benchmarkDirectory = os.path.abspath(logFolder + "//" + os.path.basename(benchmark))
            pixDirectory       = os.path.abspath(logFolder + "//" + os.path.basename(benchmark) + "//pix")
            pixDirParam        = "--plotter.pixDirectory=" + pixDirectory

            #if (os.path.exists(benchmarkDirectory) != True):##
            #    os.mkdir(benchmarkDirectory)

            #if (os.path.exists(pixDirectory) != True):
            #    os.makedirs(pixDirectory)

            milestonePixDirectory = pixDirectory + "//milestones"

            if (os.path.exists(milestonePixDirectory) != True):
                os.makedirs(milestonePixDirectory)

            milestonePixDirParam  = "--plotter.milestonePixDirectory=" + milestonePixDirectory

            params = [GeneralParameters.binDir + "itlPlaceRelease.exe", os.path.abspath(experiment.cfg),\
                      defFile, experiment.cmdLine, pixDirParam, milestonePixDirParam]
            #HACK: ugly hack for ISPD04 benchmarks
            if experiment.cfg.find("ispd04") != -1:
                lefFile = "--params.lef=" + os.path.dirname(os.path.abspath(experiment.benchmarks))\
                          + "//" + benchmark + ".lef"

                params.append(lefFile)

            benchmarkResult = ''

            try:
                p = subprocess.Popen(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)

            except WindowsError:
                ###print('Error: can not call ' + GeneralParameters.binDir + 'itlPlaceRelease.exe')
                print("Error: can not call %s itlPlaceRelease.exe" % (GeneralParameters.binDir))
                self.SendErrorMessageAndExit('Night experiments', 'can not start itlPlaceRelease.exe')

            t_start = time.time()
            seconds_passed = 0

            while(not p.poll() and seconds_passed < self.parameters.maxTimeForBenchmark):
                seconds_passed = time.time() - t_start

            retcode = p.poll()

            if (retcode == None):
                print("Time out on " + benchmark)
                ##benchmarkResult            = TERMINATED
                ##self.terminatedBenchmarks  += ' ' + benchmark + ';'
                nTerminatedBenchmarks += 1
                ##self.experimentResult      = TERMINATED
                experimentResults.result = TERMINATED
                experimentResults.AddBencmarkResult(benchmark, TERMINATED)
                p.terminate()

                if (nTerminatedBenchmarks >= 3):
                    ##self.errors += 'Reached maximum number of terminated benchmarks\n'
                    AddErrorToResults('Reached maximum number of terminated benchmarks')
                    return (reportTable)

            else:
                (experimentResults.result, resultValues) = \
                        experiment.ParseLogAndFillTable(logFileName, benchmark, reportTable)

                experimentResults.AddPFSTForBenchmark(resultValues)

            fPlacerOutput.close()
            print(benchmark + ' DONE')

            #if (experiment in self.experimentsToCompare):
            #    self.experimentsToCompare[experiment][benchmark] = resultValues

        return reportTable

if __name__ == '__main__':
    pass #main()