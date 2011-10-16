from ConfigParser import ConfigParser

from CoreScripts.Checker import Checker
from ParametersParsing import TestRunnerParameters, EmailerParameters
from CoreScripts.Emailer import Emailer
from CoreScripts.TestRunner import TestRunner
from CoreScripts.CoreFunctions import Logger, CreateConfigParser
from Experiment_HippocrateDP import Experiment_HippocrateDP
from Experiment_HPWL import Experiment_HPWL
from Experiment_LR import Experiment_LR
from Experiment_Weighting import Experiment_Weighting
from Experiment_New_Buffering import Experiment_New_Buffering
from FastCheckRunner import FastCheckRunner

def NightExperiment(testRunner):
  exp_HPWL = Experiment_HPWL()
  chk_HPWL_IWLS = Checker(exp_HPWL, r"MasterLogs/HPWL/IWLS")

  exp_HPWL.name = "ISPD04 HPWL Experiment"
  exp_HPWL.SetConfig("hpwl_ispd04.cfg")
  exp_HPWL.SetBenchmarksList("ISPD04.list")
  chk_HPWL_ISPD = Checker(exp_HPWL, r"MasterLogs/HPWL/ISPD")

  chk_LR  = Checker(Experiment_LR(), r"MasterLogs/LR")
  chk_BUF = Checker(Experiment_New_Buffering(), r"MasterLogs/New_Buffering/IWLS")
  chk_HDP = Checker(Experiment_HippocrateDP(), r"MasterLogs/HippocrateDP/Aleksandr")

  exp_W   = Experiment_Weighting()
  chk_SGW = Checker(exp_W, r"MasterLogs/Weighting/SensitivityGuided")

  exp_W.name = "APlace weighting experiment"
  exp_W.SetConfig("APlace_weighting.cfg")
  chk_APW = Checker(exp_W, r"MasterLogs/Weighting/SensitivityGuided")

  testRunner.Append(chk_BUF)
  testRunner.AddExperimentToGroup(chk_SGW)
  testRunner.AddExperimentToGroup(chk_APW)
  testRunner.Append(chk_HPWL_IWLS)
  testRunner.Append(chk_HPWL_ISPD)
  testRunner.Append(chk_LR)
  testRunner.Append(chk_HDP)

  try:
    testRunner.Run()

  except Exception:
    import traceback
    logger = Logger()
    logger.Log("exception: %s" % (traceback.format_exc()))


def RunNightExperiments():
  cfgParser                  = CreateConfigParser()
  nightTestParams            = TestRunnerParameters(cfgParser)
  nightTestParams.doBuild    = True
  nightTestParams.doCheckout = True

  emailerParams            = EmailerParameters(cfgParser)
  emailerParams.doSendMail = True
  emailer                  = Emailer(emailerParams)

  testRunner = TestRunner(nightTestParams, emailer)
  NightExperiment(testRunner)

if (__name__ == "__main__"):
    RunNightExperiments()
