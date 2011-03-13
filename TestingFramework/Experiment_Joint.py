import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_Joint(BaseExperiment):
	def __init__(self):
		metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
		stages  = ['INIT', 'GP', 'BUF', 'LEGB']

		BaseExperiment.__init__(self, 'Joint Algorithm', 'JointPlacementBuffering.cfg', 'IWLS05.list', metrics, stages)

def TestRun():
	experimentWithoutBuffering = Experiment_Joint()
	experimentWithoutBuffering.name = "JointWithoutBuffering"
	experimentWithoutBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=false"]

	experimentWithBuffering = Experiment_Joint()
	experimentWithBuffering.name = "JointWithBuffering"
	experimentWithBuffering.cmdArgs = ["--LSE.GlobalPlacement.UseBuffering=true"]

	testRunner = TestRunner()
	testRunner.AddExperimentToGroup(experimentWithoutBuffering)
	testRunner.AddExperimentToGroup(experimentWithBuffering)
	testRunner.Run()

if (__name__ == "__main__"):
	TestRun()