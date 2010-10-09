import LogParser
from LogParser import *

def PlotChartForBenchmark(benchmark, table):
    try:
        import matplotlib.pyplot as plt
        from matplotlib.pyplot import figure, savefig

    except ImportError:
        print('Unable to plot charts: matplotlib needed')

    xValues = []
    yValues = []
    del table[0] #don't use values of 0 iteration

    for currStage in table.keys():
        xValues.append(table[currStage][1])
        yValues.append(table[currStage][0])

    fig = plt.figure()
    plt.plot(xValues, yValues)
    plt.plot(xValues, yValues, 'bo')
    plt.plot([xValues[0]], [yValues[0]], 'ro')
    plt.ylabel('HPWL')
    plt.xlabel('TNS')
    #plt.show()
    fig.savefig(benchmark + '.png')

def ParsePQAT(logFolder, doPlotCharts):
    metrics = ['HPWL', 'TNS', 'WNS']

    if (logFolder == ''):
      logFolder = os.getcwd()

    for log in os.listdir(logFolder):
      if (os.path.isfile(os.path.join(logFolder, log)) and ('.log' == os.path.splitext(log)[-1])):
        parser = LogParser(logFolder + '/' + log)
        table  = parser.ParsePQATAndPrintTable(metrics)

        if (doPlotCharts):
            PlotChartForBenchmark(logFolder + '/' + log, table)

def Run():
    #logFolder    = '../Reports/LR.cfg_2010-10-07_22-37-59'
    logFolder    = '../Reports/LR_make_charts'
    doPlotCharts = True

    if (os.path.exists(logFolder) == False):
        print('folder ' + logFolder + 'does not exist')
        return

    ParsePQAT(logFolder, doPlotCharts)

Run()
