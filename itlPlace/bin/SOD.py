import subprocess

configName = 'default.cfg'
outputFileName = 'SOD.csv'

testSet = ['s298', 'pci_spoci_ctrl']

##testSet = ['s298',
##           'pci_spoci_ctrl',
##           'tv80',
##           'ac97_ctrl',
##           'b21',
##           'b22',
##           'b17',
##           'des_perf']


def findAndDumpHPWL(stringList, pattern, resultTable):
    for line in stringList:
        idx = line.find(pattern)
        if idx != -1:
            HPWL = line[idx + len(pattern):-1]
            resultTable.write(HPWL.replace('.', ',') + ';')
            return
    print("ERROR: pattern was not found!\n")


def findAndDumpTNS(src, resultTable):
    idx = src.find('  TNS: ')
    if idx != -1:
        TNS = src[idx + len('  TNS: '):-1]
        resultTable.write(TNS.replace('.', ',') + ';')
        return True
    return False


def findAndDumpWNS(src, resultTable):
    idx = src.find('  WNS: ')
    if idx != -1:
        WNS = src[idx + len('  WNS: '):-1]
        resultTable.write(WNS.replace('.', ',') + ';')

        # get time
        resultTable.write(src[1:11].replace('.', ',') + ';')
        resultTable.write(';')

        return True
    return False


def findAndDumpSTA(stringList, pattern, resultTable):
    isTiming = False

    for line in stringList:
        idx = line.find(pattern)
        if idx != -1:
            isTiming = True
            continue

        if isTiming == True:
            if findAndDumpTNS(line, resultTable) == True:
                continue

            if findAndDumpWNS(line, resultTable) == True:
                return


def parseLog(logName, benchmark):
    fh = open(logName, 'r')
    stringList = fh.readlines()

    resultTable = open(outputFileName, 'a')
    resultTable.write('\n' + benchmark + ';')    # set the benchmark name

    findAndDumpHPWL(stringList, 'HPWL before buffering: ', resultTable)
    findAndDumpSTA(stringList, 'STA before buffering:\n', resultTable)

    findAndDumpHPWL(stringList, 'HPWL after buffering (not legalized): ', resultTable)
    findAndDumpSTA(stringList, 'STA after buffering (not legalized):\n', resultTable)

    findAndDumpHPWL(stringList, 'HPWL after buffering and legalization: ', resultTable)
    findAndDumpSTA(stringList, 'STA after buffering and legalization:\n', resultTable)

    fh.close()
    resultTable.close()


def main():
    print('Performing tests on the following set of benchmarks: ' + ', '.join(testSet))

    resultTable = open(outputFileName, 'w')
    resultTable.write(';HPWL;TNS;WNS;Time;;bufferedHPWL;bTNS;bWNS;bTime;;blHPWL;blTNS;blWNS;blTime;')
    resultTable.close()

    for benchmark in testSet:
        logFileName = '.\\IWLS05\\' + benchmark + '.log'
        fPlacerOutput = open(logFileName, 'w');
        subprocess.Popen(['itlPlaceRelease.exe', configName,
            '--benchmark.def=.\\IWLS05\\' + benchmark + '.def'],
            stdout = fPlacerOutput).communicate()
        fPlacerOutput.close()
        print(benchmark + ' is done...')
        #Parse results on current benchmark
        parseLog(logFileName, benchmark)

main()