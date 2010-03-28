class BaseExperiment:
    name = 'ISPD04 experiment'
    cfg = 'cfg/hpwl_ispd04.cfg' #path relative to bin directory
    benchmarks = '../../Benchmarks/ISPD04.list'

    def CreateEmptyTable(self, reportTable):
        print('ERROR')

    def ParseLogAndFillTable(self, logName, benchmark, reportTable):
        print('ERROR')