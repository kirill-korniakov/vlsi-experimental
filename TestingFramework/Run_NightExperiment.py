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

import Experiment_Weighting
from Experiment_Weighting import *

import Experiment_New_Buffering
from Experiment_New_Buffering import *

import FastCheckRunner
from FastCheckRunner import *

def NightExperiment(testRunner):
    exp_HPWL = Experiment_HPWL()
    chk_HPWL_IWLS = Checker(exp_HPWL, "MasterLogs/HPWL/IWLS")

    exp_HPWL.name = 'ISPD04 HPWL Experiment'
    exp_HPWL.SetConfig('hpwl_ispd04.cfg')
    exp_HPWL.SetBenchmarksList('ISPD04.list')
    chk_HPWL_ISPD = Checker(exp_HPWL, "MasterLogs/HPWL/ISPD")

    chk_LR  = Checker(Experiment_LR(), "MasterLogs/LR")
    chk_BUF = Checker(Experiment_New_Buffering(), "MasterLogs/New_Buffering/IWLS")
    chk_HDP = Checker(Experiment_HippocrateDP(), "MasterLogs/HippocrateDP/Aleksandr")

    exp_W   = Experiment_Weighting()
    chk_SGW = Checker(exp_W, "MasterLogs/Weighting/SensitivityGuided")

    exp_W.name = 'APlace weighting experiment'
    exp_W.SetConfig('APlace_weighting.cfg')
    chk_APW = Checker(exp_W, "MasterLogs/Weighting/SensitivityGuided")

    testRunner.Append(chk_BUF)
    testRunner.AddExperimentToGroup(chk_SGW)
    testRunner.AddExperimentToGroup(chk_APW)
    testRunner.Append(chk_HPWL_IWLS)
    testRunner.Append(chk_HPWL_ISPD)
    testRunner.Append(chk_LR)
    testRunner.Append(chk_HDP)
    testRunner.Run()

def RunNightExperiments():
    nightTestParams            = TestRunnerParameters()
    nightTestParams.doBuild    = True
    nightTestParams.doCheckout = True
    nightTestParams.doSendMail = True

    testRunner = TestRunner(nightTestParams)
    NightExperiment(testRunner)

if (__name__ == "__main__"):
    RunNightExperiments()
