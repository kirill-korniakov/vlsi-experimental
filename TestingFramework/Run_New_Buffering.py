import Experiment_New_Buffering
from Experiment_New_Buffering import *

def New_Buffering():
    benchmarkList = 'new_buf_IWLS05_GP.list'

    exp_New_Buffering = Experiment_New_Buffering('IWLS05 New_Buffering experiment ', 'New_Buffering.cfg', benchmarkList)

    testRunner = TestRunner()
    testRunner.parameters.experiments.append(exp_New_Buffering)
    testRunner.Run()

New_Buffering()
