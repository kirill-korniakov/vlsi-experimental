add = "addpin"
pto = "pto"

def loadCellsList(cellListFile):
    badCells = open(cellListFile, 'r')
    badCellsList = badCells.readlines()
    for index in range(len(badCellsList)):
        badCellsList[index] = badCellsList[index].strip()

    return badCellsList

def findAbsent(fullList, lookForList):
    absent = []

    found = False
    for cell in lookForList:
        for exCell in fullList:
            if cell == exCell:
                found = True
                break
        if found == False:
            absent.append(cell)
        found = False

    return absent

def main():
    a = loadCellsList(add)
    p = loadCellsList(pto)

    absent = findAbsent(p, a)

    print("a size = ", len(a))
    print("p size = ", len(p))
    print("absent size = ", len(absent))
    #print(absent)

main()