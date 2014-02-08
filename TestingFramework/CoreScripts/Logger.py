import time
import os

from CoreFunctions import GetTimeStamp
from CfgParserFactory import CfgParserFactory


class Logger:
    def __init__(self):
        pass

    cfgParser = CfgParserFactory.createCfgParser()

    selfLogFolder = cfgParser.get("ReportParameters", "selfLogFolder")
    if not os.path.exists(selfLogFolder):  #TODO: use ReportCreator.CreateLogFolder
        os.mkdir(selfLogFolder)

    startTime = time.time()
    logFileName = selfLogFolder + r"TF_%s.log" % (GetTimeStamp())

    def LogWorkTime(self):
        runTime = time.time() - self.startTime
        self.Log("RUNTIME %2.2i:%2.2i" % (int(runTime / 60), int(runTime % 60)))

    def CoolLog(self, message):
        self.Log("\n")
        self.Log("######################################################")
        self.Log("####### %s" % message)
        self.Log("######################################################")
        self.LogWorkTime()
        self.Log("\n")

    def Log(self, message):
        message = ">> " + message

        print(message)
        log = open(self.logFileName, 'a')
        log.write("%s\n" % message)
        log.close()

    def LogStartMessage(self):
        self.CoolLog("Started on %s" % (GetTimeStamp()))


if __name__ == "__main__":
    pass
