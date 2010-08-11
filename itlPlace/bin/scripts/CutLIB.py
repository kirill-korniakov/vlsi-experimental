badCellsFile = "biaridec_c1355_DEF_cells.txt"
lefFile = 'v00_nhm_nominal_1266_v110.lib'

openPattern = "  lu_table_template (delay_outputslew_template_5X5_"
closePattern = openPattern
finalPattern = "}\n"

def loadCellsList(cellListFile):
    badCells = open(cellListFile, 'r')
    badCellsList = badCells.readlines()
    for index in range(len(badCellsList)):
        badCellsList[index] = badCellsList[index].strip()

    return badCellsList

def ReadFileIntoString(file):
    f = open(file, 'r')
    content = f.read()
    return content

def WriteFile(content, fileName):
    f = open(fileName, 'w')
    f.write(content)
    f.close()

def ConcatenateCuttedFileContent(lefContent, cellList):
    #copy initial section
    idx = lefContent.find(openPattern)
    lefContentCutted = lefContent[0:idx]

    #add each cell
    for cell in cellList:
        idxFrom = lefContent.find(openPattern + cell)
        idxTo = lefContent.find(closePattern, idxFrom + 1)
        if idxTo == -1:
            print("FAIL")
            return "FAIL"
        lefContentCutted = lefContentCutted + lefContent[idxFrom:idxTo] + '\n\n'

    #insert ending
    lefContentCutted = lefContentCutted + finalPattern

    return lefContentCutted

def main():
    cellList = loadCellsList(badCellsFile)
    lefContent = ReadFileIntoString(lefFile)

    lefContentCutted = ConcatenateCuttedFileContent(lefContent, cellList)

    WriteFile(lefContentCutted, 'cutted_' + lefFile)

main()