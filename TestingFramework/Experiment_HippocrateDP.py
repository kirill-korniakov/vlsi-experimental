import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_HippocrateDP(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write(';GP + Leg;;;;GP + Leg + HippocrateDP\n')
        po.write('Benchmark;HPWL;TNS;WNS;;HPWL;TNS;WNS\n')
        po.close()

def test():
    hippMetrics = ['HPWL', 'TNS', 'WNS']
    hippStages  = ['LEG', 'DP']
    testRunner = TestRunner()
    #e = Experiment_HippocrateDP('HippocrateDP experiment', 'HippocrateDP.cfg', 'IWLS_GP_r1511/IWLS_GP_Hippocrate.list', hippMetrics, hippStages)
    e = BaseExperiment('HippocrateDP experiment', 'HippocrateDP.cfg', 'IWLS05_fast.list', hippMetrics, hippStages)
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()
