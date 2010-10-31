import LogParser
from LogParser import *

import CoreFunctions
from CoreFunctions import PrintTableToFile, MakeTableInPercents

import ChartPlotter
from ChartPlotter import *

def ParsePQAT(logFolder, doPlotCharts):
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

        if (doPlotCharts):
            PlotChartForBenchmark(logFolder + '/' + log, table)

def Run():
    logFolder    = '../Reports/LR_make_charts'
    doPlotCharts = True

    if (os.path.exists(logFolder) == False):
        print('folder ' + logFolder + 'does not exist')
        return

    ParsePQAT(logFolder, doPlotCharts)

Run()
