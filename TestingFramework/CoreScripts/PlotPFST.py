import os

from LogParser import LogParser
from ChartPlotter import PlotChartForBenchmark
from CoreFunctions import PrintTableToFile, ExtractXYFromTable, MakeTableInPercents

def PrintAndPlotPFST(logName, metrics, stages):
  parser = LogParser(logName)
  table  = parser.ParsePFST(metrics, stages)

  if (table == []):
      print("Error: table is empty")
      return

  table  = MakeTableInPercents(table)
  PrintTableToFile(r"%s.csv" % (logName), table, metrics, stages)

  del table[0] #don"t use values of 0 iteration
  [xValues, yValues] = ExtractXYFromTable(table)
  PlotChartForBenchmark(logName, xValues, "TNS", yValues, "HPWL")

def Run():
  lastLogFolder = r"../Reports/HDP"
  #lastLogFolder = r"../Reports/SGNW"
  #lastLogFolder = r"../Reports/APlace_weighting"

  metrics = ["HPWL", "TNS"]
  stages  = ["INIT", "LEG1", "LEG2", "LEG3", "LEG4", "LEG5", "LEG6", "LEG7", "LEG8", "LEG9"]
  stages  = ["INIT", "LEG", "DP"]

  if (lastLogFolder == ""):
    lastLogFolder = os.getcwd()

  if (os.path.exists(lastLogFolder) == False):
    print("folder %s does not exist" % (lastLogFolder))
    return

  for log in os.listdir(lastLogFolder):
    if (os.path.isfile(os.path.join(lastLogFolder, log)) and (".log" == os.path.splitext(log)[-1])):
      PrintAndPlotPFST(os.path.join(lastLogFolder, log), metrics, stages)

  print("Success")

if (__name__ == "__main__"):
  Run()
