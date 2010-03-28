import Parameters
from Parameters import *

class BaseExperiment:
    name = ''
    cfg  = ''
    benchmarks = ''

    def __init__(self, name, cfg, benchmarks):
        self.name = name
        self.cfg = GeneralParameters.binDir + 'cfg/' + cfg
        self.benchmarks = GeneralParameters.benchmarkCheckoutPath + benchmarks

    def CreateEmptyTable(self, reportTable):
        print('ERROR')

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        print('ERROR')