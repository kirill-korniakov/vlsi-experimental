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

class CheckRunner(TestRunner):

    def __init__(self):
        nightTestParams = TestRunnerParameters()
        nightTestParams.doCheckout = True
        nightTestParams.doBuild    = True
        nightTestParams.doSendMail = True
        TestRunner.__init__(self, nightTestParams)
