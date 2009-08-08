@echo off

::  Build itlPlace (release configuration)
call "%VS80COMNTOOLS%\..\..\VC\vcvarsall.bat"
msbuild .\itlPlace\make\itlPlace.sln /t:%1 /p:Configuration=Release