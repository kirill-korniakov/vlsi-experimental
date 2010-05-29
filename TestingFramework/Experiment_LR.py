import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_LR(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        #NOTE: do not format the string below, because it harms table formatting
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
        #define metrics
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
        printStr = benchmark + ';'
        for col in range(len(metrics)):
            for row in range(len(stages)):
                printStr += str(table[row][col]) + ';'
        printStr = printStr + ';'

        #print percents
        for col in range(len(metrics)):
            for row in range(1, len(stages)):
                percent = 100.0 * (table[row][col]/table[0][col] - 1.0)
                printStr += "%.2f;" % percent
        printStr = printStr + '\n'
        print(printStr)

        #write metrics to the file
        table = open(reportTable, 'a')
        table.write(printStr.replace('.', ','))
        table.close()

def test():
    testRunner = TestRunner()

    cmdArgs = '--LR.GlobalPlacement.LagrangianRelaxation.alphaTWL=1.0e-5'
    e = Experiment_LR('IWLS05 LR after weighting experiment', 'LR.cfg', 'WGHT_large.list', cmdArgs)
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()