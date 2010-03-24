import subprocess
import sys
import os

import datetime
from datetime import date
import time

from Emailer    import *
from SvnWorker  import *
from Reporter   import *

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

class TestRunner:
    svnRevision = ''
    parameters = ''
    reporter = 0

    def __init__(self, parameters = TestRunnerParameters()):
        self.parameters = parameters
        self.reporter = Reporter()

    def BuildSln(self, slnPath, mode = "Rebuild"):
        print('Building solution...')
        subprocess.call(["BuildSolution.bat", slnPath, mode])
        #TODO: try another option (next two lines), without bat-file
        #args = [MSBuild, '.\itlPlace\make\itlPlace.sln', '/t:' + 'Rebuild', '/p:Configuration=Release']
        #subprocess.Popen(subprocess.list2cmdline(args)).communicate()

    def OpenFilesList(self, listName):
        filesList = []
        groupNames = []
        group = []
        filesListInGroups = list(list())
        cfgCommentsList = []
        stringsList  = (open(listName).read()).split('\n')

        # Perform filtering of commented by # benchmarks
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

        send_mail(
            parameters.sender,     # from
            parameters.recipients, # to
            subject,               # subject
            text,                  # text
            attachmentFiles,       # attachment files
            parameters.smtpserver, # SMTP server
            25,                    # port
            parameters.smtpuser,   # login
            parameters.smtppass,   # password
            0)                     # TTLS

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
        cfgNamesList, cfgCommentsList, filesListInGroups = self.OpenFilesList(self.parameters.benchmarks)

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

    def ExtractBenchmarkList(self, benchmarksListPath):
        benchmarks = (open(benchmarksListPath).read()).split('\n')

        # Perform filtering of empty lines and commented by # benchmarks
        benchmarks = [x for x in benchmarks if not x.strip().startswith('#')]
        benchmarks = [x for x in benchmarks if len(x.strip())]

        return benchmarks

    def RunSet(self, cfgName, benchmarksListPath):
        benchmarks = self.ExtractBenchmarkList(benchmarksListPath)

        pythonOutput = self.reporter.GetPythonOutput(cfgName)
        print('Config name = %s' % cfgName)
        print("Performing tests on the following set of benchmarks:\n" + ", ".join(benchmarks))

        self.reporter.CreateEmptyTable(pythonOutput)

        logFolder = os.path.basename(cfgName)
        if os.path.exists(logFolder):
            newFolderName = logFolder + "_backup_from_" + GetTimeStamp()
            os.rename(logFolder, newFolderName)
        os.mkdir(logFolder)

        for benchmark in benchmarks:
            logFileName = logFolder + "\\" + benchmark + ".log" # + "\\" +
            fPlacerOutput = open(logFileName, 'w');

            defFile = "--params.def=" + os.path.dirname(benchmarksListPath) + "\\" + benchmark + ".def"
            lefFile = "--params.lef=" + os.path.dirname(benchmarksListPath) + "\\" + benchmark + ".lef"
            params = [GeneralParameters.binDir + "itlPlaceRelease.exe",
                      cfgName, defFile, lefFile]
            #params.append()

            #subprocess.call(params, stdout = fPlacerOutput, cwd = GeneralParameters.binDir)
            fPlacerOutput.close()
            print(benchmark + ' is done...')
            self.svnRevision = self.reporter.ParseLog(logFileName, benchmark, pythonOutput)

        return pythonOutput

    def RunAll(self):
        cp = CoolPrinter()
        svn = SvnWorker()

        cp.CoolPrint('Start')

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
            #self.RunTestsOnCfgList(GeneralParameters.ispd04)
            print(self.parameters.benchmarks)
            self.RunSet(self.parameters.cfg, self.parameters.benchmarks)

        cp.CoolPrint('Finish')

def main():
    testRunner = TestRunner()
    testRunner.RunAll()

main()