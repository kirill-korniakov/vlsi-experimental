import subprocess

testSet = ['s298', 'pci_spoci_ctrl', 'tv80', 'ac97_ctrl',
'b21', 'b22', 'b17', 'ethernet', 'RISC', 'des_perf', 'pci_spoci_ctrl']

outputFileName = 'BufferingOutput.xls'

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

    for line in fh.readlines():
        #get HPWL
        idx = line.find('HPWL before buffering: ')
        if idx != -1:
            HPWL = line[idx + len('HPWL before buffering: '):-1]
            po.write(HPWL.replace('.', ',') + '\t')
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

            # get time
            po.write(line[1:11].replace('.', ',') + '\t')
            po.write('\t')
            continue

        #get bHPWL
        idx = line.find('HPWL after buffering (not legalized): ')
        if idx != -1:
            HPWL = line[idx + len('HPWL after buffering (not legalized): '):-1]
            po.write(HPWL.replace('.', ',') + '\t')
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
            isTiming = False

            # get time
            po.write(line[1:11].replace('.', ',') + '\t')
            po.write('\t')
            continue

        #get blHPWL
        idx = line.find('HPWL after buffering and legalization: ')
        if idx != -1:
            
            blHPWL = line[idx + len('HPWL after buffering and legalization: '):-1]
            po.write(blHPWL.replace('.', ',') + '\t')
            continue

        #get blSTA
        idx = line.find('STA after buffering and legalization:')
        if idx != -1:
            isTiming = True
            continue
        idx = line.find('  TNS: ')
        if (isTiming) and (idx != -1):
            blTNS = line[idx + len('  TNS: '):-1]
            po.write(blTNS.replace('.', ',') + '\t')
            continue
        idx = line.find('  WNS: ')
        if (isTiming) and (idx != -1):
            blWNS = line[idx + len('  WNS: '):-1]
            po.write(blWNS.replace('.', ',') + '\t')
            isTiming = False

            # get time
            po.write(line[1:11].replace('.', ',') + '\t')
            po.write('\t')
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
po.write('\tHPWL\tTNS\tWNS\tTime\t\tbHPWL\tbTNS\tbWNS\tbTime\t\tblHPWL\tblTNS\tblWNS\tblTime\t')
po.close()

for benchmark in testSet:
    #Run
    logFileName = '.\\EWDTS-2009\\' + benchmark + '.log'
    fPlacerOutput = open(logFileName, 'w');
    subprocess.Popen(['itlPlaceRelease.exe', 'default.cfg',
        '--benchmark.def=.\\EWDTS-2009\\' + benchmark + '.def'],
        stdout = fPlacerOutput).communicate()
    fPlacerOutput.close()
    print(benchmark + ' is done...')
    #Parse results on current benchmark
    parseLog(logFileName, benchmark)





