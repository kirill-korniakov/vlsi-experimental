import subprocess
import sys
import os

import datetime
from datetime import date
import time

from Emailer import *
from SvnWorker import *

from CoreFunctions import Logger

import Parameters
from Parameters import *

class SolutionBuilder:
    emailer = None

    def __init__(self, emailer):
        self.emailer = emailer

    def BuildSln(self, slnPath = GeneralParameters.slnPath, mode = "Rebuild"):
        logger = Logger()
        logger.CoolLog("Building solution...")
        res  = 0
        args = [Tools.MSBuild, slnPath, "/t:" + mode, "/p:Configuration=Release"]

        try:
            res = subprocess.call(args)

        except WindowsError:
            error = ("Error: can not call %s" % (Tools.MSBuild))
            ReportErrorAndExit(error, logger, self.emailer)

        if (res != 0):
            error = "Build failed!"
            print(error)
            buildLog = GeneralParameters.buildLog
            self.emailer.SendMessageAndExit(error, [buildLog])
