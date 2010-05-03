import subprocess
import sys
import os

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

class ReportCreator:
    logFolder = ''
    cfgName = ''

    def __init__(self, cfgName):
        self.cfgName = cfgName
        self.logFolder = ReportParameters.logFolder + os.path.basename(cfgName) + '_' + GetTimeStamp()

    def GetReportTableName(self):
        (path, cfgFileName) = os.path.split(self.cfgName)
        return self.logFolder + '/' + "ReportTable_{0}.csv".format(cfgFileName)

    def CreateLogFolder(self):
        if os.path.exists(self.logFolder):
            newFolderName = self.logFolder + "_backup_from_" + GetTimeStamp()
            os.rename(self.logFolder, newFolderName)
        if os.path.exists(ReportParameters.logFolder) != True:
            os.mkdir(ReportParameters.logFolder)
        os.mkdir(self.logFolder)

        return self.logFolder