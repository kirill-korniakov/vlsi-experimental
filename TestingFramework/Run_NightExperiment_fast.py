from CoreScripts.FastCheckRunner import FastCheckRunner
from Run_NightExperiment import NightExperiment


def run_fast_experiments():
    fast_lists = {
        # "IWLS05 HPWL experiment": "IWLS05.list",
        # "IWLS05GP LR experiment": "IWLS_GP_r1511_fast.list",
        # "IWLS05 new_buffering experiment (NBL)": "IWLS_GP_r1511_New_Buffering.list",
        "HippocrateDP experiment": "IWLS_GP_Hippocrate.list",

        # "HPWL_ISPD": "ISPD04.list",
        # "Weighting (SGNW) experiment": "IWLS05.list",
        # "APlace weighting experiment": "IWLS05.list",
    }

    NightExperiment(FastCheckRunner(fast_lists))


if __name__ == "__main__":
    run_fast_experiments()