import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class ExperimentBuffering(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('Benchmark;bbHPWL;bbTWL;bbTNS;bbWNS;abHPWL;abTWL;abTNS;abWNS;Time')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #define metrics
        bbHPWL   = 0
        abHPWL   = 0
        bbTWL    = 0
        abTWL    = 0
        bbTNS    = 0
        abTNS    = 0
        bbWNS    = 0
        abWNS    = 0
        workTime = 0

        #collect metrics
        log = open(logName, 'r')
        for line in log.readlines():
            idx = line.find('INIT')
            if idx != -1:
                #TODO: create correct implementation
                #HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
                continue

            idx = line.find('LEGB')
            if idx != -1:
                #TODO: create correct implementation
                #HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
                #workTime = line[1:11].replace('.', ',') + ';'
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

    e = ExperimentBuffering('IWLS05 buffering experiment (GBL)', 'kirill_buffering.cfg', 'IWLS05_fast.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()