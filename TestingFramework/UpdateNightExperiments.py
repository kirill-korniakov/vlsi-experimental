import subprocess
import sys
import os

def CheckOut():
            print("Updating Run_NightExperiment.py")
            cmdline = r"C:\Program Files (x86)\CollabNet\Subversion Client\svn.exe up Run_NightExperiment.py"
            subprocess.call(cmdline)
            print("updated")
CheckOut()
