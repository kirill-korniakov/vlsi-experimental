from CoreScripts.LogParser import *

def test():
    parser = LogParser('sample.log')
    print(parser.GetFromPFST('INIT', 0))
    print(parser.GetFromPFST('INIT', 1))
    print(parser.GetFromPFST('INIT', 2))
    print(parser.GetFromPFST('INIT', 3))

test()