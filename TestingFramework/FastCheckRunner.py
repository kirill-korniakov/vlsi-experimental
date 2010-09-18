import Checker
from Checker import *

import Experiment_HippocrateDP
from Experiment_HippocrateDP import *

import Experiment_HPWL
from Experiment_HPWL import *

import Experiment_LR
from Experiment_LR import *

import Experiment_SGNW
from Experiment_SGNW import *

import Experiment_New_Buffering
from Experiment_New_Buffering import *

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