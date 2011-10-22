from CoreScripts.TestRunner import TestRunner
from CoreScripts.CoreFunctions import END_OF_COLUMN, WriteStringToFile
from CoreScripts.BaseExperiment import BaseExperiment

class Experiment_LR(BaseExperiment):
    def __init__(self):
        _metrics = ["HPWL", "TWL", "TNS", "WNS"]
        _stages  = ["INIT", "LEG"]
        BaseExperiment.__init__(self, "LR experiment (IWLS05GP)", "LR.cfg", r"IWLS_GP_r1511\IWLS_GP.list", _metrics, _stages)

        self.doParsePQAT = True

    def CreateEmptyTable(self, reportTable):
        cols = ["Benchmark", END_OF_COLUMN, "initHPWL", END_OF_COLUMN, "lrHPWL", END_OF_COLUMN, "dpHPWL", END_OF_COLUMN,\
        "initTWL", END_OF_COLUMN, "lrTWL", END_OF_COLUMN, "dpTWL", END_OF_COLUMN,\
        "initTNS", END_OF_COLUMN, "lrTNS", END_OF_COLUMN, "dpTNS", END_OF_COLUMN,\
        "initWNS", END_OF_COLUMN, "lrWNS", END_OF_COLUMN, "dpWNS", END_OF_COLUMN, END_OF_COLUMN,\
        "lrHPWL%", END_OF_COLUMN, "dpHPWL%", END_OF_COLUMN, "lrTWL%", END_OF_COLUMN, "dpTWL%", END_OF_COLUMN,\
        "lrTNS%", END_OF_COLUMN, "dpTNS%", END_OF_COLUMN, "lrWNS%", END_OF_COLUMN, "dpWNS%"]

        WriteStringToFile(cols, reportTable)

    def AddStringToTable(self, values, benchmark, reportTable):
        cols = [benchmark, END_OF_COLUMN]

        #print absolute values
        for col in range(len(self.metrics)):
            for row in range(len(self.stages)):
                cols.extend([str(values[row][col]), END_OF_COLUMN])

        cols.append(END_OF_COLUMN)

        #print percents
        for col in range(len(self.metrics)):
            for row in range(1, len(self.stages)):
                if values[0][col] != 0.0:
                    percent = 100.0 * (values[row][col] / values[0][col] - 1.0)
                else:
                    percent = -1.0

                printStr = "%.2f;" % percent
                cols.append(printStr)

        WriteStringToFile(cols, reportTable)

def TestRun():
    e = Experiment_LR()
    e.SetBenchmarksList("IWLS_GP_r1511/IWLS_GP_fast.list")

    testRunner = TestRunner()
    testRunner.Append(e)
    testRunner.Run()

if (__name__ == "__main__"):
    TestRun()
