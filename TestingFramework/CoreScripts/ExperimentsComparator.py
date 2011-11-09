from CoreFunctions import Logger, GetTimeStamp, END_OF_COLUMN, WriteStringToFile, CompareValues,\
                          NOT_EQUAL, MarkResultAsBest

from ReportCreator import ReportCreator

class ExperimentsComparator:
  logger               = None
  storage              = None
  masterExperiment     = None
  masterResuts         = {} #benchmark: pfst
  experimentsToCompare = {} #Group of experiments. Their results will be compared
                            #experimentName: {benchmark: pfst}

  def __init__(self, storage):
    self.storage              = storage
    self.masterExperiment     = None
    self.masterResuts         = {}
    self.experimentsToCompare = {}

  def AddExperimentToGroup(self, newExperiment):
    if (self.masterExperiment == None):
      self.masterExperiment = newExperiment

    else:
      if (newExperiment.benchmarks != self.masterExperiment.benchmarks):
        self.logger.Log("Error: list files are not equal!")
        exit(1)

      if (newExperiment.metrics != self.masterExperiment.metrics):
        self.logger.Log("Error: metrics are not equal!")
        exit(1)

      self.experimentsToCompare[newExperiment.name] = {}

  def GetResultsForExperiment(self, experimentName):
    if (experimentName in self.storage.experimentResults):
      return self.storage.experimentResults[self.masterExperiment.name].pfstTables

    self.logger.Log("Error: results for %s not found" % (experimentName))
    return {}

  def GetExperimentsResults(self):
    #Masater experiment
    self.masterResuts = self.GetResultsForExperiment(self.masterExperiment.name)

    #Normal experiments
    for experimentName in self.experimentsToCompare.iterkeys():
      self.experimentsToCompare[experimentName] = self.GetResultsForExperiment(experimentName)

  def PrintLabelAndBlanks(self, cols, label, nMetrics):
    cols.extend([END_OF_COLUMN, label])

    for col in nMetrics:
      cols.append(END_OF_COLUMN)

  def PrintTableHeader(self, resultFileName):
    metrics  = self.masterExperiment.metrics
    nMetrics = range(len(metrics))

    # First line of the table header
    cols = []

    self.PrintLabelAndBlanks(cols, "INIT", nMetrics)
    self.PrintLabelAndBlanks(cols, self.masterExperiment.name, nMetrics)

    for experimentName in self.experimentsToCompare.iterkeys():
      self.PrintLabelAndBlanks(cols, experimentName, nMetrics)

    WriteStringToFile(cols, resultFileName)

    # Second line of the table header
    cols = ["benchmark", END_OF_COLUMN]

    #Init values
    for row in nMetrics:
      cols.extend([metrics[row], END_OF_COLUMN])

    cols.append(END_OF_COLUMN)

    #Target experiment's values
    for row in nMetrics:
      cols.extend([metrics[row], END_OF_COLUMN])

    cols.append(END_OF_COLUMN)
    nNormalExperiments = len(self.experimentsToCompare)

    for experimentName in xrange(nNormalExperiments):
      for row in nMetrics:
        if (metrics[row] != "Time"):
          cols.extend([metrics[row] + "%", END_OF_COLUMN])

        else:
          cols.extend([metrics[row], END_OF_COLUMN])

      cols.append(END_OF_COLUMN)

    WriteStringToFile(cols, resultFileName)

  def MakeResultTable(self, resultFileName):
    self.PrintTableHeader(resultFileName)
    benchmarks = list(self.masterResuts.keys())

    for benchmark in benchmarks:
      newTableLine = self.CreateNewTableLine(benchmark)
      WriteStringToFile(newTableLine, resultFileName)

  def CreateNewTableLine(self, benchmark):
    metrics         = self.masterExperiment.metrics
    nMetrics        = range(len(metrics))
    bestValues      = [1000000 for i in nMetrics]
    bestValuesIdx   = [0 for i in nMetrics]
    experimentsLine = []

    # INIT metrics
    initialMetrics     = []
    initialMetricsLine = []

    # reference metrics
    referenceMetrics = []
    resultValues     = self.masterResuts[benchmark]

    if (resultValues != []): #If target experiment succeed on current benchmark
      for metricIdx in nMetrics:
        #Fill reference metrics
        finalStageIdx = len(self.masterExperiment.stages) - 1
        value         = resultValues[finalStageIdx][metricIdx]
        experimentsLine.extend([str(value), END_OF_COLUMN])
        referenceMetrics.append(value)

        #Fill Initial metrics
        value = resultValues[0][metricIdx]
        initialMetricsLine.extend([str(value), END_OF_COLUMN])
        initialMetrics.append(value)

      initialMetricsLine.append(END_OF_COLUMN)

    else: #Target experiment failed
      referenceMetrics = initialMetrics

      for metricIdx in nMetrics:
        experimentsLine.extend(["N/A", END_OF_COLUMN])

    experimentsLine.append(END_OF_COLUMN)

    # normal experiments
    for experimentName, experimentResults in self.experimentsToCompare.iteritems():
      resultValues = experimentResults[benchmark]

      if (resultValues == []):
        for metricIdx in nMetrics:
          experimentsLine.extend(["N/A", END_OF_COLUMN])

        experimentsLine.append(END_OF_COLUMN)
        continue

      #if 'INITIAL' metrics haven't been printed yet
      if (initialMetrics == []):
          #take them from the table of this experiment
          for metricIdx in nMetrics:
            value = resultValues[0][metricIdx]
            initialMetricsLine.extend([str(value), END_OF_COLUMN])
            initialMetrics.append(value)

          initialMetricsLine.append(END_OF_COLUMN)

      #else compare 'INITIAL' metrics
      else:
        for metricIdx in nMetrics:
          cmpResult = CompareValues(initialMetrics[metricIdx], resultValues[0][metricIdx])

          if (cmpResult == NOT_EQUAL and metrics[metricIdx] != "Time"):
            self.logger.Log("Error: not equal INIT metrics")

      finalStageIdx = - 1 #Index of the last elemnt in the list

      for metricIdx in nMetrics:
        valueStr = resultValues[finalStageIdx][metricIdx]

        if (metrics[metricIdx] != "Time"):
          percent  = 100 * valueStr / referenceMetrics[metricIdx]
          valueStr = "%.2f" % percent

        if (percent < bestValues[metricIdx]):
          bestValues[metricIdx]    = percent #remember the best result
          bestValuesIdx[metricIdx] = len(experimentsLine) #and its index

        experimentsLine.extend([valueStr, END_OF_COLUMN])

      experimentsLine.append(END_OF_COLUMN)

    #for idx in bestValuesIdx:
    #  if (idx > 0):
    #    experimentsLine[idx] = MarkResultAsBest(experimentsLine[idx])

    if (initialMetricsLine == []):
      for metricIdx in nMetrics:
        initialMetricsLine.extend(["N/A", END_OF_COLUMN])

      initialMetricsLine.append(END_OF_COLUMN)

    newTableLine = [benchmark, END_OF_COLUMN]
    newTableLine.extend(initialMetricsLine)
    newTableLine.extend(experimentsLine)
    return newTableLine

  def CompareExperiments(self, reportParameters):
    if (len(self.experimentsToCompare) > 0):
      self.logger = Logger()
      self.logger.CoolLog("Comparing experiments")

      try:
        reportCreator = ReportCreator("Comparing", "Comparing %s" % (GetTimeStamp()), reportParameters)
        logFolder     = reportCreator.CreateLogFolder()
        cmpFileName   = reportCreator.GetReportTableName()
        self.GetExperimentsResults()
        self.MakeResultTable(cmpFileName)

      except:
        import traceback
        self.logger.Log("exception: %s" % (traceback.format_exc()))

      self.logger.Log("Comparing finished")
