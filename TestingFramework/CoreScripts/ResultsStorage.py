import BaseExperiment
from BaseExperiment import *
from CoreFunctions import Logger

import Emailer
from Emailer import *

class ResultsStorage:
    logger = None
    experimentResults = {}

    def __init__(self):
        self.logger = Logger()

    def AddExperimentResult(self, experiment, result):
        if (experiment in self.experimentResults.keys()):
            self.logger.Log("Error: the result for experiment %s is already stored" % experiment.name)

        self.experimentResults[experiment] = result

    def AsString(self):
        result = ""
        for experiment in list(self.experimentResults.keys()):
            result += ("%s:\n%s\n" % (experiment.name, self.experimentResults[experiment].AsString()))
        return result

    def LogResults(self):
        self.logger.Log("\nResults:\n%s" % self.AsString())

    def SendResults(self, emailer):
        attachmentFiles = []
        for experiment in list(self.experimentResults.keys()):
            resultFile = self.experimentResults[experiment].resultFile
            if (resultFile != ""):
                attachmentFiles.append(resultFile)

        emailer.PrepareAndSendMailIfRequired(self.AsString(), attachmentFiles)
