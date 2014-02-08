from Experiment_LR import *

import Experiment_Buffering
from Experiment_Buffering import *


def LRvsBuffering():
    benchmarkList = 'IWLS05_GP.list'

    exp_LR = Experiment_LR('IWLS05 LR experiment', 'LR.cfg', benchmarkList)
    exp_Buffering = Experiment_Buffering('IWLS05 buffering experiment (GBL)', 'kirill_buffering.cfg', benchmarkList)

    testRunner = TestRunner()
    testRunner.Append(exp_LR)
    testRunner.Append(exp_Buffering)
    testRunner.Run()


LRvsBuffering()