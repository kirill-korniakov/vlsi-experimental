import Experiment_LR
from Experiment_LR import *

import Parameters
from Parameters import *

def CompareValues(value1, value2, eps = 0.001):

    if (value1 == value2):
      return 'same'

    value1 = float(value1)
    value2 = float(value2)

    if (abs(value1 - value2) < eps):
      return 'equal'

    return 'notEqual'

class LRPlacementChecker(Experiment_LR):
    masterLogName = '' #TODO: init master log

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #define metrics
        result   = 'OK'
        metrics = ['HPWL', 'TWL', 'TNS', 'WNS']
        stages = ['INIT', 'LEG', 'DP']

        #read numbers
        parser = LogParser(logName)
        table = [[0 for col in range(len(metrics))] for row in range(len(stages))]
        for col in range(len(metrics)):
            for row in range(len(stages)):
                value = str(parser.GetFromPFST(stages[row], metrics[col]))
                table[row][col] = float(value.replace(',', '.'))

        #print absolute values
        masterParser = LogParser(logName) #masterLogName
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
                percent = 100.0 * (table[row][col]/table[0][col] - 1.0)
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
    e = LRPlacementChecker('IWLS05 LR after weighting experiment', 'LR.cfg', 'IWLS05_GP_fast.list', cmdArgs)
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()