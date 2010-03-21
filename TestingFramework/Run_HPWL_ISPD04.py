import CoreScripts.TestRunner
from CoreScripts.TestRunner import *

import CoreScripts.Emailer
from CoreScripts.Emailer import *

cfg = "hpwl_ispd04.cfg"
blist = "ispd04.list"

def main():
    trp = TestRunnerParameters()
    trp.blist = blist;
    trp.cfg = cfg;

    testRunner = TestRunner(trp)
    testRunner.RunAll()

main()