import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_HippocrateDP(BaseExperiment):
    def __init__(self):
        _metrics = ['HPWL', 'TNS', 'WNS']
        _stages  = ['LEG', 'DP']
        BaseExperiment.__init__(self, 'HippocrateDP experiment',\
            'HippocrateDP.cfg', 'IWLS_GP_Hippocrate.list', _metrics, _stages)

    def CreateEmptyTable(self, reportTable):
        #po.write(';GP + Leg;;;;GP + Leg + HippocrateDP\n')
        cols = [END_OF_COLUMN]
        cols.append('GP + Leg')
        cols.append(END_OF_COLUMN)
        cols.append(END_OF_COLUMN)
        cols.append(END_OF_COLUMN)
        cols.append(END_OF_COLUMN)
        cols.append('GP + Leg + HippocrateDP')
        WriteStringToFile(cols, reportTable)

        #po.write('Benchmark;HPWL;TNS;WNS;;HPWL;TNS;WNS\n')
        cols = ['Benchmark']
        cols.append(END_OF_COLUMN)
        cols.append('HPWL')
        cols.append(END_OF_COLUMN)
        cols.append('TNS')
        cols.append(END_OF_COLUMN)
        cols.append('WNS')
        cols.append(END_OF_COLUMN)
        cols.append(END_OF_COLUMN)
        cols.append('HPWL')
        cols.append(END_OF_COLUMN)
        cols.append('TNS')
        cols.append(END_OF_COLUMN)
        cols.append('WNS')

        WriteStringToFile(cols, reportTable)

def test():
    e = Experiment_HippocrateDP()
    e.stages = ['LEG', 'DP']
    #e.stages = []
    #testRunner = TestRunner()
    #testRunner.Append(e)
    #testRunner.Run()

    e.MakeResultTable('.\Reports\HippocrateDP.cfg_2010-12-04_12-59-24', 'HippocrateDPTable.csv')

if (__name__ == "__main__"):
    test()
