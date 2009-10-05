import subprocess

testSet = ['_tv80']

outputFileName = 'BufferingOutput.xls'

def parseLog(logName, benchmark):
    fh = open(logName, 'r')
    po = open(outputFileName, 'a')

    po.write('\n' + benchmark + '\t')    # set the benchmark name

    isTiming = False
    HPWL = 0.0
    TNS  = 0.0
    WNS  = 0.0
    HPWL = 0.0
    bTNS  = 0.0
    bWNS  = 0.0
    blHPWL = 0.0
    blTNS  = 0.0
    blWNS  = 0.0
    bi = 0
    Candidates = 0
    Reverts = 0
    maxSlack = 0.0
    PinsCount = 0
    WL = 0.0
    Lext = 0.0
    vgSlack = 0.0

    for line in fh.readlines():
        po.write('\n');  
        
        idx = line.find('net name ')
        if idx != -1:
            net = line[idx + len('net name '):-1]
            po.write(net + '\t')
            continue

        
        #get STA
        idx = line.find('STA before buffering:\n')
       
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
            continue
        idx = line.find('maxSlack = ')
        if  (idx != -1):
            maxSlack = line[idx + len('maxSlack = '):-1]
            po.write(maxSlack.replace('.', ',') + '\t\t')
            continue
        idx = line.find('Pins count = ')
        if idx != -1:
            PinsCount = line[idx + len('Pins count = '):-1]
            po.write(PinsCount + '\t')
            continue
        idx = line.find('HPWL = ')
        if idx != -1:
            HPWL = line[idx + len('HPWL = '):-1]
            po.write(HPWL.replace('.', ',') + '\t')
            continue
        idx = line.find('WL = ')
        if idx != -1:
            WL = line[idx + len('WL = '):-1]
            po.write(WL.replace('.', ',') + '\t')
            continue
        idx = line.find('Lext = ')
        if idx != -1:
            Lext = line[idx + len('Lext = '):-1]
            po.write(Lext.replace('.', ',') + '\t')
            continue
        idx = line.find('Buffer count = ')
        if idx != -1:
            bi = line[idx + len('Buffer count = '):-1]
            po.write(bi + '\t')
            continue

        #get bSTA
        idx = line.find('STA after buffering (real):\n')
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
            continue
        idx = line.find('vgSlack = ')
        if (isTiming) and (idx != -1):
            vgSlack = line[idx + len('vgSlack = '):-1]
            po.write(vgSlack.replace('.', ',') + '\t')
            isTiming = False
            continue


        #get blSTA
        idx = line.find('STA after buffering in source:')
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
            continue
        idx = line.find('vgSlack = ')
        if (isTiming) and (idx != -1):
            vgSlack = line[idx + len('vgSlack = '):-1]
            po.write(vgSlack.replace('.', ',') + '\t\n')
            isTiming = False
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
po.write('\tnet\tTNS\tWNS\tmaxSlack\tPins count\tHPWL\tWL\tLext\tBuffer count\t\tbrTNS\tbrWNS\tvgrSlack\t\tbTNS\tbWNS\tvgSlack\t')
po.close()

for benchmark in testSet:
    #Run
    logFileName = '.\\IWLS05\\' + benchmark + '.log'
    fPlacerOutput = open(logFileName, 'w');
    subprocess.Popen(['itlPlacerelease.exe', 'default.cfg',
        '--benchmark.def=.\\IWLS05\\' + benchmark + '.def'],
        stdout = fPlacerOutput).communicate()
    fPlacerOutput.close()
    print(benchmark + ' is done...')
    #Parse results on current benchmark
    parseLog(logFileName, benchmark)





