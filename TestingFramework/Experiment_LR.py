import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_LR(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('Benchmark;initHPWL;lrHPWL;dpHPWL;\
initTWL;lrTWL;dpTWL;\
initTNS;lrTNS;dpTNS;\
initWNS;lrWNS;dpWNS;;\
lrHPWL%;dpHPWL%;\
lrTWL%;dpTWL%;\
lrTNS%;dpTNS%;\
lrWNS%;dpWNS%\n')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        parser = LogParser(logName)

        #define metrics
        metrics = ['HPWL', 'TWL', 'TNS', 'WNS']
        stages = ['INIT', 'LEG', 'DP']

        table = [[0 for col in range(len(metrics))] for row in range(len(stages))]

        for col in range(len(metrics)):
            for row in range(len(stages)):
                table[row][col] = float((parser.GetFromPFST(stages[row], metrics[col])).replace(',', '.'))

        printStr = benchmark + ';'
        for col in range(len(metrics)):
            for row in range(len(stages)):
                printStr += str(table[row][col]) + ';'
        printStr = printStr + ';'
        for col in range(len(metrics)):
            for row in range(1, len(stages)):
                percent = 100.0 * (table[row][col]/table[0][col] - 1.0)
                printStr += "%.2f;" % percent
        printStr = printStr + '\n'
        print(printStr)

        #write metrics
        table = open(reportTable, 'a')
        table.write(printStr.replace('.', ','))
        table.close()

def test():
    testRunner = TestRunner()

    e = Experiment_LR('IWLS05 LR experiment', 'LR.cfg', 'IWLS05_GP.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()