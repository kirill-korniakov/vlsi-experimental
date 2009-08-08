import subprocess
import sys
from PyQt4 import QtGui
from PyQt4 import QtCore
import datetime
from datetime import date
import time
from itlPlaceEmail import send_mail

repoPath        = "http://www.software.unn.ru/VLSI/CODE/branch/"
#testSet         = ""
#cfgName         = ""
#pythonOutput    = ""
ispd04 = "ISPD04"
iwls05 = "IWLS05"
binDir = ".\\itlPlace\\bin\\"

windowTitle = 'Automatic Building Script iltPlace'

def parseLog(logName, benchmark, pythonOutput, isTimingUsed):
    fh = open(logName, "r")
    po = open(pythonOutput, 'a')
    po.write('\n' + benchmark + '\t')    # set the benchmark name
    isTiming = False
    workTime = ""
    for line in fh.readlines():
        idx = line.find('HPWL after  legalization: ')
        if idx != -1:
            HPWL = line[idx + len('HPWL after  legalization: '):-1]
            po.write(HPWL.replace('.', ',') + '\t')
            
            # get time
            workTime = line[1:11].replace('.', ',') + '\t'
            continue

        if (isTimingUsed):
            idx = line.find('STA after legalization:\n')
            if idx != -1:
                isTiming = True
                continue
            idx = line.find('  TNS: ')
            if (isTiming) and (idx != -1):
                TNS = line[idx + len('  TNS: '):-1]
                po.write(TNS.replace('.', ',') + '\t')
                continue
            idx = line.find('  WNS: ')
            if (isTiming) and (idx != -1):
                WNS = line[idx + len('  WNS: '):-1]
                po.write(WNS.replace('.', ',') + '\t')
                po.write(workTime)
                isTiming = False


                
        idx = line.find('HPWL after  detailed placement: ')
        if idx != -1:
            HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
            po.write(HPWL.replace('.', ',') + '\t')

            # get time
            workTime = line[1:11].replace('.', ',') + '\t'
            continue

        if (isTimingUsed):
            idx = line.find('STA after detailed placement:\n')
            if idx != -1:
                isTiming = True
                continue
            idx = line.find('  TNS: ')
            if (isTiming) and (idx != -1):
                TNS = line[idx + len('  TNS: '):-1]
                po.write(TNS.replace('.', ',') + '\t')
                continue
            idx = line.find('  WNS: ')
            if (isTiming) and (idx != -1):
                WNS = line[idx + len('  WNS: '):-1]
                po.write(WNS.replace('.', ',') + '\t')
                po.write(workTime + '\t')
                isTiming = False

    po.write(workTime + '\t')
    po.close()
    fh.close()

class DistributionBuilder(QtGui.QWidget):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.setGeometry(300, 300, 300, 100)
        self.setWindowTitle(windowTitle)

        #field for PeopleCounter version
        #lblVersion = QtGui.QLabel('PeopleCounter version', self)
        #self.teditVersion = QtGui.QLineEdit(version, self)

        #field for SVN revision
        lblRev = QtGui.QLabel('SVN Revision (keep empty for HEAD)', self)
        self.teditRev = QtGui.QLineEdit('', self)

        #field for bugzilla issue number
        #lblIssue = QtGui.QLabel('Bugzilla issue (keep empty for empty)', self)
        #self.teditIssue = QtGui.QLineEdit('', self)

        #lbl = QtGui.QLabel('Definitions.h settings:', self)

        #self.cbNoEwc = QtGui.QCheckBox('define NO_EWCLID_FILTER', self)
        #self.cbNoEwc.toggle();

        #self.cbUK = QtGui.QCheckBox('define USBKEY', self)
        #self.cbUK.toggle();

        #lblTD = QtGui.QLabel('define TRIAL_DAYS', self)

        #lblTC = QtGui.QLabel('define TRIAL_COUNT', self)

        self.cbCheckout = QtGui.QCheckBox('Checkout', self)
        self.cbCheckout.toggle();

        self.cbRebuild = QtGui.QCheckBox('Build', self)
        self.cbRebuild.toggle();

        self.cbISPD04 = QtGui.QCheckBox(ispd04, self)
        self.cbISPD04.toggle();

        self.cbIWLS05 = QtGui.QCheckBox(iwls05, self)
        self.cbIWLS05.toggle();

        #self.cbMakeSetupOffice = QtGui.QCheckBox('Make SetupOffice.msi (and pack zip)', self)
        #self.cbMakeSetupOffice.toggle();

        btnMD = QtGui.QPushButton('Run', self)
        btnMD.setFocus()

        #self.teditTD = QtGui.QLineEdit(nTrialDays, self)
        #self.teditTD.resize(50, 25)

        #self.teditTC = QtGui.QLineEdit(nTrialCount, self)
        #self.teditTC.resize(50, 25)

        grid = QtGui.QGridLayout()

        #grid.addWidget(lblVersion, 0, 0)
        #grid.addWidget(self.teditVersion, 0, 1)
        grid.addWidget(lblRev, 1, 0)
        grid.addWidget(self.teditRev, 1, 1)
        #grid.addWidget(lblIssue, 2, 0)
        #grid.addWidget(self.teditIssue, 2, 1)
        #grid.addWidget(lbl, 3, 0)
        #grid.addWidget(self.cbNoEwc, 4, 0)
        #grid.addWidget(self.cbUK, 4, 1)
        #grid.addWidget(lblTD, 5, 0)
        #grid.addWidget(self.teditTD, 5, 1)
        #grid.addWidget(lblTC, 6, 0)
        #grid.addWidget(self.teditTC, 6, 1)
        grid.addWidget(self.cbCheckout, 7, 0)
        grid.addWidget(self.cbRebuild, 8, 0)
        grid.addWidget(self.cbISPD04, 9, 0)
        grid.addWidget(self.cbIWLS05, 10, 0)
        #grid.addWidget(self.cbMakeSetupOffice, 11, 0)
        grid.addWidget(btnMD, 12, 1)

        self.setLayout(grid)

        self.connect(btnMD, QtCore.SIGNAL('clicked()'), self.btnMDClicked)
        self.connect(self, QtCore.SIGNAL('closeEmitApp()'), QtCore.SLOT('close()') )

    def Build(self):
        self.setWindowTitle('Building solution...')
        subprocess.call(["BuildSolution.bat", "Rebuild"])

    def CheckOut(self):
        rev = str(self.teditRev.text())
        if rev == '':
            self.setWindowTitle('Checking out HEAD revision')
            subprocess.call('svn co ' + repoPath + '/itlPlace/ .\itlPlace')
        else :
            self.setWindowTitle('Checking out revision ' + rev)
            subprocess.call('svn co -r ' + rev + ' ' + repoPath + '/itlPlace/ .\itlPlace')

    def OpenBenchmarkList(self, listName):
        testSet  = (open(listName).read()).split('\n')
        #filter(isNotCommented, '#jjfjf')
        for x in testSet:
            if x.startswith('#'):
                testSet.remove(x)
        return testSet

    def RunSet(self, setName):
        testSet = self.OpenBenchmarkList(binDir + setName + ".list")
        cfgName = setName + ".cfg"
        isTimingPresent = False
        pythonOutput = "pythonOutput_" + setName.lower() + ".xls"
        print("Performing tests on the following set of benchmarks: " + ", ".join(testSet))

        po = open(pythonOutput, 'w')
        if setName == ispd04:
            po.write('\tHPWL\tTime\t')
        else:
            #po.write(9*'\tHPWL\tTNS\tWNS\tTime\t')
            #po.write('\tSOD\t\n')
            po.write('\tbefore DP\t\t\t\tafter DP\n')
            po.write(2*'\tHPWL\tTNS\tWNS\tTime')
        po.close()

        for benchmark in testSet:
            logFileName = binDir + setName + "\\" + benchmark + ".log"
            fPlacerOutput = open(logFileName, 'w');

            params = [binDir + "itlPlaceRelease.exe", cfgName,
                      "--benchmark.def=.\\" + setName + "\\" + benchmark + ".def"]

            if setName == ispd04:
                params.append("--benchmark.lef=.\\" + setName + "\\" + benchmark + ".lef")

            subprocess.call(params, stdout = fPlacerOutput, cwd = binDir)
            fPlacerOutput.close()
            print(benchmark + ' is done...')
            parseLog(logFileName, benchmark, pythonOutput, setName == iwls05)

        print("Sending mail with " + pythonOutput)
        smtpserver = 'smtp.gmail.com'
        smtpuser = 'VLSIMailerDaemon@gmail.com'  # for SMTP AUTH, set SMTP username here
        smtppass = '22JUL22:19:49'
        subject = "Experiments results on " + setName

        RECIPIENTS = ['itlab.vlsi@www.software.unn.ru']#['zhivoderov.a@gmail.com']
        SENDER = 'VLSIMailerDaemon@gmail.com'

        text = "This is automatically generated mail. Please do not reply."

        send_mail(
            SENDER,     # from
            RECIPIENTS, # to
            subject,    # subject
            text,       # text
            [pythonOutput],         # attachment files
            smtpserver, # SMTP server
            587,        # port
            smtpuser,   # login
            smtppass,   # password
            1)          # TTLS
        print('Success!')

    # the main function
    def btnMDClicked(self):
        print('\n')
        print('######################################################')
        print('####### S T A R T ####################################')
        print('######################################################')
        print('\n')
        if self.cbCheckout.checkState() == 2:
            self.CheckOut()
        if self.cbRebuild.checkState() == 2:
            self.Build()
        if self.cbISPD04.checkState() == 2:
            subprocess.call([binDir + "rar.exe",  "x", "-u", "ISPD04.rar"], cwd = binDir)
            self.RunSet(ispd04)
        if self.cbIWLS05.checkState() == 2:
            self.RunSet(iwls05)

        self.setWindowTitle(windowTitle)
        print('\n')
        print('######################################################')
        print('####### F I N I S H ##################################')
        print('######################################################')
        print('\n')

app = QtGui.QApplication(sys.argv)
db = DistributionBuilder()
db.show()
app.exec_()
