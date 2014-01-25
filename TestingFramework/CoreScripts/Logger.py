import time
from CoreFunctions import GetTimeStamp

class Logger:
    startTime   = time.time()
    logFileName = r"./Logs/%s_TF.log" % (GetTimeStamp()) # TODO: use ReportCreator.CreateLogFolder

    def LogWorkTime(self):
        runTime = time.time() - self.startTime
        self.Log("RUNTIME %2.2i:%2.2i" % (int(runTime / 60), int(runTime % 60)))

    def CoolLog(self, message):
        self.Log("\n")
        self.Log("######################################################")
        self.Log("####### %s" % (message))
        self.Log("######################################################")
        self.LogWorkTime()
        self.Log("\n")

    def Log(self, message):
        message = ">> " + message

        print(message)
        log = open(self.logFileName, 'a')
        log.write(("%s\n") % (message))
        log.close()

    def LogStartMessage(self):
        self.CoolLog("Started on %s" % (GetTimeStamp()))

if (__name__ == "__main__"):
  pass
