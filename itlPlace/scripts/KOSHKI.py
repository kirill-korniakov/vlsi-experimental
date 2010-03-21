import itlPlaceCoreFuctions
from itlPlaceCoreFuctions import *

##import datetime
##from datetime import date
##import time
##
##class CoolPrinter:
##    startTime = 0
##
##    def __init__(self):
##        self.startTime = time.time()
##
##    def PrintWorkTime(self):
##        runTime = time.time() - self.startTime
##        print("RUNTIME %2.2i:%2.2i" % (int(runTime/60), int(runTime%60)))
##
##    def CoolPrint(self, message):
##        print('\n')
##        print('######################################################')
##        print('####### ' + message)
##        print('######################################################')
##        self.PrintWorkTime()
##        print('\n')

def main():
    print("qq")
    cp = CoolPrinter()

    cp.CoolPrint("start")
    time.sleep(1)
    cp.CoolPrint("1")
    time.sleep(2)
    cp.CoolPrint("2")

main()