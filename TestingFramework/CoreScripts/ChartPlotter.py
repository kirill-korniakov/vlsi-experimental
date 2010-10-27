def PlotChartForBenchmark(benchmark, table):
    try:
        import matplotlib.pyplot as plt
        from matplotlib.pyplot import figure, savefig

    except ImportError:
        print('Unable to plot charts: matplotlib needed')
        return

    xValues = []
    yValues = []
    del table[0] #don't use values of 0 iteration

    for currStage in table:
        xValues.append(currStage[1])
        yValues.append(currStage[0])

    fig = plt.figure()
    plt.plot(xValues, yValues)
    plt.plot(xValues, yValues, 'bo')
    plt.plot([xValues[0]], [yValues[0]], 'ro')
    plt.ylabel('HPWL')
    plt.xlabel('TNS')
    #plt.show()
    fig.savefig(benchmark + '.png')