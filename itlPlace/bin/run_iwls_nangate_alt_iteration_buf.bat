@echo off
for /f %%b in (.\IWLS05\original_list.txt) do (
itlPlacerelease.exe Config_for_night_experiments.cfg --params.def=.\IWLS05\%%b.def --DesignFlow.nMacroIterations=9 --DesignFlow.Buffering=true --NetWeighting.method=SensitivityGuidedNetWeighting --params.techname=NangateMapped
copy /Y Replicated_Config_for_night_experiments.cfg .\cfg\%date:~-4,4%%date:~-7,2%%date:~-10,2%%time:~-11,2%%time:~-8,2%%time:~-5,2%%time:~-2,2%_ngaltb_%%b.cfg >nul
echo %date% %time% %%b.def completed
)