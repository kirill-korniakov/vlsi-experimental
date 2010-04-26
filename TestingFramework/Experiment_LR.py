import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_LR(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('Benchmark;bbHPWL;abHPWL;bbTWL;abTWL;bbTNS;abTNS;bbWNS;abWNS\n')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #get metrics
        parser = LogParser(logName)
        bbHPWL = parser.GetFromPFST('INIT', 4)
        abHPWL = parser.GetFromPFST('LEG', 4)
        bbTWL = parser.GetFromPFST('INIT', 5)
        abTWL = parser.GetFromPFST('LEG', 5)
        bbTNS = parser.GetFromPFST('INIT', 6)
        abTNS = parser.GetFromPFST('LEG', 6)
        bbWNS = parser.GetFromPFST('INIT', 7)
        abWNS = parser.GetFromPFST('LEG', 7)

        #print metrics
        printStr = benchmark + ';'
        printStr += str(bbHPWL) + ';'
        printStr += str(abHPWL) + ';'
        printStr += str(bbTWL) + ';'
        printStr += str(abTWL) + ';'
        printStr += str(bbTNS) + ';'
        printStr += str(abTNS) + ';'
        printStr += str(bbWNS) + ';'
        printStr += str(abWNS) + ';'
        printStr += '\n'
        print(printStr)

        #write metrics
        table = open(reportTable, 'a')
        table.write(printStr)
        table.close()

def test():
    testRunner = TestRunner()

    e = Experiment_LR('IWLS05 LR experiment', 'LR.cfg', 'IWLS05_GP_fast.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()