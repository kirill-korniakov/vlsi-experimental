import subprocess
import sys
from PyQt4 import QtGui
from PyQt4 import QtCore
import datetime
from datetime import date
import time
import os

from itlPlaceEmail import send_mail

import itlPlaceParameters
from itlPlaceParameters import *

chekoutString = "svn"
subprocess.Popen(chekoutString)
