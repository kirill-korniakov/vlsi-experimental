import HPWLExperiment
from HPWLExperiment import *

def NightTest():
    testRunner = TestRunner()

    exp_HPWL_ISPD04 = HPWLExperiment('ISPD04 HPWL experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    testRunner.parameters.experiments.append(exp_HPWL_ISPD04)

    exp_HPWL_IWLS05 = HPWLExperiment('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05/fast.list')
    testRunner.parameters.experiments.append(exp_HPWL_IWLS05)

    testRunner.Run()

NightTest()