import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_New_Buffering(BaseExperiment):
    def __init__(self):
        _metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
        _stages  = ['INIT', 'NBUF', 'LEGB']
        BaseExperiment.__init__(self, 'IWLS05 new_buffering experiment (NBL)',\
            'New_buffering.cfg', 'IWLS_GP_r1511_New_Buffering.list', _metrics, _stages)

def test():
    testRunner = TestRunner()
    e = Experiment_New_Buffering()
    e.metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
    e.stages  = ['INIT', 'NBUF', 'LEGB']
    testRunner.parameters.experiments.append(e)
    testRunner.Run()

if (__name__ == "__main__"):
    test()
