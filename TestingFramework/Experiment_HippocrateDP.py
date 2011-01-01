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
        cols = [END_OF_COLUMN, 'GP + Leg', END_OF_COLUMN, END_OF_COLUMN, END_OF_COLUMN, END_OF_COLUMN,\
                'GP + Leg + HippocrateDP']
        WriteStringToFile(cols, reportTable)

        cols = ['Benchmark', END_OF_COLUMN, 'HPWL', END_OF_COLUMN, 'TNS', END_OF_COLUMN, 'WNS',\
                END_OF_COLUMN, END_OF_COLUMN, 'HPWL', END_OF_COLUMN, 'TNS', END_OF_COLUMN, 'WNS']
        WriteStringToFile(cols, reportTable)

def test():
    e = Experiment_HippocrateDP()
    #e.stages = ['LEG', 'DP']
    e.stages = []
    #testRunner = TestRunner()
    #testRunner.Append(e)
    #testRunner.Run()

    e.MakeResultTable('.\Reports\HippocrateDP_experiment_HippocrateDP.cfg_2011-01-01_14-12-44', 'HippocrateDPTable.csv')

if (__name__ == "__main__"):
    test()
