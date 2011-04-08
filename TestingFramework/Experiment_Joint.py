import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_Joint(BaseExperiment):
    def __init__(self):
        metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
        stages  = ['INIT', 'GP', 'NBUF', 'LEGB']

        BaseExperiment.__init__(self, 'Joint Algorithm', 'JointPlacementBuffering.cfg', 'IWLS_GP_r1511_joint.list', metrics, stages)

def TestRun():
    plotterFlag = "--plotter.enabled=false"
    bufferingType = "--New_Buffering.TypeNetListBuffering=1"

    experimentWithoutBuffering = Experiment_Joint()
    experimentWithoutBuffering.name = "GP + Buffering"
    experimentWithoutBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=false", bufferingType, plotterFlag]
    
    experimentWithBuffering = Experiment_Joint()
    experimentWithBuffering.name = "Joint GP + Buffering"
    experimentWithBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=true", bufferingType, plotterFlag]
    
    #experimentWithBuffering = Experiment_Joint()
    #experimentWithBuffering.name = "GP + Path-Based Buffering"
    #experimentWithBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=false", plotterFlag, "--New_Buffering.TypeNetListBuffering=1"]

    testRunner = TestRunner()
    testRunner.AddExperimentToGroup(experimentWithoutBuffering)
    testRunner.AddExperimentToGroup(experimentWithBuffering)
    testRunner.Run()

if (__name__ == "__main__"):
    TestRun()