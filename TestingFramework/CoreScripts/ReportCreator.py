import os

from CoreFunctions import GetTimeStamp
from CoreScripts.CfgParserFactory import CfgParserFactory


class ReportCreator:
    logFolder = ""
    cfgName = ""
    reportParameters = None

    def __init__(self, experimentName, cfgName, reportParameters):
        self.cfgName = cfgName
        self.reportParameters = reportParameters
        self.currentLogs = os.path.join(CfgParserFactory.get_root_dir(), self.reportParameters.logFolder)
        self.logFolder = r"%s%s_%s_%s" % (self.currentLogs,
                                          experimentName,
                                          os.path.basename(cfgName), GetTimeStamp())

        self.logFolder = self.logFolder.replace(" ", "_")

    def GetReportTableName(self):
        (path, cfgFileName) = os.path.split(self.cfgName)
        return r"%s/ReportTable_%s.csv" % (self.logFolder, cfgFileName)

    def CreateLogFolder(self):
        if not os.path.exists(self.currentLogs):
            os.mkdir(self.currentLogs)

        if os.path.exists(self.logFolder):
            newFolderName = "%s_backup_from_%s_%s" % (self.logFolder, GetTimeStamp())
            os.rename(self.logFolder, newFolderName)

        os.mkdir(self.logFolder)
        return self.logFolder