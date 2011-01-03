import CoreScripts
from CoreScripts import *

import FastCheckRunner
from FastCheckRunner import *

import Run_NightExperiment
from Run_NightExperiment import *

def RunFastExperiments():
    fast_lists = {\
#        "IWLS05 HPWL experiment":"IWLS05_fast.list",\
#        "HPWL_ISPD":"ISPD04_fast.list",\
#        "IWLS05GP LR experiment":"IWLS_GP_r1511/IWLS_GP_fast.list",\
#        "IWLS05 new_buffering experiment (NBL)":"IWLS_GP_r1511_New_Buffering.list",\
        "HippocrateDP experiment":"IWLS_GP_Hippocrate.list",\
#        "Weighting (SGNW) experiment":"IWLS05_fast.list",\
#        "APlace weighting experiment":"IWLS05_fast.list",\
        }

    Run_NightExperiment.NightExperiment(FastCheckRunner(fast_lists))

RunFastExperiments()
