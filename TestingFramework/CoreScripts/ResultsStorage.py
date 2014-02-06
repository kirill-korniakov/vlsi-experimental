from Logger import Logger


class ResultsStorage:
    logger = None
    experimentResults = {}  #experimentName: experimentResult

    def __init__(self):
        self.logger = Logger()

    def AddExperimentResult(self, experimentName, result):
        if experimentName in list(self.experimentResults.keys()):
            self.logger.Log("Error: the result for experiment %s is already stored" % experimentName)

        self.experimentResults[experimentName] = result

    def __str__(self):
        resultStr = ""

        for experimentName, experimentResult in self.experimentResults.iteritems():
            resultStr += ("%s:\n%s\n" % (experimentName, experimentResult.__str__()))

        return resultStr

    def LogResults(self):
        self.logger.Log("\nResults:\n%s" % self.__str__())

    def SendResults(self, emailer):
        attachmentFiles = []

        for experimentResult in self.experimentResults.itervalues():
            resultFile = experimentResult.resultFile

            if resultFile != "":
                attachmentFiles.append(resultFile)

        emailer.PrepareAndSendMailIfRequired(self.__str__(), attachmentFiles)
