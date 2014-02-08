import os

from CoreScripts import ExperimentLauncher, GeneralParameters, ReportParameters, ResultsStorage, Logger, \
    ComparisonResult
from CoreScripts.CfgParserFactory import CfgParserFactory
from CoreScripts.Checker import Checker
from CoreScripts.ReportCreator import ReportCreator
from Experiments.Experiment_HippocrateDP import Experiment_HippocrateDP
from unittest import TestCase


class HippocrateTest(TestCase):
    benchmark_list = "IWLS_GP_Hippocrate.list"
    referenceLogFolder = "/HippocrateDP"

    nTerminatedBenchmarks = 0
    MAX_TERMINATED_BENCHMARKS_NUM = 3

    def test_full(self):
        cfgParser = CfgParserFactory.createCfgParser()
        referenceLogs = os.path.join(CfgParserFactory.get_root_dir(),
                                     cfgParser.get("ReportParameters", "ReferenceLogs"))

        experiment = Experiment_HippocrateDP()
        experiment.SetBenchmarksList(self.benchmark_list)
        checked_HDP = Checker(experiment, referenceLogs + self.referenceLogFolder)
        experiment = checked_HDP

        generalParameters = GeneralParameters(cfgParser)
        reportParameters = ReportParameters(cfgParser)

        storage = ResultsStorage()
        launcher = ExperimentLauncher(checked_HDP, storage, None)

        logger = Logger()
        logger.Log("Config: %s" % experiment.cfg)
        logger.Log("Benchmarks: %s" % experiment.benchmarks)

        reportCreator = ReportCreator(experiment.name, experiment.cfg, reportParameters)
        logFolder = reportCreator.CreateLogFolder()
        reportTable = reportCreator.GetReportTableName()

        experiment.CreateEmptyTable(reportTable)
        benchmarks = launcher.CheckParametersAndPrepareBenchmarks()

        for benchmark in benchmarks:
            if self.nTerminatedBenchmarks >= self.MAX_TERMINATED_BENCHMARKS_NUM:
                logger.Log("Reached maximum number of terminated benchmarks")
                break

            launcher.RunExperimentOnBenchmark(benchmark, logFolder, reportTable, generalParameters)

            result = launcher.experimentResults.benchmarkResults.get(benchmark)
            self.assertEqual(ComparisonResult.OK, result, "Benchmark's result wasn't OK")

            logger.Log("[%s/%s] %s is finished\n" % (benchmarks.index(benchmark) + 1, len(benchmarks), benchmark))
