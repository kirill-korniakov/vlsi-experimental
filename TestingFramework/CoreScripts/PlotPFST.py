import LogParser
from LogParser import *

import ChartPlotter
from ChartPlotter import *

from CoreFunctions import PrintTableToFile

def PrintAndPlotPFST(logName, metrics, stages):
    parser = LogParser(logName)
    table  = parser.ParsePFST(metrics, stages)
    table  = MakeTableInPercents(table)
    PrintTableToFile(logName + '.csv', table, metrics, stages)
    PlotChartForBenchmark(logName, table)

def Run():
    #lastLogFolder = '../Reports/SGNW'
    lastLogFolder   = '../Reports/APlace_weighting'
    metrics = ['HPWL', 'TNS']
    stages  = ['INIT', 'LEG1', 'LEG2', 'LEG3', 'LEG4', 'LEG5', 'LEG6', 'LEG7', 'LEG8', 'LEG9']

    if (lastLogFolder == ''):
        lastLogFolder = os.getcwd()

    if (os.path.exists(lastLogFolder) == False):
        print('folder ' + lastLogFolder + ' does not exist')
        return

    for log in os.listdir(lastLogFolder):
        if (os.path.isfile(os.path.join(lastLogFolder, log)) and ('.log' == os.path.splitext(log)[-1])):
            PrintAndPlotPFST(lastLogFolder + '/' + log, metrics, stages)

Run()