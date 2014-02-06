import os
import sys
import time
import datetime
from datetime import date
import subprocess
from ConfigParser import ConfigParser

from Emailer import Emailer
from SvnWorker import SvnWorker
from SolutionBuilder import SolutionBuilder
from ResultsStorage import ResultsStorage
from ExperimentLauncher import ExperimentLauncher
from ExperimentsComparator import ExperimentsComparator
from CoreFunctions import GetTimeStamp, CreateConfigParser
from Logger import Logger
from ParametersParsing import TestRunnerParameters, EmailerParameters, RepoParameters, \
    ReportParameters, GeneralParameters, LogParserParameters, Tools


class TestRunner:
    emailer = None
    cfgParser = CreateConfigParser()
    parameters = None
    experiments = []
    storage = ResultsStorage()
    comparator = ExperimentsComparator(storage)

    def __init__(self, parameters=None, emailer=None):
        if (parameters == None):
            parameters = TestRunnerParameters(self.cfgParser)

        if (emailer == None):
            emailerParameters = EmailerParameters(self.cfgParser)
            emailer = Emailer(emailerParameters)

        self.emailer = emailer
        self.parameters = parameters
        self.experiments = []

    def Append(self, newExperiment):
        self.experiments.append(newExperiment)
        return 0

    def AddExperimentToGroup(self, newExperiment):
        if (self.Append(newExperiment) != 0):
            return

        self.comparator.AddExperimentToGroup(newExperiment)

    def Run(self):
        logger = Logger()
        logger.LogStartMessage()
        generalParameters = GeneralParameters(self.cfgParser)
        reportParameters = ReportParameters(self.cfgParser)

        if self.parameters.doCheckout:
            repoParameters = RepoParameters(self.cfgParser)
            svn = SvnWorker(self.emailer, generalParameters, repoParameters)
            svn.CheckOut()

        if self.parameters.doBuild:
            tools = Tools(self.cfgParser)
            solutionBuilder = SolutionBuilder(self.emailer)
            solutionBuilder.BuildSln(generalParameters, tools)

        for experiment in self.experiments:
            logger.CoolLog("%s: starting %s" % (GetTimeStamp(), experiment.name))
            launcher = ExperimentLauncher(experiment, self.storage, self.emailer)
            launcher.RunExperiment(generalParameters, reportParameters)

        self.storage.LogResults()
        self.storage.SendResults(self.emailer)

        self.comparator.CompareExperiments(reportParameters)
        logger.CoolLog("Finish")


def test():
    testRunner = TestRunner()
    testRunner.Run()


if (__name__ == "__main__"):
    test()
