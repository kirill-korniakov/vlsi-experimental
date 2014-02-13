import os
from ConfigParser import ConfigParser


class CfgParserFactory:
    def __init__(self):
        pass

    @staticmethod
    def get_root_dir():
        parentDir = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir)) + '/'
        return parentDir

    @staticmethod
    def createCfgParser():
        parentDir = CfgParserFactory.get_root_dir()
        configFile = os.path.join(parentDir, "Parameters.conf")

        cfgParser = ConfigParser()
        cfgParser.read(configFile)

        return cfgParser