import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

#Sensitivity Guided Net Weighting experiment
class Experiment_SGNW(BaseExperiment):
    def __init__(self):
        #_metrics = ['HPWL', 'TNS', 'WNS']
        #_stages  = ['GP1', 'LEG1', 'GP', 'LEG']
        _metrics = ['HPWL', 'TNS']
        _stages  = ['LEG1', 'LEG2', 'LEG3', 'LEG4', 'LEG5', 'LEG6', 'LEG7', 'LEG8', 'LEG9', 'LEG']
        BaseExperiment.__init__(self, 'Weighting (SGNW) experiment',\
            'Sensitivity_guided_weighting.cfg', 'IWLS05.list',\
            _metrics, _stages)

    def PrintBenchmarkTable(self, values, benchmark, reportTable):
        benchmarkFileName = os.path.dirname(reportTable) + '/' + os.path.basename(benchmark) + '.csv'

        for stage in values:
            cols = []
            #cols.append(END_OF_COLUMN)

            for metric in stage:
                cols.append(str(metric))
                cols.append(END_OF_COLUMN)

            WriteStringToFile(cols, benchmarkFileName)

def test():
    testRunner = TestRunner()
    eSGNW      = Experiment_SGNW()
    eSGNW.SetBenchmarksList('IWLS05_fast.list')
    testRunner.Append(eSGNW)
    testRunner.Run()

#test()
