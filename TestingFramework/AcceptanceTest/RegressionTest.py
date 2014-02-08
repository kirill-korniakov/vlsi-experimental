import os
import sys
path = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(path)

import unittest

from CoreScripts import ExperimentLauncher, GeneralParameters, ReportParameters, ResultsStorage, Logger, \
    ComparisonResult
from CoreScripts.CfgParserFactory import CfgParserFactory
from CoreScripts.Checker import Checker
from CoreScripts.ReportCreator import ReportCreator
from Experiments.Experiment_HPWL import Experiment_HPWL
from Experiments.Experiment_HippocrateDP import Experiment_HippocrateDP
from Experiments.Experiment_LR import Experiment_LR
from Experiments.Experiment_New_Buffering import Experiment_New_Buffering
from unittest import TestCase


class RegressionTest(TestCase):
    nTerminatedBenchmarks = 0
    MAX_TERMINATED_BENCHMARKS_NUM = 3
    # TODO:
    # "APlace weighting experiment": "IWLS05.list",
    # "HPWL_ISPD": "ISPD04.list", -- crash
    # "Weighting (SGNW) experiment": "IWLS05.list", -- crash

    def test_hippocrate_detailed_placement(self):
        self.benchmark_list = "IWLS_GP_Hippocrate.list"
        self.referenceLogFolder = "/HippocrateDP"
        experiment = Experiment_HippocrateDP()
        self.run_experiment(experiment)

    def test_new_buffering(self):
        self.benchmark_list = "IWLS_GP_r1511_New_Buffering.list"
        self.referenceLogFolder = "/New_Buffering/IWLS"
        experiment = Experiment_New_Buffering()
        self.run_experiment(experiment)

    def DISABLED_test_HPWL_placement(self):
        self.benchmark_list = "IWLS05.list"
        self.referenceLogFolder = "/HPWL/IWLS"
        experiment = Experiment_HPWL()
        self.run_experiment(experiment)

    def DISABLED_test_lr(self):
        self.benchmark_list = "IWLS_GP_r1511_fast.list"
        self.referenceLogFolder = "/LR"
        experiment = Experiment_LR()
        self.run_experiment(experiment)

    def run_experiment(self, experiment):
        cfgParser = CfgParserFactory.createCfgParser()
        referenceLogs = os.path.join(CfgParserFactory.get_root_dir(),
                                     cfgParser.get("ReportParameters", "ReferenceLogs"))

        experiment.SetBenchmarksList(self.benchmark_list)
        checked_HDP = Checker(experiment, referenceLogs + self.referenceLogFolder)
        experiment = checked_HDP

        generalParameters = GeneralParameters(cfgParser)
        reportParameters = ReportParameters(cfgParser)

        storage = ResultsStorage()
        launcher = ExperimentLauncher(checked_HDP, storage, None)

        logger = Logger()
        logger.LogD("Config: %s" % experiment.cfg)
        logger.LogD("Benchmarks: %s" % experiment.benchmarks)

        reportCreator = ReportCreator(experiment.name, experiment.cfg, reportParameters)
        logFolder = reportCreator.CreateLogFolder()
        reportTable = reportCreator.GetReportTableName()

        experiment.CreateEmptyTable(reportTable)
        benchmarks = launcher.CheckParametersAndPrepareBenchmarks()
        logger.LogD("List of benchmarks:\n   * %s\n" % ("\n   * ".join(benchmarks)))

        for benchmark in benchmarks:
            if self.nTerminatedBenchmarks >= self.MAX_TERMINATED_BENCHMARKS_NUM:
                logger.Log("Reached maximum number of terminated benchmarks")
                break

            launcher.RunExperimentOnBenchmark(benchmark, logFolder, reportTable, generalParameters)

            result = launcher.experimentResults.benchmarkResults.get(benchmark)
            self.assertEqual(ComparisonResult.OK, result, "Benchmark's result wasn't OK")

            logger.LogD("[%s/%s] %s is finished" % (benchmarks.index(benchmark) + 1, len(benchmarks), benchmark))

        logger.LogD("Experiment is finished\n")

if __name__ == "__main__":
    unittest.main()