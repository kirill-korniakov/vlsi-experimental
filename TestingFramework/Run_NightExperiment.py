import HPWLPlacementChecker
from HPWLPlacementChecker import *

import LRPlacementChecker
from LRPlacementChecker import *

def NightExperiment():
    exp_HPWL_ISPD04 = HPWLPlacementChecker('ISPD04 HPWL experiment', 'hpwl_ispd04.cfg', 'ISPD04.list', "MasterLogs/HPWL/ISPD")
    exp_HPWL_IWLS05 = HPWLPlacementChecker('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05.list', "MasterLogs/HPWL/IWLS")
    exp_LR_IWLS05GP = LRPlacementChecker('IWLS05GP LR experiment', 'LR.cfg', 'IWLS_GP_r1511/IWLS_GP.list', "MasterLogs/LR")

    testRunner = TestRunner()
    testRunner.parameters.experiments.append(exp_HPWL_IWLS05)
    testRunner.parameters.experiments.append(exp_HPWL_ISPD04)
    #testRunner.parameters.experiments.append(exp_LR_IWLS05GP)
    testRunner.Run()

NightExperiment()
