import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class BaseChecker(BaseExperiment):
    masterLogFolder = ''

    def __init__(self, name, cfg, benchmarks, masterLogFolder = '', metrics = [], stages = [], cmdLine = ''):
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
        printStr = 'Benchmark;'
        
        #write header of a table.
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                printStr += 'Current ' + self.metrics[col] + '_' + self.stages[row] + ';'
                printStr += 'Master '  + self.metrics[col] + '_' + self.stages[row] + ';'
                
            printStr += ';' #an empty column between metrics on different stages
                
        printStr += '\n'
        resultFile = open(reportTable, 'a')
        resultFile.write(printStr.replace('.', ','))
        resultFile.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        currentTable = self.ParseLog(logName, benchmark)
        
        if (currentTable == []):
          return FAILED
        
        masterLogName = self.masterLogFolder + "/" + os.path.basename(logName)
        masterTable   = self.ParseLog(masterLogName, benchmark)
        
        printStr = benchmark + ';'

        if (masterTable == []):
            printStr += 'experiment has not failed but master log is empty\n'
            return CHANGED
        
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                printStr += str(currentTable[row][col]) + ';' + str(masterTable[row][col]) + ';'

            printStr = printStr + ';' #an empty column between metrics on different stages

        printStr += '\n'
        print(printStr)

        #write metrics to the file
        table = open(reportTable, 'a')
        table.write(printStr.replace('.', ','))
        table.close()
        return self.CompareTables(currentTable, masterTable)

def test():
    metrics = ['HPWL', 'TWL', 'TNS', 'WNS']
    stages  = ['INIT', 'LEG']
    testRunner = TestRunner()
    cmdArgs = '--LR.GlobalPlacement.LagrangianRelaxation.alphaTWL=1.0e-5'
    #e = BaseChecker('IWLS05 LR after weighting experiment', 'LR.cfg', 'IWLS05_fast.list', "MasterLogs/LR", metrics, stages, cmdArgs)
    e = BaseChecker('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list', "MasterLogs/HPWL/IWLS", ['HPWL'], ['DP'])
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()
