import subprocess
import sys
import os

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

class ReportCreator:
    def GetLogFolderName(self, cfgName):
        return ReportParameters.logFolder + os.path.basename(cfgName)

    def GetReportTableName(self, cfgName):
        (path, cfgFileName) = os.path.split(cfgName)
        return self.GetLogFolderName(cfgName) + '/' + "ReportTable_{0}.csv".format(cfgFileName)

    def CreateLogFolder(self, cfgName):
        logFolder = self.GetLogFolderName(cfgName)
        if os.path.exists(logFolder):
            newFolderName = logFolder + "_backup_from_" + GetTimeStamp()
            os.rename(logFolder, newFolderName)
        if os.path.exists(ReportParameters.logFolder) != True:
            os.mkdir(ReportParameters.logFolder)
        os.mkdir(logFolder)

        return logFolder