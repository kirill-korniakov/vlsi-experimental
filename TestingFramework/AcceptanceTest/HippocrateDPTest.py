import os
from CoreScripts import Logger
from CoreScripts.CfgParserFactory import CfgParserFactory
from CoreScripts.Checker import Checker
from CoreScripts.TestRunner import TestRunner
from Experiments.Experiment_HippocrateDP import Experiment_HippocrateDP
from unittest import TestCase


class HippocrateTest(TestCase):
    name = "HippocrateDP experiment"
    benchmark_list = "IWLS_GP_Hippocrate.list"

    def test_simple(self):
        self.assertEqual(1, 1)

    def test_full(self):
        cfgParser = CfgParserFactory.createCfgParser()
        referenceLogs = os.path.join(CfgParserFactory.get_root_dir(), cfgParser.get("ReportParameters", "ReferenceLogs"))

        chk_HDP = Checker(Experiment_HippocrateDP(), referenceLogs + r"/HippocrateDP")

        testRunner = TestRunner()
        testRunner.Append(chk_HDP)

        try:
            testRunner.Run()

        except Exception:
            import traceback

            logger = Logger()
            logger.Log("exception: %s" % (traceback.format_exc()))
