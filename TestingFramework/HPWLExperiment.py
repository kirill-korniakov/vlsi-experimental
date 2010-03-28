import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class HPWLExperiment(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('Benchmark;HPWL;Time')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #define metrics
        HPWL     = 0
        workTime = 0

        #collect metrics
        fh = open(logName, 'r')
        for line in fh.readlines():
            idx = line.find(RepoParameters.repoPath)
            if idx != -1:
                idx = line.find('Revision ') + len('Revision ')
                svnRevision = line[idx:idx + 4]
                print('svnRevision = ' + svnRevision)
                continue

            idx = line.find('HPWL after  detailed placement: ')
            if idx != -1:
                HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
                workTime = line[1:11].replace('.', ',') + ';'
                continue
        fh.close()

        #print metrics
        po = open(reportTable, 'a')
        po.write('\n')
        printStr = benchmark + ';'
        printStr += str(HPWL).replace('.', ',') + ';'
        printStr += str(workTime)
        print(printStr)
        po.write(printStr)
        po.close()

        return svnRevision

def test():
    #e = HPWLExperiment('ISPD04 experiment', 'hpwl_ispd04.cfg', 'ISPD04.list')
    e = HPWLExperiment('IWLS05 HPWL experiment', 'hpwl_iwls05.cfg', 'IWLS05/fast.list')

    testRunner = TestRunner()
    testRunner.parameters.experiments.append(e)
    testRunner.Run()

#test()