import CoreScripts
from CoreScripts import *

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

def NightExperiment():
    exp_HPWL = Experiment_HPWL()
    chk_HPWL_IWLS = Checker(exp_HPWL, "MasterLogs/HPWL/IWLS")

    exp_HPWL.SetConfig('hpwl_ispd04.cfg')
    exp_HPWL.SetBenchmarksList('ISPD04.list')
    chk_HPWL_ISPD = Checker(exp_HPWL, "MasterLogs/HPWL/ISPD")

    chk_LR  = Checker(Experiment_LR(), "MasterLogs/LR")
    chk_BUF = Checker(Experiment_New_Buffering(), "MasterLogs/New_Buffering/IWLS")
    chk_HDP = Checker(Experiment_HippocrateDP(), "MasterLogs/HippocrateDP/Aleksandr")
    chk_SGW = Checker(Experiment_SGNW(), "MasterLogs/Weighting/SensitivityGuided")

    nightTestParams = TestRunnerParameters()
    nightTestParams.doCheckout = True
    nightTestParams.doBuild    = True
    nightTestParams.doSendMail = True
    testRunner = TestRunner(nightTestParams)
    testRunner.parameters.experiments.append(chk_HPWL_IWLS)
    testRunner.parameters.experiments.append(chk_HPWL_ISPD)
    testRunner.parameters.experiments.append(chk_LR)
    testRunner.parameters.experiments.append(chk_BUF)
    testRunner.parameters.experiments.append(chk_HDP)
    testRunner.parameters.experiments.append(chk_SGW)
    testRunner.Run()

NightExperiment()
