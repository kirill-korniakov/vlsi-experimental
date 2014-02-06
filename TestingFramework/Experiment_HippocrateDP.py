from CoreScripts.TestRunner import TestRunner
from CoreScripts.CoreFunctions import WriteStringToFile
from CoreScripts.BaseExperiment import BaseExperiment


class Experiment_HippocrateDP(BaseExperiment):
    def __init__(self):
        _metrics = ["HPWL", "TNS", "WNS"]
        _stages = ["LEG", "DP"]
        BaseExperiment.__init__(self, "HippocrateDP experiment", \
                                "HippocrateDP.cfg", "IWLS_GP_Hippocrate.list", _metrics, _stages)

    def CreateEmptyTable(self, reportTable):
        cols = ["", "GP + Leg", "", "", "", "GP + Leg + HippocrateDP"]
        WriteStringToFile(cols, reportTable)

        cols = ["Benchmark", "HPWL", "TNS", "WNS", "", "HPWL", "TNS", "WNS"]
        WriteStringToFile(cols, reportTable)


def test():
    e = Experiment_HippocrateDP()
    #e.stages = ["LEG", "DP"]
    #e.stages = []
    #e.cmdArgs = ["--plotter.enabled=true", "--plotter.createVideo=true"]
    testRunner = TestRunner()
    testRunner.Append(e)
    testRunner.Run()

    #e.MakeResultTable(r".\Reports\HippocrateDP_experiment_HippocrateDP.cfg_2011-01-01_14-12-44", "HippocrateDPTable.csv")


if (__name__ == "__main__"):
    test()
