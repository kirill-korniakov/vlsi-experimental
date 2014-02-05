@echo off > outfile
if %1'==' echo which file? && goto :eof
if not exist %1 echo %1 not found && goto :eof
for /f "tokens=* delims= " %%a in (%1) do (
find "%%a" < outfile > nul
if errorlevel 1 echo %%a >> outfile
)