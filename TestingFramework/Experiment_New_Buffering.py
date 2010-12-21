import CoreScripts
from CoreScripts import *

import Parameters
from Parameters import *

class Experiment_New_Buffering(BaseExperiment):
    def __init__(self):
       # _metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
       # _stages  = ['INIT','GP1','NBUF1', 'LEGB1', 'GP2' ,  'NBUF2', 'LEGB2', 'GP3' ,  'NBUF3', 'LEGB3', 'GP4'  , 'NBUF4', 'LEGB4', 'GP5'  , 'NBUF5', 'LEGB5', 'GP6'  , 'NBUF6', 'LEGB6', 'GP7'   ,'NBUF7' ,'LEGB7' ,'GP8'  , 'NBUF8' ,'LEGB8', 'GP9'  , 'NBUF9', 'LEGB9', 'GP10' , 'NBUF10','LEGB10']
        _metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
        _stages  = ['INIT', 'GP',  'NBUF', 'LEGB']
        BaseExperiment.__init__(self, 'IWLS05 new_buffering experiment (NBL)',\
            'New_buffering.cfg', 'IWLS05.list', _metrics, _stages)

def test():
    testRunner = TestRunner()

    #e.metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
    #e.stages  = ['INIT','GP1','NBUF1', 'LEGB1', 'GP2' ,  'NBUF2', 'LEGB2', 'GP3' ,  'NBUF3', 'LEGB3', 'GP4'  , 'NBUF4', 'LEGB4', 'GP5'  , 'NBUF5', 'LEGB5', 'GP6'  , 'NBUF6', 'LEGB6', 'GP7'   ,'NBUF7' ,'LEGB7' ,'GP8'  , 'NBUF8' ,'LEGB8', 'GP9'  , 'NBUF9', 'LEGB9', 'GP10' , 'NBUF10','LEGB10']

    #e.MakeResultTable('buf', '111')
    metrics = ['Time', 'Cells', 'HPWL', 'TWL', 'TNS', 'WNS']
    stages  = ['INIT', 'GP', 'NBUF', 'LEGB']

    
    TotalAllowableBuffersAreas = ['0.001', '0.005', '0.01', '0.03']
    Intervals = ['2', '20'];
    AdaptiveSizeBufferMultipliers = ['false','true']
    UseBufferings = ['false','true']
    NumberMetaIterationStartBufferings = ['1', '2']
    
    for NumberMetaIterationStartBuffering in NumberMetaIterationStartBufferings:
        for AdaptiveSizeBufferMultiplier in AdaptiveSizeBufferMultipliers:
            for Interval in Intervals:
                for TotalAllowableBuffersArea in TotalAllowableBuffersAreas:
                    expName = 'NBL_int_'+Interval+'_Area_'+TotalAllowableBuffersArea+'_AdaptiveMultipliers_'+AdaptiveSizeBufferMultiplier+'_NumberMetaIteration_'+ NumberMetaIterationStartBuffering
                    print(expName)                    
                    cmdArgs = ["--plotter.pixDirectory=.\\\\pix\\\\"+expName+"\\\\",
                       " --LSE.GlobalPlacement.New_Buffering.AdaptiveSizeBufferMultiplier="+AdaptiveSizeBufferMultiplier,
                       " --LSE.GlobalPlacement.New_Buffering.TotalAllowableBuffersArea="+TotalAllowableBuffersArea,
                       " --LSE.GlobalPlacement.New_Buffering.NumberMetaIterationStartBuffering="+NumberMetaIterationStartBuffering,
                       " --LSE.GlobalPlacement.New_Buffering.Interval="+Interval]
                    print(cmdArgs);

                    e = BaseExperiment(expName, 'New_buffering.cfg', 'IWLS05.list', metrics, stages, cmdArgs)
                    testRunner.AddExperimentToGroup(e)
    testRunner.Run()

if (__name__ == "__main__"):
    test()
