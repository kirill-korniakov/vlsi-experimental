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
    testRunner = TestRunner()

    #eISPD = Experiment_HPWL('ISPD04 experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    #eIWLS = Experiment_HPWL('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list', ['HPWL'], ['DP'])
    eIWLS = BaseExperiment('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg',\
        'IWLS05_fast.list', ['HPWL'], ['DP'])

    testRunner.parameters.experiments.append(eIWLS)
    #testRunner.parameters.experiments.append(eISPD)

    testRunner.Run()

#test()
