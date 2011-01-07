import subprocess
import sys
import os

import datetime
from datetime import date
import time

from Emailer import *
from SvnWorker import *

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

import BaseExperiment
from BaseExperiment import *

import ExperimentLauncher
from ExperimentLauncher import *

import ExperimentsComparator
from ExperimentsComparator import *

class TestRunner:
    emailer    = None
    parameters = None
    storage    = ResultsStorage()
    comparator = ExperimentsComparator(storage)

    def __init__(self, parameters = TestRunnerParameters(), emailer = Emailer()):
        self.emailer                = emailer
        self.parameters             = parameters
        self.parameters.experiments = []

    def BuildSln(self, slnPath, mode = "Rebuild"):
        print("Building solution...")
        res  = 0
        args = [Tools.MSBuild, slnPath, "/t:" + mode, "/p:Configuration=Release"]

        try:
            res = subprocess.call(args)

        except WindowsError:
            error = ("Error: can not call %s" % (Tools.MSBuild))
            ReportErrorAndExit(error, self.emailer)

        if (res != 0):
            error = "Build failed!"
            print(error)
            buildLog = GeneralParameters.buildLog
            self.emailer.SendMessageAndExit(error, [buildLog])

    def PrintXXXBenchmarks(self, status, nXXXBenchmarks, benchmarks = ''):
        if (nXXXBenchmarks == 0):
            return ('')

        printStr = status + str(nXXXBenchmarks)

        if (benchmarks != ''):
            printStr += '(' + benchmarks + ')'

        return (printStr + '\n')

    def Append(self, newExperiment):
         self.parameters.experiments.append(newExperiment)
         return 0

    def AddExperimentToGroup(self, newExperiment):
        if (self.Append(newExperiment) != 0):
            return

        self.comparator.AddExperimentToGroup(newExperiment)

    def Run(self):
        cp      = CoolPrinter()
        svn     = SvnWorker()
        cp.CoolPrint('Start')

        if self.parameters.doCheckout:
            cp.CoolPrint("Delete sources and Checkout")
            svn.DeleteSources(GeneralParameters.checkoutPath)
            retcode = 1

            for i in range(10):
                #TODO: implement non HEAD revision
                try:
                    retcode = svn.CheckOut(RepoParameters.srcRepoPath, GeneralParameters.checkoutPath)

                except WindowsError:
                    error = "Error: can not call svn.exe"
                    ReportErrorAndExit(error, self.emailer)

                if retcode == 0:
                    break

            if retcode != 0:
                error = "svn error: checkout failed!"
                print(error)
                self.emailer.SendMessageAndExit(error, [])

        if self.parameters.doBuild:
            cp.CoolPrint("Build")
            self.BuildSln(GeneralParameters.slnPath)

        for experiment in self.parameters.experiments:
            startTime = GetTimeStamp()
            print("Start time: %s" % (startTime))
            cp.CoolPrint(experiment.name)

            launcher = ExperimentLauncher(experiment, self.storage, self.emailer)
            launcher.RunExperiment()

        self.storage.PrintResults()
        self.storage.SendResults(self.emailer)

        self.comparator.CompareExperiments()

        cp.CoolPrint("Finish")
