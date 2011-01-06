import BaseExperiment
from BaseExperiment import *

class ResultsStorage:
    experimentResults = {}

    def __init__(self):
        pass

    def AddExperimentResult(self, experiment, result):
        if (self.experimentResults.has_key(experiment)):
            print("Error: the result for experiment %s is already stored" % (experiment.name))

        self.experimentResults[experiment] = result

    def GetResults(self):
        for experiment in list(self.experimentResults.keys()):
            print("%s:" % (experiment.name))
            self.experimentResults[experiment].Print()
            print("\n")