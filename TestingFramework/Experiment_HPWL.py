import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_HPWL(BaseExperiment):
    def __init__(self):
        BaseExperiment.__init__(self, 'IWLS05 HPWL experiment',\
            'hpwl_iwls05.cfg', 'IWLS05.list', ['HPWL'], ['DP'])

    def CreateEmptyTable(self, reportTable):
        cols = ['Benchmark']
        cols.append(END_OF_COLUMN)
        cols.append('HPWL')

        WriteStringToFile(cols, reportTable)

def test():
    eIWLS = Experiment_HPWL()
    testRunner = TestRunner()
    testRunner.parameters.experiments.append(eIWLS)
    testRunner.Run()

#test()
