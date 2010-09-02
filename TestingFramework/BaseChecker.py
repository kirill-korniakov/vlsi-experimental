import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class BaseChecker(BaseExperiment):
    masterLogFolder = ''

    def __init__(self, name, cfg, benchmarks, masterLogFolder, metrics = [], stages = [], cmdLine = ''):
        BaseExperiment.__init__(self, name, cfg, benchmarks, metrics, stages, cmdLine)
        self.masterLogFolder = masterLogFolder

    def CompareTables(self, table1, table2, eps = 0.001):
        for col in range(len(self.metrics)):
            for row in range(len(self.stages)):
                compare_result = CompareValues(table1[row][col], table2[row][col])

                if (compare_result == 'notEqual'):
                  return(CHANGED)

        return(OK)

    def CreateEmptyTable(self, reportTable):
        cols = ['Benchmark']
        cols.append(EMPTYSITE)
        
        #write header of a table.
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append('Current ' + self.metrics[col] + '_' + self.stages[row])
                cols.append(EMPTYSITE)
                cols.append('Master '  + self.metrics[col] + '_' + self.stages[row])
                cols.append(EMPTYSITE)
                
            cols.append(EMPTYSITE) #an empty column between metrics on different stages

        WriteStringToFile(cols, reportTable)

    def FillTable(self, currentValues, masterValues, benchmark, reportTable):
        cols = [benchmark]
        cols.append(EMPTYSITE)

        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                cols.append(str(currentValues[row][col]))
                cols.append(EMPTYSITE)
                cols.append(str(masterValues[row][col]))
                cols.append(EMPTYSITE)

            cols.append(EMPTYSITE) #an empty column between metrics on different stages

        #write metrics to the file
        WriteStringToFile(cols, reportTable)

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        currentValues = self.ParseLog(logName, benchmark)
        
        if (currentValues == []):
          return FAILED
        
        masterLogName = self.masterLogFolder + "/" + os.path.basename(logName)
        masterValues  = self.ParseLog(masterLogName, benchmark)
        
        if (masterValues == []):
            print('experiment has not failed but master log is empty\n')
            return NEW
        
        self.FillTable(currentValues, masterValues, benchmark, reportTable)
        return self.CompareTables(currentValues, masterValues)

def test():
    hippMetrics = ['HPWL', 'TNS', 'WNS']
    hippStages  = ['LEG', 'DP']

    weightingMetrics = ['HPWL', 'TNS', 'WNS']
    weightingStages  = ['GP1', 'LEG1', 'GP9', 'LEG9']
    
    testRunner = TestRunner()
    e = BaseChecker('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list', "MasterLogs/HPWL/IWLS", ['HPWL'], ['DP'])
    #e = BaseChecker('HippocrateDP experiment', 'HippocrateDP.cfg', 'IWLS_GP_r1511/IWLS_GP_Hippocrate.list', "MasterLogs/HippocrateDP/Aleksandr", hippMetrics, hippStages)
    #e = BaseChecker('Weighting (SGNW) experiment', 'Sensitivity_guided_weighting.cfg', 'IWLS05_fast.list', "MasterLogs/Weighting/SensitivityGuided", weightingMetrics, weightingStages)
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()
