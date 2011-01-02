import BaseExperiment
from BaseExperiment import *

class ResultsStorage:
    experimentResults = {}

    def __init__(self):
        pass

    def AddExperimentResult(self, experiment, result):
        if (experimentResults.has_key(experiment)):
            print("Error: the result for experiment %s is already stored" % (experiment.name))

    def GetResults(self):
        pass