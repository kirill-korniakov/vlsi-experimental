import HPWLPlacementChecker
from HPWLPlacementChecker import *

def NightExperiment():
    exp_HPWL_ISPD04 = HPWLPlacementChecker('ISPD04 HPWL experiment', 'hpwl_ispd04.cfg', 'ISPD04_fast.list', "../master logs/HPWL/ispd")
    exp_HPWL_IWLS05 = HPWLPlacementChecker('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05.list', "../master logs/HPWL/iwls")

    testRunner = TestRunner()
    testRunner.parameters.experiments.append(exp_HPWL_IWLS05)
    testRunner.parameters.experiments.append(exp_HPWL_ISPD04)
    testRunner.Run()

NightExperiment()
