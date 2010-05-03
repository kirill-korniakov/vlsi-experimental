import Parameters
from Parameters import *

class BaseExperiment:
    name = ''
    cfg  = ''
    benchmarks = ''
    cmdLine = ''

    def __init__(self, name, cfg, benchmarks, cmdLine = ''):
        self.name = name
        self.cfg = GeneralParameters.binDir + 'cfg/' + cfg
        self.benchmarks = GeneralParameters.benchmarkCheckoutPath + benchmarks
        self.cmdLine = cmdLine

    def CreateEmptyTable(self, reportTable):
        print('ERROR: you can\'t use BaseExperiment class itself!')

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        print('ERROR: you can\'t use BaseExperiment class itself!')