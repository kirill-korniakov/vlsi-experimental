from CoreScripts.TestRunner import TestRunner
from CoreScripts.BaseExperiment import BaseExperiment


class Experiment_Joint(BaseExperiment):
    def __init__(self):
        metrics = ["Time", "Cells", "HPWL", "TWL", "TNS", "WNS"]
        stages = ["INIT", "GP", "NBUF", "LEGB"]

        BaseExperiment.__init__(self, "Joint Algorithm", "JointPlacementBuffering.cfg",
                                "IWLS_GP_r1511_joint.list", metrics, stages)


def TestRun():
    plotterFlag = "--plotter.enabled=false"
    bufferingTypeSimple = "--LSE.GlobalPlacement.New_Buffering.TypeNetListBuffering=0"
    bufferingTypePath = "--LSE.GlobalPlacement.New_Buffering.TypeNetListBuffering=1"

    simpleBuffering = Experiment_Joint()
    simpleBuffering.name = "Simple Buffering"
    simpleBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=false", bufferingTypeSimple,
                               plotterFlag]

    pathBuffering = Experiment_Joint()
    pathBuffering.name = "Path Buffering"
    pathBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=false", bufferingTypePath, plotterFlag]

    jointSimpleBuffering = Experiment_Joint()
    jointSimpleBuffering.name = "Joint Simple Buffering"
    jointSimpleBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=true", bufferingTypeSimple,
                                    plotterFlag]

    jointPathBuffering = Experiment_Joint()
    jointPathBuffering.name = "Joint Path Buffering"
    jointPathBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=true", bufferingTypePath,
                                  plotterFlag]

    testRunner = TestRunner()
    testRunner.AddExperimentToGroup(simpleBuffering)
    testRunner.AddExperimentToGroup(pathBuffering)
    testRunner.AddExperimentToGroup(jointSimpleBuffering)
    testRunner.AddExperimentToGroup(jointPathBuffering)
    testRunner.Run()


if __name__ == "__main__":
    TestRun()