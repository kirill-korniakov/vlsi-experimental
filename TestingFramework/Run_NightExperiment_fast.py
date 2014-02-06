from CoreScripts.FastCheckRunner import FastCheckRunner
from Run_NightExperiment import NightExperiment

def RunFastExperiments():
    fast_lists = {\
        "IWLS05 HPWL experiment":"IWLS05.list",\
        "IWLS05GP LR experiment":"IWLS_GP_r1511_fast.list",\
        "HippocrateDP experiment":"IWLS_GP_Hippocrate.list",\

       # "IWLS05 new_buffering experiment (NBL)":"IWLS_GP_r1511_New_Buffering.list",\
#        "HPWL_ISPD":"ISPD04_fast.list",\
#        "Weighting (SGNW) experiment":"IWLS05_fast.list",\
#        "APlace weighting experiment":"IWLS05_fast.list",\
        }

    NightExperiment(FastCheckRunner(fast_lists))

if (__name__ == "__main__"):
  RunFastExperiments()