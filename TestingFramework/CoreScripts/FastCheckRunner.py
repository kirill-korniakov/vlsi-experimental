import TestRunner
from TestRunner import *

class FastCheckRunner(TestRunner):
    checkersParameters = {}

    def __init__(self, chkParams):
        self.checkersParameters = chkParams
        TestRunner.__init__(self)

    def Append(self, newExperiment):
        expName = newExperiment.name

        if (expName in self.checkersParameters):
            newExperiment.SetBenchmarksList(self.checkersParameters[expName])
            TestRunner.Append(self, newExperiment)