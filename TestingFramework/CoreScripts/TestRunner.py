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

import SolutionBuilder
from SolutionBuilder import *

class TestRunner:
    emailer     = None
    parameters  = None
    experiments = []
    storage     = ResultsStorage()
    comparator  = ExperimentsComparator(storage)

    def __init__(self, parameters = TestRunnerParameters(), emailer = Emailer()):
        self.emailer     = emailer
        self.parameters  = parameters
        self.experiments = []

    def Append(self, newExperiment):
         self.experiments.append(newExperiment)
         return 0

    def AddExperimentToGroup(self, newExperiment):
        if (self.Append(newExperiment) != 0):
            return

        self.comparator.AddExperimentToGroup(newExperiment)

    def Run(self):
        cp  = CoolPrinter()
        cp.CoolPrint("Start")

        if self.parameters.doCheckout:
            svn = SvnWorker(self.emailer)
            svn.CheckOut()

        if self.parameters.doBuild:
            solutionBuilder = SolutionBuilder(self.emailer)
            solutionBuilder.BuildSln()

        for experiment in self.experiments:
            startTime = GetTimeStamp()
            print("Start time: %s" % (startTime))
            cp.CoolPrint(experiment.name)

            launcher = ExperimentLauncher(experiment, self.storage, self.emailer)
            launcher.RunExperiment()

        self.storage.PrintResults()
        self.storage.SendResults(self.emailer)

        self.comparator.CompareExperiments()
        cp.CoolPrint("Finish")
