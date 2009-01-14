@echo off
set path="%VS80COMNTOOLS%..\Packages\Debugger\"
copy /-Y %path%autoexp.dat %path%autoexp.dat.bak
copy /Y autoexp.dat %path%autoexp.dat > nul

