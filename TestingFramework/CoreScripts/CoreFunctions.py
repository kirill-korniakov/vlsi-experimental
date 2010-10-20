import os

import datetime
from datetime import date
import time

END_OF_COLUMN = ';'

class CoolPrinter:
    startTime = 0

    def __init__(self):
        self.startTime = time.time()

    def PrintWorkTime(self):
        runTime = time.time() - self.startTime
        print("RUNTIME %2.2i:%2.2i" % (int(runTime/60), int(runTime%60)))

    def CoolPrint(self, message):
        print('\n')
        print('######################################################')
        print('####### ' + message)
        print('######################################################')
        self.PrintWorkTime()
        print('\n')

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

def PrintPQATToFile(PQATTable, metrics, tableFileName):
    cols = ['stage', END_OF_COLUMN]

    for col in metrics:
        cols += [col, END_OF_COLUMN]

    WriteStringToFile(cols, tableFileName)

    for currStage in PQATTable:
        cols = [str(currStage), END_OF_COLUMN]

        for col in range(len(metrics)):
            cols += [str(PQATTable[currStage][col]), END_OF_COLUMN]

        WriteStringToFile(cols, tableFileName)
