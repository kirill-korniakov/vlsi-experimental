import subprocess
import sys
from PyQt4 import QtGui
from PyQt4 import QtCore
import datetime
from datetime import date
import time
import os
from itlPlaceEmail import send_mail

repoPath = "http://svn.software.unn.ru/VLSI/CODE/trunk"
ispd04   = "ISPD04"
iwls05   = "IWLS05"
binDir   = ".\\itlPlace\\bin\\"
MSBuild  = "c:\\WINDOWS\\Microsoft.NET\\Framework\\v3.5\\MSBuild.exe"

windowTitle = 'Automatic Building Script iltPlace'

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

def ParseLog(logName, benchmark, pythonOutput, isTimingUsed, isDP = True, isBeforeDP = True):
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

class DistributionBuilder(QtGui.QWidget):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.setGeometry(300, 300, 320, 100)
        self.setWindowTitle(windowTitle)

        #field for SVN revision
        lblRev = QtGui.QLabel('SVN Revision (keep empty for HEAD)', self)
        self.teditRev = QtGui.QLineEdit('', self)

        #field for bugzilla issue number
        #lblIssue = QtGui.QLabel('Bugzilla issue (keep empty for empty)', self)
        #self.teditIssue = QtGui.QLineEdit('', self)

        self.cbCheckout = QtGui.QCheckBox('Checkout', self)
        self.cbCheckout.toggle();

        self.cbRebuild = QtGui.QCheckBox('Build', self)
        self.cbRebuild.toggle();

        self.gridGroupBoxISPD = QtGui.QGroupBox(self.tr(ispd04))
        layoutISPD = QtGui.QGridLayout()
        self.cbISPD04 = QtGui.QCheckBox(ispd04, self)
        #self.cbISPD04.toggle()
        layoutISPD.addWidget(self.cbISPD04, 9, 0)
        self.cbISPD04BDP = QtGui.QCheckBox('Collect BeforeDP data', self)
        self.cbISPD04BDP.toggle()
        layoutISPD.addWidget(self.cbISPD04BDP, 9, 1)
        self.cbISPD04isDP = QtGui.QCheckBox('Perform DP', self)
        self.cbISPD04isDP.toggle()
        layoutISPD.addWidget(self.cbISPD04isDP, 9, 2)
        
        self.gridGroupBoxIWLS = QtGui.QGroupBox(self.tr(iwls05))
        layoutIWLS = QtGui.QGridLayout()
        self.cbIWLS05 = QtGui.QCheckBox(iwls05, self)
        self.cbIWLS05.toggle()
        layoutIWLS.addWidget(self.cbIWLS05, 10, 0)
        self.cbIWLS05BDP = QtGui.QCheckBox('Collect BeforeDP data', self)
        self.cbIWLS05BDP.toggle()
        layoutIWLS.addWidget(self.cbIWLS05BDP, 10, 1)
        self.cbIWLS05isDP = QtGui.QCheckBox('Perform DP', self)
        self.cbIWLS05isDP.toggle()
        layoutIWLS.addWidget(self.cbIWLS05isDP, 10, 2)
        
        self.gridGroupBoxISPD.setLayout(layoutISPD)
        self.gridGroupBoxIWLS.setLayout(layoutIWLS)

        btnMD = QtGui.QPushButton('Run', self)
        btnMD.setFocus()

        grid = QtGui.QGridLayout()

        grid.addWidget(lblRev, 1, 0)
        grid.addWidget(self.teditRev, 1, 1)
        grid.addWidget(self.cbCheckout, 7, 0)
        grid.addWidget(self.cbRebuild, 8, 0)
        grid.addWidget(self.gridGroupBoxISPD, 9, 0)
        grid.addWidget(self.gridGroupBoxIWLS, 10, 0)
        grid.addWidget(btnMD, 12, 1)

        self.setLayout(grid)

        self.connect(btnMD, QtCore.SIGNAL('clicked()'), self.btnMDClicked)
        self.connect(self, QtCore.SIGNAL('closeEmitApp()'), QtCore.SLOT('close()') )

    def Build(self):
        self.setWindowTitle('Building solution...')
        args = [MSBuild, '.\itlPlace\make\itlPlace.sln', '/t:' + 'Rebuild', '/p:Configuration=Release']
        subprocess.Popen(subprocess.list2cmdline(args)).communicate()

    def DeleteSources(self):
        print('Deleting previous version of itlPlace...')
        if os.path.exists('.\itlPlace'):
            RemoveDir('.\itlPlace')
        print('Done.')
                
    def CheckOut(self):
        rev = str(self.teditRev.text())
        if rev == '':
            self.setWindowTitle('Checking out HEAD revision')
            subprocess.call('svn co ' + repoPath + '/itlPlace/ .\itlPlace')
        else :
            self.setWindowTitle('Checking out revision ' + rev)
            subprocess.call('svn co -r ' + rev + ' ' + repoPath + '/itlPlace/ .\itlPlace')

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
        print(smtpserver)
        subject = "Experiments results on " + setName + " with " + cfgName

        RECIPIENTS = ['itlab.vlsi@www.software.unn.ru']
        #RECIPIENTS = ['zhivoderov.a@gmail.com']
        SENDER = 'VLSIMailerDaemon@gmail.com'

        text = cfgComment + '\n\nThis is automatically generated mail. Please do not reply.'
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
    
    def RunTestsOnCfgList(self, setName):
        cfgNamesList, cfgCommentsList, filesListInGroups = self.OpenFilesList(binDir + setName + 'cfg.list')
        #print(str(filesListInGroups))
        isDP = (self.cbIWLS05isDP.checkState() == 2) if (setName == iwls05) else (self.cbISPD04isDP.checkState() == 2)
        isBeforeDP = (self.cbIWLS05BDP.checkState() == 2) if (setName == iwls05) else (self.cbIWLS05BDP.checkState() == 2)
        
        # Run all tests
        for idx in range(0, len(cfgNamesList)):
            outFileName = self.RunSet(setName, cfgNamesList[idx], cfgCommentsList[idx], isDP, isBeforeDP)
        
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
            text += '\n\nThis is automatically generated mail. Please do not reply.'
            self.PrepareAndSendMail(str(attachmentFiles), subject, text, attachmentFiles)    
        
    def RunSet(self, setName, cfgName = '', cfgComment = '', isDP = True, isBeforeDP = True):
        testSet, comments, fake = self.OpenFilesList(binDir + setName + ".list")
        defaultCfgName = setName + ".cfg"
        if cfgName == "":
            cfgName = defaultCfgName
        isTimingPresent = False
        pythonOutput = self.GetPythonOutput(setName, cfgName)
        print('Config name = %s' % cfgName)
        print("Performing tests on the following set of benchmarks: " + ", ".join(testSet))

        po = open(pythonOutput, 'w')
        if setName == ispd04:
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
            logFileName = binDir + setName + "\\" + benchmark + ".log"
            fPlacerOutput = open(logFileName, 'w');

            params = [binDir + "itlPlaceRelease.exe", cfgName,
                      "--params.def=.\\" + setName + "\\" + benchmark + ".def"]

            if setName == ispd04:
                params.append("--params.lef=.\\" + setName + "\\" + benchmark + ".lef")
            
            if setName == iwls05:
                params.append("--DesignFlow.Timing=true")
             
            if not isDP:
                params.append("--DesignFlow.DetailedPlacement=false")

            subprocess.call(params, stdout = fPlacerOutput, cwd = binDir)
            fPlacerOutput.close()
            print(benchmark + ' is done...')
            ParseLog(logFileName, benchmark, pythonOutput, setName == iwls05, isDP, isBeforeDP)
        
        return pythonOutput

    # the main function
    def btnMDClicked(self):
        print('\n')
        print('######################################################')
        print('####### S T A R T ####################################')
        print('######################################################')
        print('\n')
        if self.cbCheckout.checkState() == 2:
            self.DeleteSources()
            self.CheckOut()
        if self.cbRebuild.checkState() == 2:
            self.Build()
        if self.cbISPD04.checkState() == 2:
            subprocess.call([binDir + "rar.exe", "x", "-u", "ISPD04.rar"], cwd = binDir)
            self.RunTestsOnCfgList(ispd04)
        if self.cbIWLS05.checkState() == 2:
            self.RunTestsOnCfgList(iwls05)

        self.setWindowTitle(windowTitle)
        print('\n')
        print('######################################################')
        print('####### F I N I S H ##################################')
        print('######################################################')
        print('\n')

app = QtGui.QApplication(sys.argv)
db = DistributionBuilder()
db.show()
#db.btnMDClicked()
app.exec_()
