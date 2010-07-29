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
        result   = 'OK'

        #collect metrics
        log = open(logName, 'r')
        for line in log.readlines():
            idx = line.find('HPWL after  detailed placement: ')
            if idx != -1:
                HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
                workTime = line[1:11].replace('.', ',') + ';'
                continue
        log.close()
        
        log = open(masterLogName, 'r')
        for line in log.readlines():
            idx = line.find('HPWL after  detailed placement: ')
            if idx != -1:
                masterHPWL = line[idx + len('HPWL after  detailed placement: '):-1]
                compare_result = CompareValues(HPWL, masterHPWL)
                
                if (compare_result == 'notEqual'):
                  result = 'Changed'
                  
                continue
        log.close()

        #print metrics
        table = open(reportTable, 'a')
        table.write('\n')
        printStr = benchmark + ';'
        printStr += str(HPWL).replace('.', ',') + ';'
        printStr += str(workTime)
        print(printStr)
        table.write(printStr)
        
        if (result == 'Changed'):
          table.write('Changed;')
        
        table.close()

def test():
    testRunner = TestRunner()

    #e = Experiment_HPWL('ISPD04 experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    e = HPWLPlacementChecker('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()