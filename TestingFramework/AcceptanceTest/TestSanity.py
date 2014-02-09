import os
import sys
path = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(path)

import nose
from nose.tools import nottest

from AcceptanceTest.TestHelper import TestHelper
from Experiments.Experiment_HippocrateDP import Experiment_HippocrateDP
from Experiments.Experiment_New_Buffering import Experiment_New_Buffering


class TestSanity:
    test_helper = TestHelper()

    # @nottest
    def test_hippocrate(self):
        benchmark_list = "sanity/HippocrateDP.list"
        referenceLogFolder = "/HippocrateDP"
        experiment = Experiment_HippocrateDP()

        benchmarks = self.test_helper.expand_benchmark_list(benchmark_list)
        for benchmark in benchmarks:
            yield self.test_helper.run, experiment, benchmark, referenceLogFolder

    @nottest # Because of difference in Linux
    def test_new_buffering(self):
        benchmark_list = "sanity/New_Buffering.list"
        referenceLogFolder = "/New_Buffering/IWLS"
        experiment = Experiment_New_Buffering()

        benchmarks = self.test_helper.expand_benchmark_list(benchmark_list)
        for benchmark in benchmarks:
            yield self.test_helper.run, experiment, benchmark, referenceLogFolder

if __name__ == "__main__":
    nose.main()