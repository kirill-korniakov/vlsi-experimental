import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

#Sensitivity Guided Net Weighting experiment
class Experiment_SGNW(BaseExperiment):
    def __init__(self):
        _metrics = ['HPWL', 'TNS', 'WNS']
        _stages  = ['GP1', 'LEG1', 'GP', 'LEG']
        BaseExperiment.__init__(self, 'Weighting (SGNW) experiment',\
            'Sensitivity_guided_weighting.cfg', 'IWLS05.list',\
            _metrics, _stages)

def test():
    testRunner = TestRunner()
    eSGNW      = Experiment_SGNW()
    testRunner.Append(eSGNW)
    testRunner.Run()

#test()
