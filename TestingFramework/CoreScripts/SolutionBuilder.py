import subprocess
import sys
import os
import datetime
from datetime import date
import time

from Emailer import Emailer
from Logger import Logger


class SolutionBuilder:
    emailer = None

    def __init__(self, emailer):
        self.emailer = emailer

    def BuildSln(self, generalParameters, tools, mode="Rebuild"):
        logger = Logger()
        logger.CoolLog("Building solution...")

        slnPath = generalParameters.slnPath
        res = 0
        #args    = [tools.MSBuild, slnPath, "/t:" + mode, "/p:Configuration=Release"]
        args = "%s %s /t:%s /p:Configuration=Release" % (tools.MSBuild, slnPath, mode)
        logger.Log("Building solution using command: %s" % (args))

        try:
            res = subprocess.call(args, shell=True)

        except WindowsError:
            error = ("Error: can not call %s" % (Tools.MSBuild))
            ReportErrorAndExit(error, logger, self.emailer)

        except Exception:
            import traceback

            error = "Error: %s" % (traceback.format_exc())
            ReportErrorAndExit(error, logger, self.emailer)

        if (res != 0):
            error = "Build failed!"
            print(error)
            buildLog = generalParameters.buildLog

            if (os.path.exists(buildLog)):
                self.emailer.SendMessageAndExit(error, [buildLog])

            else:
                self.emailer.SendMessageAndExit(error)


def test():
    from ConfigParser import ConfigParser
    from ParametersParsing import EmailerParameters, GeneralParameters, Tools

    configFile = "Parameters.conf"
    parentDir = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
    configFile = os.path.join(parentDir, configFile)
    cfgParser = ConfigParser()
    cfgParser.read(configFile)

    tools = Tools(cfgParser)
    emailerParameters = EmailerParameters(cfgParser)
    generalParameters = GeneralParameters(cfgParser)
    solutionBuilder = SolutionBuilder(Emailer(emailerParameters))

    solutionBuilder.BuildSln(generalParameters, tools)


if (__name__ == "__main__"):
    test()
