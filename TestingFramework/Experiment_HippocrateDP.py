import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

def findAndDumpHPWL(stringList, pattern, resultTable):
            for line in stringList:
                idx = line.find(pattern)
                if idx != -1:
                    HPWL = line[idx + len(pattern):-1]
                    resultTable.write(HPWL.replace('.', ',') + ';')
                    return
            print("ERROR: pattern was not found!\n")

def findAndDumpTNS(src, resultTable):
        idx = src.find('  TNS: ')
        if idx != -1:
            TNS = src[idx + len('  TNS: '):-1]
            resultTable.write(TNS.replace('.', ',') + ';')
            return True
        return False

def findAndDumpWNS(src, resultTable):
        idx = src.find('  WNS: ')
        if idx != -1:
            WNS = src[idx + len('  WNS: '):-1]
            resultTable.write(WNS.replace('.', ',') + ';;')
            return True
        return False


def findAndDumpSTA(stringList, pattern, resultTable):
        isTiming = False

        for line in stringList:
            idx = line.find(pattern)
            if idx != -1:
                isTiming = True
                continue

            if isTiming == True:
                if findAndDumpTNS(line, resultTable) == True:
                    continue

                if findAndDumpWNS(line, resultTable) == True:
                    return

class Experiment_HippocrateDP(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write(';GP + Leg;;;;GP + Leg + HippocrateDP\n')
        po.write('Benchmark;HPWL;TNS;WNS;;HPWL;TNS;WNS')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        fh = open(logName, 'r')
        stringList = fh.readlines()

        resultTable = open(reportTable, 'a')
        resultTable.write('\n' + benchmark + ';')    # set the benchmark name

        findAndDumpHPWL(stringList, 'HPWL after legalization: ', resultTable)
        findAndDumpSTA(stringList, 'STA after legalization:\n', resultTable)

        findAndDumpHPWL(stringList, 'HPWL after  detailed placement: ', resultTable)
        findAndDumpSTA(stringList, 'STA after detailed placement:\n', resultTable)
        resultTable.write(';')

        fh.close()
        resultTable.close()

def test():
    testRunner = TestRunner()
    e = Experiment_HippocrateDP('HippocrateDP experiment', 'HippocrateDP.cfg', 'ISPD04_fast.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()
