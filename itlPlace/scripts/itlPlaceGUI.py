import subprocess
import sys
import os

import datetime
from datetime import date
import time

from PyQt4 import QtGui
from PyQt4 import QtCore

import itlPlaceTestRunner
from itlPlaceTestRunner import *

class DistributionBuilder(QtGui.QWidget):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.setGeometry(300, 300, 320, 100)
        self.setWindowTitle(itlPlaceParameters.windowTitle)

        #field for SVN revision
        lblRev = QtGui.QLabel('SVN Revision (keep empty for HEAD)', self)
        self.teditRev = QtGui.QLineEdit('', self)

        #field for bugzilla issue number
        #lblIssue = QtGui.QLabel('Bugzilla issue (keep empty for empty)', self)
        #self.teditIssue = QtGui.QLineEdit('', self)

        self.cbCheckout = QtGui.QCheckBox('Checkout', self)
        #self.cbCheckout.toggle();

        self.cbRebuild = QtGui.QCheckBox('Build', self)
        #self.cbRebuild.toggle();

        self.gridGroupBoxISPD = QtGui.QGroupBox(self.tr(itlPlaceParameters.ispd04))
        layoutISPD = QtGui.QGridLayout()
        self.cbISPD04 = QtGui.QCheckBox(itlPlaceParameters.ispd04, self)
        self.cbISPD04.toggle()
        layoutISPD.addWidget(self.cbISPD04, 9, 0)
        self.cbISPD04BDP = QtGui.QCheckBox('Collect BeforeDP data', self)
        self.cbISPD04BDP.toggle()
        layoutISPD.addWidget(self.cbISPD04BDP, 9, 1)
        self.cbISPD04isDP = QtGui.QCheckBox('Perform DP', self)
        self.cbISPD04isDP.toggle()
        layoutISPD.addWidget(self.cbISPD04isDP, 9, 2)

        self.gridGroupBoxIWLS = QtGui.QGroupBox(self.tr(itlPlaceParameters.iwls05))
        layoutIWLS = QtGui.QGridLayout()
        self.cbIWLS05 = QtGui.QCheckBox(itlPlaceParameters.iwls05, self)
        self.cbIWLS05.toggle()
        layoutIWLS.addWidget(self.cbIWLS05, 10, 0)
        self.cbIWLS05BDP = QtGui.QCheckBox('Collect BeforeDP data', self)
        self.cbIWLS05BDP.toggle()
        layoutIWLS.addWidget(self.cbIWLS05BDP, 10, 1)
        self.cbIWLS05isDP = QtGui.QCheckBox('Perform DP', self)
        self.cbIWLS05isDP.toggle()
        layoutIWLS.addWidget(self.cbIWLS05isDP, 10, 2)

        self.gridGroupBoxISPD.setLayout(layoutISPD)
        self.gridGroupBoxIWLS.setLayout(layoutIWLS)

        btnMD = QtGui.QPushButton('Run', self)
        btnMD.setFocus()

        grid = QtGui.QGridLayout()

        grid.addWidget(lblRev, 1, 0)
        grid.addWidget(self.teditRev, 1, 1)
        grid.addWidget(self.cbCheckout, 7, 0)
        grid.addWidget(self.cbRebuild, 8, 0)
        grid.addWidget(self.gridGroupBoxISPD, 9, 0)
        grid.addWidget(self.gridGroupBoxIWLS, 10, 0)
        grid.addWidget(btnMD, 12, 1)

        self.setLayout(grid)

        self.connect(btnMD, QtCore.SIGNAL('clicked()'), self.btnMDClicked)
        self.connect(self, QtCore.SIGNAL('closeEmitApp()'), QtCore.SLOT('close()') )

    # the main function
    def btnMDClicked(self):
        testRunner = itlPlaceTestRunner()
        testRunner.RunAll()

app = QtGui.QApplication(sys.argv)
db = DistributionBuilder()
db.show()
#db.btnMDClicked()
app.exec_()
