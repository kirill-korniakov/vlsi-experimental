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

class TestRunner:
    parameters = ''

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

        for benchmark in benchmarks:
            logFileName = logFolder + "/" + os.path.basename(benchmark) + ".log"
            fPlacerOutput = open(logFileName, 'w');

            defFile = "--params.def=" + os.path.dirname(os.path.abspath(experiment.benchmarks)) + "/" + benchmark + ".def"
            params = [GeneralParameters.binDir + "itlPlaceRelease.exe", os.path.abspath(experiment.cfg), defFile, experiment.cmdLine]
            #HACK: ugly hack for ISPD04 benchmarks
            if experiment.cfg.find("ispd04") != -1:
                lefFile = "--params.lef=" + os.path.dirname(os.path.abspath(experiment.benchmarks)) + "/" + benchmark + ".lef"
                params.append(lefFile)

            subprocess.call(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)
            fPlacerOutput.close()
            #print(benchmark + ' DONE')
            experiment.ParseLogAndFillTable(logFileName, benchmark, reportTable)

        return reportTable

    def Run(self):
        cp = CoolPrinter()
        svn = SvnWorker()
        emailer = Emailer()

        cp.CoolPrint('Start')

        if self.parameters.doCheckout:
            cp.CoolPrint('Delete sources and Checkout')
            svn.DeleteSources(GeneralParameters.checkoutPath)
            #TODO: implement non HEAD revision
            svn.CheckOut(RepoParameters.srcRepoPath, GeneralParameters.checkoutPath)

        if self.parameters.doBuild:
            cp.CoolPrint('Build')
            self.BuildSln(GeneralParameters.slnPath)

        for experiment in self.parameters.experiments:
            cp.CoolPrint(experiment.name)
            reportTable = self.RunExperiment(experiment)
            cp.CoolPrint("Sending mail with " + reportTable)
            emailer.SendResults(experiment, reportTable)

        cp.CoolPrint('Finish')
