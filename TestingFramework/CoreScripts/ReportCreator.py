import sys
import os

import CoreFunctions
from CoreFunctions import *

import Parameters
from Parameters import *

class ReportCreator:
    logFolder = ''
    cfgName   = ''

    def __init__(self, experimentName, cfgName):
        self.cfgName   = cfgName
        self.logFolder = "%s%s_%s_%s" % (ReportParameters.logFolder, experimentName.replace(" ", "_"),\
                         os.path.basename(cfgName), GetTimeStamp())

    def GetReportTableName(self):
        (path, cfgFileName) = os.path.split(self.cfgName)
        return ("%s//ReportTable_%s.csv" % (self.logFolder, cfgFileName))

    def CreateLogFolder(self):
        if os.path.exists(self.logFolder):
            newFolderName = "%s_backup_from_%s" % (self.logFolder, GetTimeStamp())
            os.rename(self.logFolder, newFolderName)

        if os.path.exists(ReportParameters.logFolder) != True:
            os.mkdir(ReportParameters.logFolder)

        os.mkdir(self.logFolder)
        return self.logFolder