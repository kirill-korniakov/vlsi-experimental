import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_HPWL(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('Benchmark;HPWL\n')
        po.close()

def test():
    testRunner = TestRunner()

    #eISPD = Experiment_HPWL('ISPD04 experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    #eIWLS = Experiment_HPWL('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list', ['HPWL'], ['DP'])
    eIWLS = BaseExperiment('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list', ['HPWL'], ['DP'])

    testRunner.parameters.experiments.append(eIWLS)
    #testRunner.parameters.experiments.append(eISPD)

    testRunner.Run()

#test()
