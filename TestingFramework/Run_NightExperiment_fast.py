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

fast_lists = {"IWLS05 HPWL experiment":"IWLS05_fast.list", "HPWL_ISPD":"ISPD04_fast.list", "LR":"IWLS_GP_r1511/IWLS_GP_fast.list",\
    "BUF":"IWLS05_fast.list", "HippocrateDP experiment":"IWLS_GP_Hippocrate.list", "Weighting (SGNW) experiment":"IWLS05_fast.list",\
    "APlace weighting experiment":"IWLS05_fast.list"}

def NightExperiment(lists):
    exp_HPWL = Experiment_HPWL()
    exp_HPWL.SetBenchmarksList(lists["IWLS05 HPWL experiment"])
    chk_HPWL_IWLS = Checker(exp_HPWL, "MasterLogs/HPWL/IWLS")

    exp_HPWL.name = 'ISPD04 HPWL Experiment'
    exp_HPWL.SetConfig('hpwl_ispd04.cfg')
    exp_HPWL.SetBenchmarksList(lists["HPWL_ISPD"])
    chk_HPWL_ISPD = Checker(exp_HPWL, "MasterLogs/HPWL/ISPD")

    chk_LR  = Checker(Experiment_LR(), "MasterLogs/LR")
    chk_BUF = Checker(Experiment_New_Buffering(), "MasterLogs/New_Buffering/IWLS")
    chk_HDP = Checker(Experiment_HippocrateDP(), "MasterLogs/HippocrateDP/Aleksandr")

    exp_W   = Experiment_Weighting()
    chk_SGW = Checker(exp_W, "MasterLogs/Weighting/SensitivityGuided")

    exp_W.name = 'APlace weighting experiment'
    exp_W.SetConfig('APlace_weighting.cfg')
    chk_APW = Checker(exp_W, "MasterLogs/Weighting/SensitivityGuided")

    testRunner = FastCheckRunner(fast_lists)
    #testRunner.Append(chk_HPWL_IWLS)
    #testRunner.Append(chk_HPWL_ISPD)
    #testRunner.Append(chk_LR)
    #testRunner.Append(chk_BUF)
    testRunner.Append(chk_HDP)
    testRunner.AddExperimentToGroup(chk_SGW)
    testRunner.AddExperimentToGroup(chk_APW)
    testRunner.Run()

def run():
    NightExperiment(lists) #<-lists


def fast_run():
    NightExperiment(fast_lists) #<-fast_lists

#run()
fast_run()
