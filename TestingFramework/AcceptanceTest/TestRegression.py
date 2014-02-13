import os
import sys
from CoreScripts import Logger
from Experiments.Experiment_HPWL import Experiment_HPWL

path = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(path)

import nose
from nose.tools import nottest

from AcceptanceTest.TestHelper import TestHelper
from Experiments.Experiment_HippocrateDP import Experiment_HippocrateDP
from Experiments.Experiment_New_Buffering import Experiment_New_Buffering


# TODO:
# "APlace weighting experiment": "IWLS05.list",
# "HPWL_ISPD": "ISPD04.list", -- crash
# "Weighting (SGNW) experiment": "IWLS05.list", -- crash
class TestRegression:
    test_helper = TestHelper()
    logger = Logger()

    @nottest
    def test_hippocrate(self):
        benchmark_list = "sanity/HippocrateDP.list"
        referenceLogFolder = "/HippocrateDP"
        experiment = Experiment_HippocrateDP()

        benchmarks = self.test_helper.expand_benchmark_list(benchmark_list)
        for benchmark in benchmarks:
            yield self.test_helper.run, self.logger, experiment, benchmark, referenceLogFolder

    def test_hpwl_placement(self):
        benchmark_list = "regression/HPWL_IWLS.list"
        referenceLogFolder = "/HPWL/IWLS-initial-mac-state"
        experiment = Experiment_HPWL(self.logger)

        benchmarks = self.test_helper.expand_benchmark_list(benchmark_list)
        for benchmark in benchmarks:
            yield self.test_helper.run, self.logger, experiment, benchmark, referenceLogFolder

    # def DISABLED_test_lr(self):
    #     benchmark_list = "IWLS_GP_r1511_fast.list"
    #     referenceLogFolder = "/LR"
    #     experiment = Experiment_LR()
    #     self.experimentRunner.run_experiment(experiment, benchmark_list, referenceLogFolder)


if __name__ == "__main__":
    nose.main()