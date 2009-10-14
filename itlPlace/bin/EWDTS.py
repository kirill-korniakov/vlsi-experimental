import subprocess
import sys

def isNotCommented(str):
    return not str.startswith('#')

def parseLog(logName, benchmark):
    fh = open(logName, "r")
    po = open('pythonOutput_ewdts.xls', 'a')
    #po = open('pythonOutput_' + logName[:-4] + '.txt', 'w');
    po.write('\n' + benchmark + '\t')    # set the benchmark name
    isTiming = False
    HPWL = 0.0
    TNS  = 0.0
    WNS  = 0.0
    for line in fh.readlines():
        idx = line.find('HPWL after  detailed placement: ')
        if idx != -1:
            HPWL = line[idx + len('HPWL after  detailed placement: '):-1]
            po.write(HPWL.replace('.', ',') + '\t')
            continue

        idx = line.find('STA after detailed placement:\n')
        if idx != -1:
            isTiming = True
            continue
        idx = line.find('  TNS: ')
        if (isTiming) and (idx != -1):
            TNS = line[idx + len('  TNS: '):-1]
            po.write(TNS.replace('.', ',') + '\t')
            continue
        idx = line.find('  WNS: ')
        if (isTiming) and (idx != -1):
            WNS = line[idx + len('  WNS: '):-1]
            po.write(WNS.replace('.', ',') + '\t')
            isTiming = False

            # get time
            po.write(line[1:11].replace('.', ',') + '\t')
            po.write('\t')
            continue
    fh.close()
    po.close()

if len(sys.argv) != 3:
    print("File names *.list and *.cfg are required to run the test sequence")
    exit()

listName = sys.argv[1]
cfgName  = sys.argv[2]
testSet  = (open(listName).read()).split('\n')
#filter(isNotCommented, '#jjfjf')
for x in testSet:
    if x.startswith('#'):
        testSet.remove(x)


print("Performing tests on the following set of benchmarks: " + ", ".join(testSet))

po = open('pythonOutput_ewdts.xls', 'w')
#if (po == NULL):
#    print("Couldn't open pythonOutput.xls. Exiting.")
#    exit
for i in range(0, 9):
    po.write('\tHPWL\tTNS\tWNS\tTime\t')
po.close()

for benchmark in testSet:
    logFileName = ".\\IWLS05\\" + benchmark + ".log"
    fPlacerOutput = open(logFileName, 'w');
    subprocess.Popen(["itlPlaceRelease.exe", cfgName,
        "--params.def=.\\IWLS05\\" + benchmark + ".def"],
        stdout = fPlacerOutput).communicate()
    fPlacerOutput.close()
    print(benchmark + ' is done...')
    parseLog(logFileName, benchmark)





