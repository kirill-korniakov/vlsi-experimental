import os
import sys

from AcceptanceTest.ExperimentRunner import ExperimentRunner
from CoreScripts import ComparisonResult
from Experiments.Experiment_HippocrateDP import Experiment_HippocrateDP
from Experiments.Experiment_New_Buffering import Experiment_New_Buffering


path = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(path)

import nose

# TODO:
# "APlace weighting experiment": "IWLS05.list",
# "HPWL_ISPD": "ISPD04.list", -- crash
# "Weighting (SGNW) experiment": "IWLS05.list", -- crash
class TestSanity:
    experimentRunner = ExperimentRunner()

    def print_differences(self, experiment_results, benchmark):
        # print "Reference results"
        # print experiment_results.pfstRefTables
        # print "Current results"
        # print experiment_results.pfstTables
        ref_pfst = experiment_results.pfstRefTables.get(benchmark)
        curr_pfst = experiment_results.pfstTables.get(benchmark)
        for x in zip(ref_pfst, curr_pfst):
            print x[0]
            print x[1]
            print [round(a / b, 2) for a, b in zip(x[0], x[1])]
            print ""

    def run(self, experiment, benchmark, referenceLogFolder):
        experiment_results = self.experimentRunner.run_experiment(experiment, benchmark, referenceLogFolder)
        result = experiment_results.benchmarkResults.get(benchmark)
        if result != ComparisonResult.OK:
            self.print_differences(experiment_results, benchmark)
            assert result == ComparisonResult.OK

    # @nottest
    def test_hippocrate(self):
        benchmark_list = "sanity/HippocrateDP.list"
        referenceLogFolder = "/HippocrateDP"
        experiment = Experiment_HippocrateDP()

        benchmarks = self.experimentRunner.parse_benchmarks_list(benchmark_list)
        for benchmark in benchmarks:
            yield self.run, experiment, benchmark, referenceLogFolder

    # @nottest
    def test_new_buffering(self):
        benchmark_list = "sanity/New_Buffering.list"
        referenceLogFolder = "/New_Buffering/IWLS"
        experiment = Experiment_New_Buffering()

        benchmarks = self.experimentRunner.parse_benchmarks_list(benchmark_list)
        for benchmark in benchmarks:
            yield self.run, experiment, benchmark, referenceLogFolder

    # def DISABLED_test_HPWL_placement(self):
    #     benchmark_list = "IWLS05.list"
    #     referenceLogFolder = "/HPWL/IWLS"
    #     experiment = Experiment_HPWL()
    #     self.experimentRunner.run_experiment(experiment, benchmark_list, referenceLogFolder)
    #
    # def DISABLED_test_lr(self):
    #     benchmark_list = "IWLS_GP_r1511_fast.list"
    #     referenceLogFolder = "/LR"
    #     experiment = Experiment_LR()
    #     self.experimentRunner.run_experiment(experiment, benchmark_list, referenceLogFolder)


if __name__ == "__main__":
    nose.main()