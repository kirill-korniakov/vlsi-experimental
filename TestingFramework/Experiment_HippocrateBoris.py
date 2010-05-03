import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_HippocrateBoris(BaseExperiment):
    def CreateEmptyTable(self, reportTable):
        po = open(reportTable, 'w')
#        po.write('Benchmark;Methods;bbHPWL;abHPWL;relHPWL;bbTWL;abTWL;relTWL;bbTNS;abTNS;relTNS;bbWNS;abWNS;relWNS\n')
        po.write('Benchmark;Methods;relHPWL;relTWL;relTNS;relWNS\n')
        po.close()
	

    def findEnabledMethods(self, logName):
	import re
	result = ''
	for line in open(logName,'r'):
	    m = re.search('-------(.*?) ENABLED', line)
	    if (m!=None):
        	result += m.group(1) + ' '
	return result

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        #get metrics
        parser = LogParser(logName)

        bbHPWL = parser.GetFromPFST('INIT', 4)
        abHPWL = parser.GetFromPFST('HP', 4)
	relHPWL = (float(bbHPWL.replace(',', '.'))-float(abHPWL.replace(',', '.')))/float(bbHPWL.replace(',', '.'))*100

        bbTWL = parser.GetFromPFST('INIT', 5)
        abTWL = parser.GetFromPFST('HP', 5)
	relTWL = (float(bbTWL.replace(',', '.'))-float(abTWL.replace(',', '.')))/float(bbTWL.replace(',', '.'))*100

        bbTNS = parser.GetFromPFST('INIT', 6)
        abTNS = parser.GetFromPFST('HP', 6)
	relTNS = (float(bbTNS.replace(',', '.'))-float(abTNS.replace(',', '.')))/float(bbTNS.replace(',', '.'))*100

        bbWNS = parser.GetFromPFST('INIT', 7)
        abWNS = parser.GetFromPFST('HP', 7)
	relWNS = (float(bbWNS.replace(',', '.'))-float(abWNS.replace(',', '.')))/float(bbWNS.replace(',', '.'))*100

	

        #print metrics
        printStr = benchmark + ';'
	printStr += self.findEnabledMethods(logName) + ';'
#        printStr += str(bbHPWL) + ';'
 #       printStr += str(abHPWL) + ';'
	printStr += str(relHPWL) + ';'
#	printStr += str(bbTWL) + ';'
 #       printStr += str(abTWL) + ';'
	printStr += str(relTWL) + ';'
#        printStr += str(bbTNS) + ';'
 #       printStr += str(abTNS) + ';'
	printStr += str(relTNS) + ';'
#        printStr += str(bbWNS) + ';'
 #       printStr += str(abWNS) + ';'
	printStr += str(relWNS) + ';'
        printStr += '\n'
        print(printStr)

        #write metrics
        table = open(reportTable, 'a')
        table.write(printStr)
        table.close()

def test():
    testRunner = TestRunner()
	
    mylist='IWLS05_GP.list'
    myexperimentname='IWLS05 Hippocrate Boris experiment (HP)'

    Hswap = Experiment_HippocrateBoris(myexperimentname, 'boris_hippocrate_swap.cfg', mylist)
    Hmove = Experiment_HippocrateBoris(myexperimentname, 'boris_hippocrate_move.cfg', mylist)
    Hcenter = Experiment_HippocrateBoris(myexperimentname, 'boris_hippocrate_center.cfg', mylist)
    Hcompact = Experiment_HippocrateBoris(myexperimentname, 'boris_hippocrate_compact.cfg', mylist)
    Hall = Experiment_HippocrateBoris(myexperimentname, 'boris_hippocrate_all.cfg', mylist)

#    testRunner.parameters.experiments.append(Hswap)
#    testRunner.parameters.experiments.append(Hmove)
#    testRunner.parameters.experiments.append(Hcenter)
#    testRunner.parameters.experiments.append(Hcompact)
    testRunner.parameters.experiments.append(Hall)

    testRunner.Run()

test()