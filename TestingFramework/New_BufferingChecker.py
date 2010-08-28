import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_New_Buffering(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
        po.write('name;целов;время;буф;нач.HPWL;фин.HPWL;нач.TWL;фин.TWL;нач.TNS;фин.TNS;нач.WNS;фин.WNS;HPWL%;TWL%;TNS%;WNS%;\n')
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
        #abTime1 = parser.GetFromPFST(tagList[3], metricTagList[2])
        #abTime0 = parser.GetFromPFST(tagList[2], metricTagList[2])
        bbHPWL = parser.GetFromPFST(tagList[0], metricTagList[4])
        #bbHPWL1 = parser.GetFromPFST(tagList[3], metricTagList[4])
        abHPWL = parser.GetFromPFST(tagList[-1], metricTagList[4])
        bbTWL = parser.GetFromPFST(tagList[0], metricTagList[5])
        #bbTWL1 = parser.GetFromPFST(tagList[3], metricTagList[5])
        abTWL = parser.GetFromPFST(tagList[-1], metricTagList[5])
        bbTNS = parser.GetFromPFST(tagList[0], metricTagList[6])
        #bbTNS1 = parser.GetFromPFST(tagList[3], metricTagList[6])
        abTNS = parser.GetFromPFST(tagList[-1], metricTagList[6])
        bbWNS = parser.GetFromPFST(tagList[0], metricTagList[7])
        #bbWNS1 = parser.GetFromPFST(tagList[3], metricTagList[7])
        abWNS = parser.GetFromPFST(tagList[-1], metricTagList[7])        
        pHPWL = (1 - float(abHPWL.replace(',', '.')) / float(bbHPWL.replace(',', '.'))) * 100
        pTWL = (1 - float(abTWL.replace(',', '.')) / float(bbTWL.replace(',', '.'))) * 100 
        pTNS = (1 - float(abTNS.replace(',', '.')) / float(bbTNS.replace(',', '.'))) * 100
        pWNS = (1 - float(abWNS.replace(',', '.')) / float(bbWNS.replace(',', '.'))) * 100
        
        #pHPWL1 = (1 - float(bbHPWL1.replace(',', '.')) / float(bbHPWL.replace(',', '.'))) * 100
        #pTWL1 = (1 - float(bbTWL1.replace(',', '.')) / float(bbTWL.replace(',', '.'))) * 100 
        #pTNS1 = (1 - float(bbTNS1.replace(',', '.')) / float(bbTNS.replace(',', '.'))) * 100
        #pWNS1 = (1 - float(bbWNS1.replace(',', '.')) / float(bbWNS.replace(',', '.'))) * 100
        
        #bTime = float(abTime1.replace(',', '.')) - float(abTime0.replace(',', '.'))
        AddBuf = float(abCells) - float(bbCells)

        #print metrics
        printStr = benchmark + ';'
        printStr += str(bbCells) + ';'
        printStr += str(abTime) + ';'
        #printStr += str(bTime) + ';'
        printStr += str(AddBuf) + ';'
        printStr += str(bbHPWL) + ';'
        #printStr += str(bbHPWL1) + ';'
        printStr += str(abHPWL) + ';'
        printStr += str(bbTWL) + ';'
        #printStr += str(bbTWL1) + ';'
        printStr += str(abTWL) + ';'
        printStr += str(bbTNS) + ';'
        #printStr += str(bbTNS1) + ';'
        printStr += str(abTNS) + ';'
        printStr += str(bbWNS) + ';'
        #printStr += str(bbWNS1) + ';'
        printStr += str(abWNS) + ';'      
        printStr += str(pHPWL) + ';'
        printStr += str(pTWL) + ';'
        printStr += str(pTNS) + ';'
        printStr += str(pWNS) + ';'
        
        #printStr += str(pHPWL1) + ';'
        #printStr += str(pTWL1) + ';'
        #printStr += str(pTNS1) + ';'
        #printStr += str(pWNS1) + ';'
        
        printStr += '\n'
        print(printStr)

        #write metrics
        table = open(reportTable, 'a')
        table.write(printStr)
        table.close()

def test():
    testRunner = TestRunner()
    metrics = ['HPWL', 'TWL', 'TNS', 'WNS']
    stages  = ['INIT', 'NBUF']
    cmdArgs = ''#--New_Buffering.Interval=20'
    e = BaseExperiment('IWLS05 new_buffering experiment (GBL)', 'New_buffering.cfg', 'IWLS05.list', 'MasterLogs/New_Buffering/IWLS', metrics, stages)
    testRunner.parameters.experiments.append(e)

    testRunner.Run()

#test()
