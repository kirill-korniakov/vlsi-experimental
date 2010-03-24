import subprocess
import sys
import os

import CoreFunctions
from CoreFunctions import *

class Reporter:

    def GetPythonOutput(self, cfgName):
        (path, cfgFileName) = os.path.split(cfgName)
        return "pythonOutput_{0}.csv".format(cfgFileName)

    def CreateEmptyTable(self, pythonOutput):
        po = open(pythonOutput, 'w')
        po.write('Benchmark;HPWL;Time')
        po.close()

    def ParseLog(self, logName, benchmark, pythonOutput, isTimingUsed, isDP = True, isBeforeDP = True):
        fh = open(logName, 'r')
        po = open(pythonOutput, 'a')
        po.write('\n' + benchmark + ';')    # set the benchmark name
        isBeforeTiming = False
        isAfterTiming  = False
        workTime = ""

        HPWLsBDP     = [] # Before DP
        TNSsBDP      = []
        WNSsBDP      = []
        workTimesBDP = []

        HPWLsADP     = [] # After  DP
        TNSsADP      = []
        WNSsADP      = []
        workTimesADP = []

        for line in fh.readlines():
            idx = line.find(GeneralParameters.repoPath)
            if idx != -1:
                idx = line.find('Revision ') + len('Revision ')
                svnRevision = line[idx:idx + 4]
                print('svnRevision = ' + svnRevision)
            if isBeforeDP:
                idx = line.find('HPWL after legalization: ')
                if idx != -1:
                    HPWL = line[idx + len('HPWL after legalization: '):-1]
                    HPWLsBDP.append(float(HPWL))

                    # get time
                    workTimesBDP.append(line[1:11].replace('.', ',') + ';')
                    continue

                if isTimingUsed:
                    idx = line.find('STA after legalization:\n')
                    if idx != -1:
                        isBeforeTiming = True
                        continue
                    idx = line.find('  TNS: ')
                    if (isBeforeTiming) and (idx != -1):
                        TNS = line[idx + len('  TNS: '):-1]
                        TNSsBDP.append(float(TNS))
                        continue
                    idx = line.find('  WNS: ')
                    if (isBeforeTiming) and (idx != -1):
                        WNS = line[idx + len('  WNS: '):-1]
                        WNSsBDP.append(float(WNS))
                        isBeforeTiming = False
                        continue

            if isDP:
                idx = line.find('HPWL after  detailed placement: ')
                if idx != -1:
                    HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
                    HPWLsADP.append(float(HPWL))

                    # get time
                    workTimesADP.append(line[1:11].replace('.', ',') + ';')
                    continue

                if isTimingUsed:
                    idx = line.find('STA after detailed placement:\n')
                    if idx != -1:
                        isAfterTiming = True
                        continue
                    idx = line.find('  TNS: ')
                    if (isAfterTiming) and (idx != -1):
                        TNS = line[idx + len('  TNS: '):-1]
                        TNSsADP.append(float(TNS))
                        continue
                    idx = line.find('  WNS: ')
                    if (isAfterTiming) and (idx != -1):
                        WNS = line[idx + len('  WNS: '):-1]
                        WNSsADP.append(float(WNS))
                        isAfterTiming = False

        for i in range(0, max(len(HPWLsBDP), len(HPWLsADP))):
            printStr = ''
            if isBeforeDP:
                printStr += str(HPWLsBDP[i]).replace('.', ',') + ';'
                if isTimingUsed:
                    printStr += str(TNSsBDP[i]).replace('.', ',') + ';' + str(WNSsBDP[i]).replace('.', ',') + ';'
                printStr += workTimesBDP[i]

            if isDP:
                printStr += str(HPWLsADP[i]).replace('.', ',') + ';'
                if isTimingUsed:
                    printStr += str(TNSsADP[i]).replace('.', ',') + ';' + str(WNSsADP[i]).replace('.', ',') + ';'
                printStr += workTimesADP[i]

            po.write(printStr)

        if isTimingUsed:
            po.write(';')
            PrintAbsValues(po, TNSsBDP)
            PrintAbsValues(po, WNSsBDP)
            PrintAbsValues(po, HPWLsBDP)

            po.write(str(HPWLsBDP[TNSsBDP.index(min(TNSsBDP))] / HPWLsBDP[0]).replace('.', ','))
            po.write(';')
            po.write(str(HPWLsBDP[WNSsBDP.index(min(WNSsBDP))] / HPWLsBDP[0]).replace('.', ','))
            po.write(2*';')

            PrintAbsValues(po, TNSsADP)
            PrintAbsValues(po, WNSsADP)
            PrintAbsValues(po, HPWLsADP)

            po.write(str(HPWLsADP[TNSsADP.index(min(TNSsADP))] / HPWLsADP[0]).replace('.', ','))
            po.write(';')
            po.write(str(HPWLsADP[WNSsADP.index(min(WNSsADP))] / HPWLsADP[0]).replace('.', ','))
            po.write(2*';')
        po.close()
        fh.close()

        return svnRevision