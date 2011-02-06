import os

import datetime
from datetime import date
import time

END_OF_COLUMN = ';'

class Logger:
    startTime   = time.time()
    logFileName = ".//TF.log"

    def LogWorkTime(self):
        runTime = time.time() - self.startTime
        self.Log("RUNTIME %2.2i:%2.2i" % (int(runTime / 60), int(runTime % 60)))

    def CoolLog(self, message):
        self.Log('\n')
        self.Log("######################################################")
        self.Log("####### %s" % (message))
        self.Log("######################################################")
        self.LogWorkTime()
        self.Log('\n')

    def Log(self, message):
        print(message)
        log = open(self.logFileName, 'a')
        log.write(message + '\n')
        log.close()

    def LogStartMessage(self):
        self.CoolLog("Started on %s" % (GetTimeStamp()))

def GetTimeStamp():
    return datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

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
        po.write((str(Absolutize(sequence)).replace(", ", ";")[1:-1] +
                ';' + str(min(sequence)) + ';' + str(Average(sequence))).replace(".", ","))
        po.write(2*';')

def Average(values):
    """Computes the arithmetic mean of a list of numbers.
    >>> print(average([20, 30, 70]))
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
#                print('remove file')
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
      return 'same'

    value1 = float(value1)
    value2 = float(value2)

    if (abs(value1 - value2) < eps):
      return 'equal'

    return 'notEqual'

def ReportErrorAndExit(error, logger, emailer):
    logger.Log(error)
    emailer.SendMessageAndExit(error)

def MarkResultAsBest(col):
    return '--> ' + col

def WriteStringToFile(cols, tableFileName):
  printStr = ''

  for col in cols:
    if (col == END_OF_COLUMN):
      printStr += ';'

    else:
      printStr += col

  printStr += '\n'
  resultFile = open(tableFileName, 'a')
  resultFile.write(printStr.replace('.', ','))
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
    cols = ['stage', END_OF_COLUMN]

    for col in metrics:
        cols += [col, END_OF_COLUMN]

    WriteStringToFile(cols, tableFileName)
    currStageIdx = 0

    for currStage in table:
        currStageName = str(currStageIdx)

        if (stages):
            currStageName = stages[currStageIdx]

        cols = [currStageName, END_OF_COLUMN]

        for value in currStage:
            cols += [str(value), END_OF_COLUMN]

        WriteStringToFile(cols, tableFileName)
        currStageIdx += 1