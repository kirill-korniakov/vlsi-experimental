import subprocess

testSet = ['_b17']

outputFileName = 'BufferingOutput'

def parseLog(logName, benchmark ):
    fh = open(logName, 'r')
    po = open(outputFileName + benchmark + '.xls', 'a')

    po.write('\n')    # set the benchmark name

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
    vgSlack = 0
    maxSlack = 0
    netIter = 0
    net = ''
    nNet = 0

    for line in fh.readlines():

        idx = line.find('  Nets count    = ')
        if idx != -1:
            nNet = line[idx + len('  Nets count    = '):-1]
            continue
        
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
            continue
        idx = line.find('maxSlack = ')
        if (isTiming) and (idx != -1):
            maxSlack = line[idx + len('maxSlack = '):-1]
            po.write(maxSlack.replace('.', ',') + '\t\t')
            isTiming = False
            continue

        #get bSTA
        idx = line.find('STA after buffering (not legalized):\n')
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

        idx = line.find('netIter = ')
        if idx != -1:
            netIter = line[idx + len('netIter = '):-1]
            continue


    fh.close()
    po.close()
    return netIter
###############################################################################
#Main function
###############################################################################

print('Performing tests on the following set of benchmarks: ' + ', '.join(testSet))
        

for benchmark in testSet:
    po = open(outputFileName + benchmark + '.xls', 'w')
    #if (po == NULL):
    #    print('Couldn't open ' + outputFileName + '. Exiting.')
    #    exit
    po.write('net\tTNS\tWNS\tmaxSlack\t\tbTNS\tbWNS\tvgSlack\t')
    po.close()
    netIter2 = "0"
    neti=0
    ni = ""
    while neti < 200:
        #Run
        logFileName = '.\\IWLS05\\' + benchmark + '.log'
        fPlacerOutput = open(logFileName, 'w');
        netIter = 0.0
        neti = neti + 1;
        #netIter = netIter2;
        #ni = netIter.conjugate.__str__();
        
        subprocess.Popen(['itlPlaceRelease.exe', 'default.cfg','--benchmark.def=.\\IWLS05\\' + benchmark + '.def', '--Buffering.netIter=' + netIter2],
            stdout = fPlacerOutput).communicate()
        fPlacerOutput.close()
        print(benchmark + ' is done...')
        #Parse results on current benchmark
        netIter2 = parseLog(logFileName, benchmark)
        print(netIter2);
        





