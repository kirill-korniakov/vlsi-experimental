import HPWLExperiment
from HPWLExperiment import *

def NightExperiment():
    exp_HPWL_ISPD04 = Experiment_HPWL('ISPD04 HPWL experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    exp_HPWL_IWLS05 = Experiment_HPWL('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05/full.list')

    testRunner = TestRunner()
    testRunner.parameters.experiments.append(exp_HPWL_ISPD04)
    testRunner.parameters.experiments.append(exp_HPWL_IWLS05)
    testRunner.Run()

NightExperiment()