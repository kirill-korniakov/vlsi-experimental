import LogParser
from LogParser import *

def ParsePQAT(logFolder = ''):
    metrics = ['HPWL', 'TNS', 'WNS']

    if (logFolder == ''):
      logFolder = os.getcwd()

    for log in os.listdir(logFolder):
      if (os.path.isfile(os.path.join(logFolder, log)) and ('.log' == os.path.splitext(log)[-1])):
        parser = LogParser(logFolder + '/' + log)
        parser.ParsePQATAndPrintTable(metrics)

def Run():
    logFolder = '../Reports/LR.cfg_2010-10-07_22-37-59'

    if (os.path.exists(logFolder) == False):
        print('folder ' + logFolder + 'does not exist')
        return

    ParsePQAT(logFolder)

Run()
