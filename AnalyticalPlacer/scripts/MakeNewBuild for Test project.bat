cls
echo off
     

                                                          
echo ******************************************************
echo Building the tests project
echo ******************************************************
C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727\MSBuild "..\make\itlDragon placer.sln" /t:Build /p:Configuration=Release >> buildLog.txt


