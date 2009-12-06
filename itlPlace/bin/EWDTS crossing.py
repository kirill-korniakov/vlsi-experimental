import subprocess

testSet = ['s298', 'pci_spoci_ctrl', 'tv80', 'ac97_ctrl',
'b21', 'b22', 'b17', 'b18']

outputFileName = 'CrossingOutput.xls'

def parseLog(logName, benchmark):
    fh = open(logName, 'r')
    po = open(outputFileName, 'a')

    po.write('\n' + benchmark + '\t')    # set the benchmark name

    isTiming = False
    HPWL = 0.0
    TNS  = 0.0
    WNS  = 0.0
    bHPWL = 0.0
    bTNS  = 0.0
    bWNS  = 0.0
    blHPWL = 0.0
    blTNS  = 0.0
    blWNS  = 0.0
    bi = 0
    Candidates = 0
    Reverts = 0

    for line in fh.readlines():
        #get HPWL

        idx = line.find('crossing = ')
        if idx != -1:
            HPWL = line[idx + len('crossing = '):-1]
            po.write(HPWL.replace('.', ',') + '\t')
            continue

    fh.close()
    po.close()

###############################################################################
#Main function
###############################################################################

print('Performing tests on the following set of benchmarks: ' + ', '.join(testSet))
        
po = open(outputFileName, 'w')
#if (po == NULL):
#    print('Couldn't open ' + outputFileName + '. Exiting.')
#    exit
po.write('\tHPWL\tTNS\tWNS\tTime\t\tBuffer inside\tCandidates\tReverts\tbHPWL\tbTNS\tbWNS\tbTime\t\tblHPWL\tblTNS\tblWNS\tblTime\t')
po.close()

for benchmark in testSet:
    #Run
    logFileName = '.\\IWLS05\\' + benchmark + '.log'
    fPlacerOutput = open(logFileName, 'w');
    subprocess.Popen(['itlPlaceRelease.exe', 'default.cfg',
        '--benchmark.def=.\\IWLS05\\' + benchmark + '.def'],
        stdout = fPlacerOutput).communicate()
    fPlacerOutput.close()
    print(benchmark + ' is done...')
    #Parse results on current benchmark
    parseLog(logFileName, benchmark)





