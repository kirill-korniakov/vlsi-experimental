import subprocess
import sys
import os

import CoreFunctions
from CoreFunctions import *

class SvnWorker:
    def DeleteSources(self, dir):
        print('Deleting previous version of itlPlace...')
        if os.path.exists(dir):
            RemoveDir(dir)
        print('Done')

    def CheckOut(self, url, to, rev = ''):
        if rev == '':
            print('Checking out HEAD revision')
            cmdline = 'C:\Program Files (x86)\CollabNet\Subversion Client\svn.exe co ' + url + ' ' + to
            subprocess.call(cmdline)
        else :
            print('Checking out revision ' + rev)
            cmdline = 'svn co -r ' + rev + ' ' + url + ' ' + to
            subprocess.call(cmdline)
