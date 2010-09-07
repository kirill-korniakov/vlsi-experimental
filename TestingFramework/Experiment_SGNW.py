import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

#Sensitivity Guided Net Weighting experiment
class Experiment_SGNW(BaseExperiment):
    def __init__(self):
        _metrics = ['HPWL', 'TNS', 'WNS']
        _stages  = ['GP1', 'LEG1', 'GP9', 'LEG9']
        BaseExperiment.__init__(self, 'Weighting (SGNW) experiment',\
            'Sensitivity_guided_weighting.cfg', 'IWLS05.list',\
            _metrics, _stages)

def test():
    testRunner = TestRunner()

    #eISPD = Experiment_HPWL('ISPD04 experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    #eIWLS = Experiment_HPWL('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list', ['HPWL'], ['DP'])
    eIWLS = BaseExperiment('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg',\
        'IWLS05_fast.list', ['HPWL'], ['DP'])

    testRunner.parameters.experiments.append(eIWLS)
    #testRunner.parameters.experiments.append(eISPD)

    testRunner.Run()

#test()
