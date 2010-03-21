import subprocess
import sys
import os

import datetime
from datetime import date
import time

from Emailer import *
from SvnWorker import *

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

class TestRunner:
    svnRevision = ''
    parameters = TestRunnerParameters()

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

    def BuildSln(self, slnPath, mode = "Rebuild"):
        print('Building solution...')
        #args = [MSBuild, '.\itlPlace\make\itlPlace.sln', '/t:' + 'Rebuild', '/p:Configuration=Release']
        #subprocess.Popen(subprocess.list2cmdline(args)).communicate()
        subprocess.call(["BuildSolution.bat", slnPath, mode])

##    def BuildSln(slnPath, mode = "Rebuild"):
##        print('Building solution...')
##        args = [Tools.MSBuild, slnPath, "/t:" + mode, "/p:Configuration=Release"]
##        subprocess.Popen(subprocess.list2cmdline(args)).communicate()

    def GetPythonOutput(self, setName, cfgName):
        (path, cfgFileName) = os.path.split(cfgName)
        return "pythonOutput_{0}[{1}].csv".format(setName.lower(), cfgFileName)

    def OpenFilesList(self, listName):
        filesList = []
        groupNames = []
        group = []
        filesListInGroups = list(list())
        cfgCommentsList = []
        stringsList  = (open(listName).read()).split('\n')
        # Perform filtering:
        stringsList = [x for x in stringsList if not x.strip().startswith('#')]
        for idx, str in enumerate(stringsList):
            str = str.expandtabs()
            if str.startswith(' '):
                continue  # skip all cfg names in a group because they have already been processed
            if str.endswith(':'):
                groupNames.append(str)
                idx += 1
                if idx < len(stringsList):
                    while stringsList[idx].expandtabs().startswith(' '):
                        splittedLine = [piece.strip() for piece in stringsList[idx].split('--')]
                        filesList.append(splittedLine[0])
                        group.append(splittedLine[0])
                        if len(splittedLine) > 1:
                            cfgCommentsList.append(splittedLine[1])
                        else:
                            cfgCommentsList.append('')
                        idx += 1
                        if idx >= len(stringsList):
                            break;
                filesListInGroups.append(group)
            else:
                splittedLine = [piece.strip() for piece in str.split('--')]
                filesList.append(splittedLine[0])
                filesListInGroups.append([splittedLine[0]])
                if len(splittedLine) > 1:
                    cfgCommentsList.append(splittedLine[1])
                else:
                    cfgCommentsList.append('')
        return filesList, cfgCommentsList, filesListInGroups

    def PrepareAndSendMail(self, pythonOutput, subject, text, attachmentFiles):
        print("Sending mail with " + pythonOutput)
        smtpserver = 'mail.unn.ru'
        smtpuser = ''
        smtppass = ''

        RECIPIENTS = ['kirill.kornyakov@gmail.com']
        #RECIPIENTS = ['itlab.vlsi@www.software.unn.ru']
        #RECIPIENTS = ['zhivoderov.a@gmail.com']
        SENDER = 'VLSIMailerDaemon@gmail.com'

        send_mail(
            SENDER,     # from
            RECIPIENTS, # to
            subject,    # subject
            text,       # text
            attachmentFiles,         # attachment files
            smtpserver, # SMTP server
            25,         # port
            smtpuser,   # login
            smtppass,   # password
            0)          # TTLS
        print('Success!')

    def GroupAndSendFiles(self, filesListInGroups, setName, cfgNamesList, cfgCommentsList):
        # Collect all output files, group and send via e-mail
        for groupOfFiles in filesListInGroups:
            attachmentFiles = list()
            text = ''
            for file in groupOfFiles:
                outFileName = self.GetPythonOutput(setName, file)
                attachmentFiles.append(outFileName)
                cfgComment = cfgCommentsList[cfgNamesList.index(file)]
                text += '{0}: {1}\n'.format(outFileName, cfgComment)
            subject = 'Experiments results on {0} with {1}'.format(setName, str(groupOfFiles))
            text += 'svn rev. ' + self.svnRevision
            text += '\n\nThis is automatically generated mail. Please do not reply.'
            self.PrepareAndSendMail(str(attachmentFiles), subject, text, attachmentFiles)

    def RunTestsOnCfgList(self, setName):
        cfgNamesList, cfgCommentsList, filesListInGroups = self.OpenFilesList(GeneralParameters.binDir + setName + 'cfg.list')
        #print(str(filesListInGroups))
        if setName == GeneralParameters.iwls05:
            isDP = self.parameters.doIWLS05DP
            isBeforeDP = self.parameters.doIWLS05BeforeDP
        else:
            isDP = self.parameters.doISPD04DP
            isBeforeDP = self.parameters.doISPD04BeforeDP

        # Run all tests
        for idx in range(0, len(cfgNamesList)):
            outFileName = self.RunSet(setName, cfgNamesList[idx], cfgCommentsList[idx], isDP, isBeforeDP)

        if self.parameters.doSendMail:
            self.GroupAndSendFiles(filesListInGroups, setName, cfgNamesList, cfgCommentsList)

    def RunSet(self, setName, cfgName = '', cfgComment = '', isDP = True, isBeforeDP = True):
        testSet, comments, fake = self.OpenFilesList(GeneralParameters.binDir + setName + ".list")
        defaultCfgName = setName + ".cfg"
        if cfgName == "":
            cfgName = defaultCfgName
        isTimingPresent = False
        pythonOutput = self.GetPythonOutput(setName, cfgName)
        print('Config name = %s' % cfgName)
        print("Performing tests on the following set of benchmarks: " + ", ".join(testSet))

        po = open(pythonOutput, 'w')
        if setName == GeneralParameters.ispd04:
            printStr = ';'
            nColumnSets = 0
            if isBeforeDP:
                printStr += 'before DP;;'
                nColumnSets += 1
            if isDP:
                printStr += 'after DP'
                nColumnSets += 1
            po.write(printStr + '\n')
            po.write(nColumnSets*';HPWL;Time')
        else:
            printStr = ''
            nColumnSets = 0
            if isBeforeDP:
                printStr += 'before DP;;;;'
                nColumnSets += 1
            if isDP:
                printStr += 'after DP;;;;'
                nColumnSets += 1
            po.write(';' + 9*printStr)

            printStr = ''
            if isBeforeDP:
                printStr += 3 * ('before DP' + 12*';') + 3*';'
            if isDP:
                printStr += 3 * ('after DP' + 12*';')
            po.write(';' + printStr)
            po.write('\n')
            printStr = 9*nColumnSets*';HPWL;TNS;WNS;Time' + nColumnSets*(2*';' + 'TNS' + 8*';' + \
                       ';min;average;;WNS' + 8*';' + ';min;average;;HPWL' + 8*';' + ';min;average;;HPWL for min TNS;HPWL for min WNS')
            po.write(printStr)
        po.close()

        for benchmark in testSet:
            logFileName = GeneralParameters.binDir + setName + "\\" + benchmark + ".log"
            fPlacerOutput = open(logFileName, 'w');

            params = [GeneralParameters.binDir + "itlPlaceRelease.exe", cfgName,
                      "--params.def=.\\" + setName + "\\" + benchmark + ".def"]

            if setName == GeneralParameters.ispd04:
                params.append("--params.lef=.\\" + setName + "\\" + benchmark + ".lef")

            if setName == GeneralParameters.iwls05:
                params.append("--DesignFlow.Timing=true")

            if not isDP:
                params.append("--DesignFlow.DetailedPlacement=false")

            subprocess.call(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)
            fPlacerOutput.close()
            print(benchmark + ' is done...')
            self.svnRevision = self.ParseLog(logFileName, benchmark, pythonOutput, setName == Parameters.iwls05, isDP, isBeforeDP)
            #def ParseLog(logName, benchmark, pythonOutput, isTimingUsed, isDP = True, isBeforeDP = True):

        return pythonOutput

    def RunAll(self):
        cp = CoolPrinter()

        cp.CoolPrint('Start')

        svn = SvnWorker()

        if self.parameters.doCheckout:
            cp.CoolPrint('Delete sources and Checkout')
            svn.DeleteSources(GeneralParameters.checkoutPath)
            #TODO: implement non HEAD revision
            svn.CheckOut(RepoParameters.srcRepoPath, GeneralParameters.checkoutPath)

        if self.parameters.doBuild:
            cp.CoolPrint('Build')
            self.BuildSln(GeneralParameters.slnPath)

        if self.parameters.useISPD04:
            cp.CoolPrint('ISPD04 experiments')
            if self.parameters.doISPD04Checkout:
                #TODO: implement non HEAD revision
                svn.CheckOut(RepoParameters.benchRepoPath, GeneralParameters.benchmarkCheckoutPath)
                archiver = GeneralParameters.binDir + Tools.UnRar
                subprocess.call([archiver, "x", "-y", "ISPD04.zip"], cwd = GeneralParameters.benchmarkCheckoutPath)
            self.RunTestsOnCfgList(GeneralParameters.ispd04)

        if self.parameters.useIWLS05:
            cp.CoolPrint('ISPD04 experiments')
            if self.parameters.doISPD04Checkout:
                #TODO: implement non HEAD revision
                svn.CheckOut(RepoParameters.benchRepoPath, GeneralParameters.benchmarkCheckoutPath)
                archiver = GeneralParameters.binDir + Tools.UnRar
                subprocess.call([archiver, "x", "-y", "IWLS05.zip"], cwd = GeneralParameters.benchmarkCheckoutPath)
            self.RunTestsOnCfgList(GeneralParameters.iwls05)

        cp.CoolPrint('Finish')

def main():
    testRunner = TestRunner()
    testRunner.RunAll()

main()