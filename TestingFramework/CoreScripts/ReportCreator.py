import sys
import os

from CoreFunctions import GetTimeStamp, CreateConfigParser
from ParametersParsing import ReportParameters

class ReportCreator:
  logFolder        = ""
  cfgName          = ""
  reportParameters = None

  def __init__(self, experimentName, cfgName, reportParameters):
    self.cfgName          = cfgName
    self.reportParameters = reportParameters

    self.logFolder = r"%s%s_%s_%s" % (self.reportParameters.logFolder, experimentName,\
                     os.path.basename(cfgName), GetTimeStamp())

    self.logFolder = self.logFolder.replace(" ", "_")

  def GetReportTableName(self):
    (path, cfgFileName) = os.path.split(self.cfgName)
    return (r"%s/ReportTable_%s.csv" % (self.logFolder, cfgFileName))

  def CreateLogFolder(self):
    if os.path.exists(self.logFolder):
      newFolderName = "%s_backup_from_%s" % (self.logFolder, GetTimeStamp())
      os.rename(self.logFolder, newFolderName)

    if os.path.exists(self.reportParameters.logFolder) != True:
      os.mkdir(self.reportParameters.logFolder)

    os.mkdir(self.logFolder)
    return self.logFolder