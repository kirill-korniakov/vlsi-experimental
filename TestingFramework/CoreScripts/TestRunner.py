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
    svnRevision = ''
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

        print('Config name = %s' % experiment.cfg)
        print("Performing tests on the following set of benchmarks:\n" + ", ".join(benchmarks))

        reportCreator = ReportCreator()
        logFolder = reportCreator.CreateLogFolder(experiment.cfg)
        reportTable = reportCreator.GetReportTableName(experiment.cfg)

        experiment.CreateEmptyTable(reportTable)

        for benchmark in benchmarks:
            logFileName = logFolder + "/" + os.path.basename(benchmark) + ".log"
            fPlacerOutput = open(logFileName, 'w');

            defFile = "--params.def=" + os.path.dirname(os.path.abspath(experiment.benchmarks)) + "/" + benchmark + ".def"
            lefFile = "--params.lef=" + os.path.dirname(os.path.abspath(experiment.benchmarks)) + "/" + benchmark + ".lef"
            params = [GeneralParameters.binDir + "itlPlaceRelease.exe", os.path.abspath(experiment.cfg), defFile, lefFile]
            #params.append()

            #FIXME: uncomment
            subprocess.call(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)
            fPlacerOutput.close()
            print(benchmark + ' is done...')
            self.svnRevision = experiment.ParseLogAndFillTable(logFileName, benchmark, reportTable)

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