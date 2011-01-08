import subprocess
import sys
import os

import Parameters
from Parameters import *

import CoreFunctions
from CoreFunctions import *

from Emailer import *
from SvnWorker import *

class SvnWorker:
    emailer = None

    def __init__(self, emailer):
        self.emailer = emailer

    def DeleteSources(self):
        dir = GeneralParameters.checkoutPath
        print("Deleting previous version of itlPlace...")

        if os.path.exists(dir):
            RemoveDir(dir)
        print("Done")

    def doCheckOut(self, command):
        retcode = 1

        for i in range(10):
            try:
                retcode = subprocess.call(command)

            except WindowsError:
                error = "Error: can not call svn.exe"
                ReportErrorAndExit(error, self.emailer)

            if retcode == 0:
                break

        if retcode != 0:
            error = "svn error: checkout failed!"
            ReportErrorAndExit(error, self.emailer)

        return

    def FormCommand(self, rev = ""):
        if (rev != ""):
            print("Checking out revision %s" % (rev))
            rev = " -r %s" % (rev)

        else:
            print("Checking out HEAD revision")

        url     = RepoParameters.srcRepoPath
        to      = GeneralParameters.checkoutPath

        #for testing...
        #url = 'http://svn.software.unn.ru/VLSI/CODE/trunk/TestingFramework'
        #to = '../../qqq'

        #command = "C:\\Program Files (x86)\\CollabNet\\Subversion Client\\svn.exe co%s %s %s"\
        command = "svn co --username belyakov --password 'njz%s %s %s"\
                    % (rev, url, to)
        return command

    def CheckOut(self):
        cp = CoolPrinter()
        cp.CoolPrint("Delete sources and Checkout")
        self.DeleteSources()
        command = self.FormCommand()
        self.doCheckOut(command)

if __name__ == '__main__':
    svnWorker = SvnWorker(Emailer())
    svnWorker.CheckOut()
