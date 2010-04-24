import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_HPWL(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('Benchmark;HPWL;Time')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #define metrics
        HPWL     = 0
        workTime = 0

        #collect metrics
        log = open(logName, 'r')
        for line in log.readlines():
            idx = line.find('HPWL after  detailed placement: ')
            if idx != -1:
                HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
                workTime = line[1:11].replace('.', ',') + ';'
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
        table.close()

def test():
    testRunner = TestRunner()

    #e = Experiment_HPWL('ISPD04 experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    e = Experiment_HPWL('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05_fast.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()