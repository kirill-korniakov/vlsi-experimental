import os
import time
import datetime
from datetime import date
from ConfigParser import ConfigParser

CONFIG_FILE   = "Parameters.conf"
END_OF_COLUMN = ";"

SAME      = "same"
EQUAL     = "equal"
NOT_EQUAL = "NotEqual"

class Logger:
    startTime   = time.time()
    logFileName = r"./TF.log"

    def LogWorkTime(self):
        runTime = time.time() - self.startTime
        self.Log("RUNTIME %2.2i:%2.2i" % (int(runTime / 60), int(runTime % 60)))

    def CoolLog(self, message):
        self.Log("\n")
        self.Log("######################################################")
        self.Log("####### %s" % (message))
        self.Log("######################################################")
        self.LogWorkTime()
        self.Log("\n")

    def Log(self, message):
        print(message)
        log = open(self.logFileName, 'a')
        log.write(("%s\n") % (message))
        log.close()

    def LogStartMessage(self):
        self.CoolLog("Started on %s" % (GetTimeStamp()))

def GetTimeStamp():
    return datetime.datetime.now().strftime("%Y-%m-%d %H-%M-%S")

def Absolutize(x):
    divisor = x[0]
    if divisor != 0.0:
        for i in range(0, len(x)):
            x[i] = x[i] / divisor
    else:
        for i in range(0, len(x)):
            x[i] = 0.0
    return x

def PrintAbsValues(po, sequence):
    if len(sequence):
        po.write(("%s;%s;%s" % (str(Absolutize(sequence)).replace(", ", ";")[1:-1], str(min(sequence)),\
                                str(Average(sequence)))).replace(".", ","))
        po.write(2 * ";")

def Average(values):
    """Computes the arithmetic mean of a list of numbers.
    >>> print(Average([20, 30, 70]))
    40.0
    """
    return sum(values) / len(values)

def RemoveDir(dirName):
        RemovePermissions(dirName)
        for name in os.listdir(dirName):
            file = os.path.join(dirName, name)
            if not os.path.islink(file) and os.path.isdir(file):
                RemoveDir(file)
            else:
                RemovePermissions(file)
                os.remove(file)
        try:
            os.rmdir(dirName)
        except WindowsError:
            pass

def RemovePermissions(filePath) :
    if (not os.access(filePath, os.W_OK)):
        os.chmod(filePath, 666)

def CompareValues(value1, value2, eps = 0.001):
    if (value1 == value2):
      return SAME

    value1 = float(value1)
    value2 = float(value2)

    if (abs(value1 - value2) < eps):
      return EQUAL

    return NOT_EQUAL

def ReportErrorAndExit(error, logger, emailer):
    logger.Log(error)
    emailer.SendMessageAndExit(error)

def MarkResultAsBest(col):
    return "* " + col

def WriteStringToFile(cols, tableFileName):
  printStr = ""

  for col in cols:
    if (col == END_OF_COLUMN):
      printStr += ";"
    else:
      printStr += str(col)

  printStr += "\n"
  resultFile = open(tableFileName, 'a')
  resultFile.write(printStr.replace(".", ","))
  resultFile.close()

def MakeTableInPercents(table):
  currStageIdx = 0

  for stage in table:
    for col in range(len(stage)):
      if ((currStageIdx > 0) and (table[0][col] > 0)):
        table[currStageIdx][col] = stage[col] * 100 / table[0][col]

    currStageIdx += 1

  return table

def ExtractXYFromTable(table):
  xValues = []
  yValues = []

  for currStage in table:
    xValues.append(currStage[1])
    yValues.append(currStage[0])

  return (xValues, yValues)

def PrintTableToFile(tableFileName, table, metrics, stages = []):
  cols = ["stage", END_OF_COLUMN]

  for col in metrics:
    cols.extend([col, END_OF_COLUMN])

  WriteStringToFile(cols, tableFileName)
  currStageIdx = 0

  for currStage in table:
    currStageName = str(currStageIdx)

    if (stages):
      currStageName = stages[currStageIdx]

    cols = [currStageName, END_OF_COLUMN]

    for value in currStage:
      cols.extend([str(value), END_OF_COLUMN])

    WriteStringToFile(cols, tableFileName)
    currStageIdx += 1

def CreateConfigParser():
  cfgParser  = ConfigParser()
  parentDir  = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
  configFile = os.path.join(parentDir, CONFIG_FILE)
  cfgParser.read(configFile)
  return cfgParser


