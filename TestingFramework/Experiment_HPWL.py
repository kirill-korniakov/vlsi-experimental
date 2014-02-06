from CoreScripts.TestRunner import TestRunner
from CoreScripts.CoreFunctions import WriteStringToFile
from CoreScripts.BaseExperiment import BaseExperiment


class Experiment_HPWL(BaseExperiment):
    def __init__(self):
        BaseExperiment.__init__(self, "IWLS05 HPWL experiment", "hpwl_iwls05.cfg", "IWLS05.list", \
                                ["HPWL"], ["DP"])

    def CreateEmptyTable(self, reportTable):
        cols = ["Benchmark", "HPWL"]
        WriteStringToFile(cols, reportTable)


def test():
    experiment = Experiment_HPWL()

    testRunner = TestRunner()
    testRunner.Append(experiment)
    testRunner.Run()


if (__name__ == "__main__"):
    test()
