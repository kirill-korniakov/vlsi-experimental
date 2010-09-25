import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

#Sensitivity Guided Net Weighting experiment
class Experiment_Weighting(BaseExperiment):
    def __init__(self):
        _metrics = ['HPWL', 'TNS', 'WNS']
        _stages  = ['INIT', 'LEG1', 'LEG2', 'LEG3', 'LEG4', 'LEG5', 'LEG6', 'LEG7', 'LEG8', 'LEG9', 'LEG']
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
    eSGNW      = Experiment_Weighting()
    eSGNW.SetBenchmarksList('IWLS05_fast.list')
    #testRunner.Append(eSGNW)

    metrics = ['HPWL', 'TNS', 'WNS']
    #stages  = ['INIT', 'LEG1', 'LEG2', 'LEG3', 'LEG4', 'LEG5', 'LEG6', 'LEG7', 'LEG8', 'LEG9', 'LEG']
    stages  = ['INIT', 'LEG1', 'LEG2', 'LEG']

    cValues = [600, 500, 750]
    expName = 'Weighting (SGNW) experiment_C_'
    cmdArgs = '--NetWeighting.SensitivityGuidedNetWeighting.C='

    for cValue in cValues:
        #cmdArgs = '--NetWeighting.SensitivityGuidedNetWeighting.C=1000'

        e = BaseExperiment(expName + str(cValue), 'Sensitivity_guided_weighting.cfg', 'IWLS05_fast.list', metrics, stages, cmdArgs + str(cValue))
        testRunner.AddExperimentToGroup(e)

    eLeg = BaseExperiment('Leg1', 'Sensitivity_guided_weighting.cfg', 'IWLS05_fast.list', metrics, ['INIT', 'LEG1'], '')#'--DesignFlow.LoopGlobalPlacement=false')
    testRunner.AddExperimentToGroup(eLeg)
    testRunner.Run()

if (__name__ == "__main__"):
    test()
