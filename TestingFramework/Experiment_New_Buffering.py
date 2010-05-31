import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_New_Buffering(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('Benchmark;bbHPWL;abHPWL;bbTWL;abTWL;bbTNS;abTNS;bbWNS;abWNS;TIME;Cells;%HPWL;%TWL;%TNS;%WNS;%AddBuf\n')
        po.close()

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #get metrics
        metricTagList = ['StageName', 'Tag', 'Time(sec)', 'Cells', 'HPWL(nm)', 'TWL(nm)', 'TNS(ns)', 'WNS(ns)']
        parser = LogParser(logName)
        tagCount = parser.GetStageTag()[0]
        print('last tag = ' + str(tagCount))
        tagList = parser.GetStageTag()[1]        
        for i in tagList:
            print('tag = ' + i)
        bbCells = parser.GetFromPFST(tagList[0], metricTagList[3])
        abCells = parser.GetFromPFST(tagList[-1], metricTagList[3])
        abTime = parser.GetFromPFST(tagList[-1], metricTagList[2])
        bbHPWL = parser.GetFromPFST(tagList[0], metricTagList[4])
        abHPWL = parser.GetFromPFST(tagList[-1], metricTagList[4])
        bbTWL = parser.GetFromPFST(tagList[0], metricTagList[5])
        abTWL = parser.GetFromPFST(tagList[-1], metricTagList[5])
        bbTNS = parser.GetFromPFST(tagList[0], metricTagList[6])
        abTNS = parser.GetFromPFST(tagList[-1], metricTagList[6])
        bbWNS = parser.GetFromPFST(tagList[0], metricTagList[7])
        abWNS = parser.GetFromPFST(tagList[-1], metricTagList[7])        
        pHPWL = (1 - float(abHPWL.replace(',', '.')) / float(bbHPWL.replace(',', '.'))) * 100
        pTWL = (1 - float(abTWL.replace(',', '.')) / float(bbTWL.replace(',', '.'))) * 100 
        pTNS = (1 - float(abTNS.replace(',', '.')) / float(bbTNS.replace(',', '.'))) * 100
        pWNS = (1 - float(abWNS.replace(',', '.')) / float(bbWNS.replace(',', '.'))) * 100
        AddBuf = float(abCells) - float(bbCells)

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
        printStr += str(abTime) + ';'
        printStr += str(bbCells) + ';'
        printStr += str(pHPWL) + ';'
        printStr += str(pTWL) + ';'
        printStr += str(pTNS) + ';'
        printStr += str(pWNS) + ';'
        printStr += str(AddBuf) + ';'
        
        printStr += '\n'
        print(printStr)

        #write metrics
        table = open(reportTable, 'a')
        table.write(printStr)
        table.close()

def test():
    testRunner = TestRunner()
    e = Experiment_New_Buffering('IWLS05 new_buffering experiment (GBL)', 'New_buffering.cfg', 'IWLS_GP_r1511.list')
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

test()
