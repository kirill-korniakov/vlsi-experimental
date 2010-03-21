@echo off

::  Build itlPlace (release configuration)
call "%VS80COMNTOOLS%\..\..\VC\vcvarsall.bat"
msbuild %1 /t:%2 /p:Configuration=Release