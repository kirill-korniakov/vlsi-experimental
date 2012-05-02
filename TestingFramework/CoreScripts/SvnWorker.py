import subprocess
import sys
import os

from Emailer import Emailer
from CoreFunctions import Logger, ReportErrorAndExit, RemoveDir

class SvnWorker:
    logger            = None
    emailer           = None
    repoParameters    = None
    generalParameters = None

    def __init__(self, emailer, generalParameters, repoParameters):
        self.emailer           = emailer
        self.repoParameters    = repoParameters
        self.generalParameters = generalParameters

    def DeleteSources(self):
        dir = self.generalParameters.checkoutPath
        self.logger.Log("Deleting previous version of itlPlace...")

        if os.path.exists(dir):
            RemoveDir(dir)
        self.logger.Log("Done")

    def doCheckOut(self, command):
        retcode = 1

        for i in range(10):
            try:
                retcode = subprocess.call(command, shell = True)

            except Exception:
                import traceback
                error = "Error: can not call svn.exe: %s" % (traceback.format_exc())
                ReportErrorAndExit(error, self.logger, self.emailer)

            if retcode == 0:
                break

        if retcode != 0:
            error = "svn error: checkout failed!"
            ReportErrorAndExit(error, self.logger, self.emailer)

        return

    def FormCommand(self, rev = ""):
        if (rev != ""):
            self.logger.Log("Checking out revision %s" % (rev))
            rev = " -r %s" % (rev)

        else:
            self.logger.Log("Checking out HEAD revision")

        url = self.repoParameters.srcRepoPath
        to  = self.generalParameters.checkoutPath

        #for testing...
        #url = 'http://svn.software.unn.ru/VLSI/CODE/trunk/TestingFramework'
        #to = '../../qqq'

        #command = "svn co --username belyakov --password 'njz%s %s %s"\
        command = r'"C:\Program Files (x86)\CollabNet\Subversion Client\svn.exe" co%s %s %s'\
                    % (rev, url, to)
        return command

    def CheckOut(self):
        self.logger = Logger()
        self.logger.CoolLog("Delete sources and Checkout")
        self.DeleteSources()
        command = self.FormCommand()
        self.doCheckOut(command)

if (__name__ == "__main__"):
  from ConfigParser import ConfigParser
  from ParametersParsing import EmailerParameters, GeneralParameters, RepoParameters

  configFile = "Parameters.conf"
  parentDir  = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
  configFile = os.path.join(parentDir, configFile)
  cfgParser  = ConfigParser()
  cfgParser.read(configFile)

  emailerParameters = EmailerParameters(cfgParser)
  repoParameters    = RepoParameters(cfgParser)
  generalParameters = GeneralParameters(cfgParser)

  svnWorker = SvnWorker(Emailer(emailerParameters), generalParameters, repoParameters)
  svnWorker.CheckOut()
