import Experiment_LR
from Experiment_LR import *

import Parameters
from Parameters import *

class LRPlacementChecker(Experiment_LR):
    masterLogFolder = ''

    def __init__(self, name, cfg, benchmarks, masterLogFolder = '', cmdLine = ''):
        Experiment_LR.__init__(self, name, cfg, benchmarks, cmdLine)
        self.masterLogFolder = masterLogFolder

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #define metrics
        result   = 'OK'
        metrics = ['HPWL', 'TWL', 'TNS', 'WNS']
        stages = ['INIT', 'LEG']#, 'DP']

        #read numbers
        parser = LogParser(logName)
        table = [[0 for col in range(len(metrics))] for row in range(len(stages))]
        for col in range(len(metrics)):
            for row in range(len(stages)):
                value = str(parser.GetFromPFST(stages[row], metrics[col]))

                if (value == str(NOT_FOUND)):
                   return ('Failed')
                
                table[row][col] = float(value.replace(',', '.'))

        #print absolute values
        masterLogName = self.masterLogFolder + "/" + os.path.basename(logName)
        masterParser = LogParser(masterLogName)
        printStr = benchmark + ';'
        for col in range(len(metrics)):
            for row in range(len(stages)):
                value = str(masterParser.GetFromPFST(stages[row], metrics[col]))
                compare_result = CompareValues(value, table[row][col])

                if (compare_result == 'notEqual'):
                  result = 'Changed'
                printStr += str(table[row][col]) + ';'
        printStr = printStr + ';'

        #print percents
        for col in range(len(metrics)):
            for row in range(1, len(stages)):
                if table[0][col] != 0.0:
                    percent = 100.0 * (table[row][col]/table[0][col] - 1.0)
                else:
                    percent = -1.0
                printStr += "%.2f;" % percent
        printStr = printStr + result + ';'
        printStr = printStr + '\n'
        print(printStr)

        #write metrics to the file
        table = open(reportTable, 'a')
        table.write(printStr.replace('.', ','))
        table.close()

def test():
    testRunner = TestRunner()

    cmdArgs = '--LR.GlobalPlacement.LagrangianRelaxation.alphaTWL=1.0e-5'
    e = LRPlacementChecker('IWLS05 LR after weighting experiment', 'LR.cfg', 'IWLS_GP_r1511/IWLS_GP_fast.list', "../master logs/LR", cmdArgs)
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()
