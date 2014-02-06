import os
from ConfigParser import ConfigParser


class CfgParserFactory:
    def __init__(self):
        pass

    def createCfgParser(self):
        parentDir = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))
        configFile = os.path.join(parentDir, "Parameters.conf")

        cfgParser = ConfigParser()
        cfgParser.read(configFile)

        return cfgParser