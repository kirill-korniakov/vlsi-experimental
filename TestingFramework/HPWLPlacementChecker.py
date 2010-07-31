import Experiment_HPWL
from Experiment_HPWL import *

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

class HPWLPlacementChecker(Experiment_HPWL):
    masterLogName = '' #TODO: init master log

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #define metrics
        HPWL     = 0
        workTime = 0
        result   = ''

        #collect metrics
        parser = LogParser(logName)
        HPWL = str(parser.GetFromPFST('DP', 'HPWL'))

        masterLogName = logName #only for debugging
        masterParser = LogParser(masterLogName)
        masterHPWL = str(masterParser.GetFromPFST('DP', 'HPWL'))
        compare_result = CompareValues(HPWL, masterHPWL)

        if (compare_result == 'notEqual'):
                  result = 'Changed'

        #print metrics
        table = open(reportTable, 'a')
        table.write('\n')
        printStr = benchmark + ';'
        printStr += str(HPWL).replace('.', ',')
        print(printStr)
        table.write(printStr)
        table.write(result)

        table.close()

def test():
    testRunner = TestRunner()

    e = HPWLPlacementChecker('ISPD04 experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    #e = HPWLPlacementChecker('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()