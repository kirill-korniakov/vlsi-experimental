echo ******************************************************
echo Start project
echo ******************************************************
for /l %%i in (0,1,5) do (
..\bin\itlDragon.exe -f ..\bin\ibm01\ibm01-cu85.aux -test    >> execLogTest.txt
..\bin\itlDragon.exe -f ..\bin\ibm011\ibm01.aux -test        >> execLogTest.txt
..\bin\itlDragon.exe -f ..\bin\ibm10\ibm10.aux -test         >> execLogTest.txt
..\bin\itlDragon.exe -f ..\bin\ibm10_98\ibm10-cu90.aux -test >> execLogTest.txt
..\bin\itlDragon.exe -f ..\bin\ibm14\ibm14.aux -test         >> execLogTest.txt
)