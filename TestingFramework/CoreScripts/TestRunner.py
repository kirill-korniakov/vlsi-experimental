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

import BaseExperiment
from BaseExperiment import *

TERMINATED = 'Terminated'

class TestRunner:
    parameters = ''
    experimentResult = OK
    
    newBenchmarks        = ''
    failedBenchmarks     = ''
    terminatedBenchmarks = ''
    
    nOkBenchmarks         = 0
    nNewBenchmarks        = 0
    nFailedenchmarks      = 0
    nChangedBenchmarks    = 0
    nTerminatedBenchmarks = 0

    def __init__(self, parameters = TestRunnerParameters()):
        self.parameters = parameters
        self.parameters.experiments = []

    def BuildSln(self, slnPath, mode = "Rebuild"):
        print('Building solution...')
        args = [Tools.MSBuild, slnPath, '/t:' + mode, '/p:Configuration=Release']
        subprocess.Popen(subprocess.list2cmdline(args)).communicate()

    def ExtractBenchmarkList(self, benchmarksListPath):
        benchmarks = (open(benchmarksListPath).read()).split('\n')

        # Perform filtering of empty lines and commented by # benchmarks
        benchmarks = [x for x in benchmarks if not x.strip().startswith('#')]
        benchmarks = [x for x in benchmarks if len(x.strip())]

        return benchmarks

    def RunExperiment(self, experiment):
        benchmarks = self.ExtractBenchmarkList(experiment.benchmarks)

        print('Config: %s' % experiment.cfg)
        print('List:   %s' % experiment.benchmarks)
        print("Benchmarks:\n" + ", ".join(benchmarks))
        print("\n")

        reportCreator = ReportCreator(experiment.cfg)
        logFolder = reportCreator.CreateLogFolder()
        reportTable = reportCreator.GetReportTableName()

        experiment.CreateEmptyTable(reportTable)
        self.experimentResult = OK

        for benchmark in benchmarks:
            logFileName = logFolder + "/" + os.path.basename(benchmark) + ".log"
            fPlacerOutput = open(logFileName, 'w');

            defFile = "--params.def=" + os.path.dirname(os.path.abspath(experiment.benchmarks)) + "/" + benchmark + ".def"
            params = [GeneralParameters.binDir + "itlPlaceRelease.exe", os.path.abspath(experiment.cfg), defFile, experiment.cmdLine]
            #HACK: ugly hack for ISPD04 benchmarks
            if experiment.cfg.find("ispd04") != -1:
                lefFile = "--params.lef=" + os.path.dirname(os.path.abspath(experiment.benchmarks)) + "/" + benchmark + ".lef"
                params.append(lefFile)

            benchmarkResult = ''
            #subprocess.call(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)
            p = subprocess.Popen(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)
            t_start = time.time()
            seconds_passed = 0

            while(not p.poll() and seconds_passed < self.parameters.maxTimeForBenchmark):
                seconds_passed = time.time() - t_start

            retcode = p.poll()

            if (retcode == None):
                print("Time out on " + benchmark)
                benchmarkResult = TERMINATED
                self.terminatedBenchmarks  += ' ' + benchmark + ';'
                self.nTerminatedBenchmarks += 1
                self.experimentResult = TERMINATED
                p.terminate()

            else:
                benchmarkResult = experiment.ParseLogAndFillTable(logFileName, benchmark, reportTable)

            fPlacerOutput.close()
            print(benchmark + ' DONE')

            if (benchmarkResult == FAILED):
                self.experimentResult  = FAILED
                self.failedBenchmarks  += ' ' + benchmark + ';'
                self.nFailedBenchmarks += 1

            elif (benchmarkResult == CHANGED):
                self.nChangedBenchmarks += 1

                if ((self.experimentResult != FAILED) and (self.experimentResult != TERMINATED)):
                    self.experimentResult = CHANGED

            elif (benchmarkResult == OK):
                self.nOkBenchmarks += 1

            elif (benchmarkResult == NEW):
                self.newBenchmarks  += ' ' + benchmark + ';'
                self.nNewBenchmarks += 1

            #else TERMINATED - do nothing 

        return reportTable

    def Run(self):
        cp = CoolPrinter()
        svn = SvnWorker()
        emailer = Emailer()

        subject = 'Night experiments'
        text    = ''
        attachmentFiles = []

        cp.CoolPrint('Start')

        if self.parameters.doCheckout:
            cp.CoolPrint('Delete sources and Checkout')
            svn.DeleteSources(GeneralParameters.checkoutPath)
            retcode = 1
            
            for i in range(3):
                #TODO: implement non HEAD revision
                retcode = svn.CheckOut(RepoParameters.srcRepoPath, GeneralParameters.checkoutPath)

                if retcode == 0:
                    break
                
            if retcode != 0:
                text = 'svn error: checkout failed!'

                if (self.parameters.doSendMail == True):
                    emailer.PrepareAndSendMail(subject, text, attachmentFiles)

                return -1

        if self.parameters.doBuild:
            cp.CoolPrint('Build')
            self.BuildSln(GeneralParameters.slnPath)

        for experiment in self.parameters.experiments:
            startTime = GetTimeStamp()
            print('Start time: ' + startTime)
            self.experimentResult = OK
            
            self.newBenchmarks        = ''
            self.failedBenchmarks     = ''
            self.terminatedBenchmarks = ''
            
            self.nOkBenchmarks         = 0
            self.nNewBenchmarks        = 0
            self.nChangedBenchmarks    = 0
            self.nFailedBenchmarks     = 0
            self.nTerminatedBenchmarks = 0
            
            cp.CoolPrint(experiment.name)
            reportTable = self.RunExperiment(experiment)
            #cp.CoolPrint("Sending mail with " + reportTable)

            #subject += ' ' + experiment.name
            #text += experiment.name + ': ' + self.experimentResult
            text += experiment.name + ', ' + os.path.basename(experiment.cfg)
            text += ', ' + os.path.basename(experiment.benchmarks) + ' ('
            nBenchmarks = self.nOkBenchmarks + self.nChangedBenchmarks + self.nFailedBenchmarks + self.nTerminatedBenchmarks + self.nNewBenchmarks
            text += str(nBenchmarks) + ' benchmark(s)):\n'
            text += 'Start time: ' + startTime + '\n'
            text += 'Ok:         '  + str(self.nOkBenchmarks) + '\n'
            text += 'Changed:    '  + str(self.nChangedBenchmarks) + '\n'
            text += 'Failed:     '  + str(self.nFailedBenchmarks)

            if (self.nFailedBenchmarks > 0):
                text += ' (' + self.failedBenchmarks + ')'
               
            text += '\nTerminated: ' + str(self.nTerminatedBenchmarks)

            if (self.nTerminatedBenchmarks > 0):
               text += ' (' + self.terminatedBenchmarks + ')'

            text += '\nNEW:        ' + str(self.nNewBenchmarks)

            if (self.nNewBenchmarks > 0):
                text += '(' + self.newBenchmarks + ')'

            text += '\n\n'

            if (self.experimentResult == CHANGED):
                attachmentFiles.append(reportTable)
            #emailer.SendResults(experiment, reportTable, self.experimentResult)

        text += 'Finished at ' + GetTimeStamp()
        print(text)
        if (self.parameters.doSendMail == True):
            emailer.PrepareAndSendMail(subject, text, attachmentFiles)

        cp.CoolPrint('Finish')
