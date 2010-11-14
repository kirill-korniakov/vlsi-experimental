import LogParser
from LogParser import *

import CoreFunctions
from CoreFunctions import PrintTableToFile, MakeTableInPercents, ExtractXYFromTable

import ChartPlotter
from ChartPlotter import *

def ParseAndPlotPQAT(logFolder, doPlotCharts):
    metrics = ['HPWL', 'TNS', 'WNS']

    if (logFolder == ''):
      logFolder = os.getcwd()

    for log in os.listdir(logFolder):
      if (os.path.isfile(os.path.join(logFolder, log)) and ('.log' == os.path.splitext(log)[-1])):
        parser = LogParser(logFolder + '/' + log)
        table  = parser.ParsePQAT(metrics)

        if (table == []):
            print('Error: table is empty')
            return

        table        = MakeTableInPercents(table)
        PQATFileName = logFolder + '/' + os.path.basename(log) + '.csv'
        PrintTableToFile(PQATFileName, table, metrics)

        #Plot
        del table[0] #don't use values of 0 iteration
        [xValues, yValues] = ExtractXYFromTable(table)

        if (doPlotCharts):
            PlotChartForBenchmark(logFolder + '/' + log, xValues, yValues)

def Run():
    #logFolder    = '../Reports/LR_make_charts'
    #logFolder    = '../Reports/Relaxation_SGNW'
    #logFolder    = '../Reports/Relaxation_APlace'
    logFolder    = '../Reports/hpwl_iwls05'
    doPlotCharts = True

    if (os.path.exists(logFolder) == False):
        print('folder ' + logFolder + 'does not exist')
        return

    ParseAndPlotPQAT(logFolder, doPlotCharts)

Run()
