from CoreScripts.LogParser import *


def test():
    parser = LogParser('sample.log')
    print(parser.GetFromPFST('INIT', 'HPWL'))
    print(parser.GetFromPFST('INIT', 'TNS'))
    print(parser.GetFromPFST('INIT', 'WNS'))
    print(parser.GetFromPFST('INIT', 'TWL'))


test()