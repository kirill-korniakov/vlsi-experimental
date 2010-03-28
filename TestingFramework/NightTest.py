import HPWLExperiment
from HPWLExperiment import *

def NightTest():
    e = HPWLExperiment()

    testRunner = TestRunner()
    testRunner.parameters.experiments.append(e)
    testRunner.parameters.experiments.append(e)
    testRunner.Run()

NightTest()