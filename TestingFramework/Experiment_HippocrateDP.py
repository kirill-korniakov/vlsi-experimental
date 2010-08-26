import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_HippocrateDP(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write(';GP + Leg;;;;GP + Leg + HippocrateDP\n')
        po.write('Benchmark;HPWL;TNS;WNS;;HPWL;TNS;WNS\n')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        table = self.ParseLog(logName, benchmark)
        
        if (table == []):
          return FAILED
                
        printStr = benchmark + ';'
        
        for row in range(len(self.stages)):
            for col in range(len(self.metrics)):
                printStr += str(table[row][col]) + ';'

            printStr += ';'

        printStr = printStr + '\n'
        print(printStr)

        #write metrics to the file
        table = open(reportTable, 'a')
        table.write(printStr.replace('.', ','))
        table.close()
        return OK

def test():
    metrics = ['HPWL', 'TNS', 'WNS']
    stages  = ['LEG', 'DP']
    testRunner = TestRunner()
    #e = Experiment_HippocrateDP('HippocrateDP experiment', 'HippocrateDP.cfg', 'IWLS_GP_r1511/IWLS_GP_Hippocrate.list', metrics, stages)
    e = BaseExperiment('HippocrateDP experiment', 'HippocrateDP.cfg', 'IWLS05_fast.list', metrics, stages)
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()
