def PlotChartForBenchmark(benchmark, xValues, xMetric, yValues, yMetric):
    try:
        import matplotlib.pyplot as plt
        from matplotlib.pyplot import figure, savefig

    except ImportError:
        print('Unable to plot charts: matplotlib needed')
        return

    fig = plt.figure()
    plt.plot(xValues, yValues)
    plt.plot(xValues, yValues, 'bo')
    plt.plot([xValues[0]], [yValues[0]], 'ro')
    plt.xlabel(xMetric)
    plt.ylabel(yMetric)
    plt.grid(True)
    #plt.show()
    fig.savefig(benchmark + '.png')