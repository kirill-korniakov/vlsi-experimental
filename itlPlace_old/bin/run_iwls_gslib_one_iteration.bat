@echo off
for /f %%b in (.\IWLS05\original_list.txt) do (
itlPlacerelease.exe Config_for_night_experiments.cfg --params.def=.\IWLS05\%%b.def --DesignFlow.nMacroIterations=1 --DesignFlow.Buffering=false --NetWeighting.method=APlace --params.techname=IWLS
set hh=%time:~0,2%
if "%time:~0,1%"==" " set hh=0%hh:~1,1%
copy /Y Replicated_Config_for_night_experiments.cfg .\cfg\%date:~-4,4%%date:~-7,2%%date:~-10,2%%hh%%time:~-8,2%%time:~-5,2%%time:~-2,2%_one_%%b.cfg >nul
echo %date% %time% %%b.def completed
)