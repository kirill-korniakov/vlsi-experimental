/*  
* SA_Window.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include "..\include\SA_Window.h"
#include <math.h>

#define nItersOnSortedCells 1000

void BubbleSortWindowElement(RectangularWindowElement* a, int N)
{
    for (int i = 0; i < N - 1; i++)
        for (int j = i + 1; j < N; j++)
            if ((a[i].GetY() < a[j].GetY()) ||
                ((a[i].GetY() == a[j].GetY()) && (a[i].XCoord > a[j].XCoord)))
            {
                RectangularWindowElement temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
}

SAWindow::SAWindow(int _nRows, int _first, double x_array[], int _num, const Circuit &circuit, int indexes[],
                   double _wl, int *changedNets, int changedNetsCount):
RectangularWindow(_nRows, _first, x_array, _num, circuit, indexes)
{
    T = MAX_T;
    currWL = _wl;
    nNets = changedNetsCount;
    windowNets = new int [nNets];

    for (int i = 0; i < nNets; i++)
        windowNets[i] = changedNets[i];

    for (int i = 0; i < elementsNum; i++)
    {
        elements[i].width = circuit.nodes[elements[i].cellIndex].width;
    }
}

double SAWindow::GetFuncValue(Circuit &circuit)
{
    PlaceCells(circuit);
    currWL = cf_recalc_some_nets(true, circuit.nNets, circuit.nets, currWL,windowNets, nNets, circuit.placement);
    //double WL = cf_recalc_all(false, circuit.nNets, circuit.nets, circuit.placement);
    return currWL;
}

void SAWindow::SearchOnSortedCells(bool trial, Circuit &circuit)
{
    int nIters = nItersOnSortedCells;

    if (trial == true)
        nIters = elementsNum;

    for (int iter = 0; iter < nIters; iter++)
    {
        int randCellInd = GetRandomCell();
        ShiftCell(randCellInd, circuit);
        ReduceTemperature();
    }
}

int SAWindow::GetRandomCell()
{
    int index = rand() % elementsNum;
    return index;
}

void SAWindow::ShiftCell(int cellInd, Circuit &circuit)
{
    BubbleSortWindowElement(elements, elementsNum);
    double startFunc = GetFuncValue(circuit);
    double optimalFunc = startFunc * 10; //optimal coords shoud be changed
    double siteWidth = circuit.rows[0].siteWidth;
    double startX = elements[cellInd].XCoord;
    double startY = elements[cellInd].GetY();
    Coordinates startCoords(startX, startY);
    Coordinates optimalCoords(startX, startY);
    Coordinates currCoords(startX, startY);
    int rowInd;

    for (rowInd = firstRow; rowInd < firstRow + nRows - 1; rowInd++)
        if (fabs(circuit.rows[rowInd].coordinate - elements[cellInd].GetY()) < eps)
            break;

    rowInd -= firstRow; //index of this row in the window

    //----------shifting to the left-----------------------------------------------------------
    if (cellInd == 0) //the first cell
    {
        currCoords.xCoord -= siteWidth;

        while (currCoords.xCoord >= windowBounds[rowInd * 2])
        {
            elements[0].XCoord = currCoords.xCoord;
            double newFuncValue = GetFuncValue(circuit);

            if (optimalFunc > newFuncValue)
            {
                optimalFunc = newFuncValue;
                optimalCoords.xCoord = currCoords.xCoord;
            }

            currCoords.xCoord -= siteWidth;
        }
    }

    else //this cell is not the first
    {
        double leftCellY = elements[cellInd - 1].GetY();
        double leftCellX = elements[cellInd - 1].XCoord + elements[cellInd - 1].width;
        currCoords.xCoord -= siteWidth;

        if (leftCellY == startCoords.yCoord)
        {
            while (currCoords.xCoord >= leftCellX)
            {
                elements[cellInd].XCoord = currCoords.xCoord;
                double newFuncValue = GetFuncValue(circuit);

                if (optimalFunc > newFuncValue)
                {
                    optimalFunc = newFuncValue;
                    optimalCoords.xCoord = currCoords.xCoord;
                }

                currCoords.xCoord -= siteWidth;        
            }
        }

        else
        {
            while (currCoords.xCoord >= windowBounds[rowInd * 2])
            {
                elements[cellInd].XCoord = currCoords.xCoord;
                double newFuncValue = GetFuncValue(circuit);

                if (optimalFunc > newFuncValue)
                {
                    optimalFunc = newFuncValue;
                    optimalCoords.xCoord = currCoords.xCoord;
                }

                currCoords.xCoord -= siteWidth;        
            }

            currCoords.xCoord = windowBounds[rowInd * 2 + 3] - elements[cellInd].width;
            currCoords.yCoord = circuit.rows[rowInd + 1].coordinate;
            elements[cellInd].YCoord = currCoords.yCoord;

            while (currCoords.xCoord >= leftCellX)
            {
                elements[cellInd].XCoord = currCoords.xCoord;
                double newFuncValue = GetFuncValue(circuit);

                if (optimalFunc > newFuncValue)
                {
                    optimalFunc = newFuncValue;
                    optimalCoords.xCoord = currCoords.xCoord;
                    optimalCoords.yCoord = currCoords.yCoord;
                }

                currCoords.xCoord -= siteWidth;        
            }
        }
    }

    elements[cellInd].XCoord = startCoords.xCoord;
    elements[cellInd].YCoord = startCoords.yCoord;
    currCoords.xCoord = elements[cellInd].XCoord + siteWidth;
    currCoords.yCoord = elements[cellInd].GetY();

    //---------------------shifting to the right---------------------------------------------
    if (cellInd == elementsNum - 1) //the last cell
    {
        while (currCoords.xCoord <= (windowBounds[rowInd * 2 + 1] - elements[cellInd].width)) ///?
        {
            elements[cellInd].XCoord = currCoords.xCoord;
            double newFuncValue = GetFuncValue(circuit);

            if (optimalFunc > newFuncValue) 
            {
                optimalFunc = newFuncValue;
                optimalCoords.xCoord = currCoords.xCoord;
                optimalCoords.yCoord = currCoords.yCoord;
            }

            currCoords.xCoord += siteWidth; 
        }
    }

    else //this cell is not the last
    {
        double rightCellY = elements[cellInd + 1].GetY();
        double rightCellX = elements[cellInd + 1].XCoord;

        if (rightCellY == startCoords.yCoord)
        {
            while (currCoords.xCoord <= (rightCellX - elements[cellInd].width))
            {
                elements[cellInd].XCoord = currCoords.xCoord;
                double newFuncValue = GetFuncValue(circuit);

                if (optimalFunc > newFuncValue)
                {
                    optimalFunc = newFuncValue;
                    optimalCoords.xCoord = currCoords.xCoord;
                    optimalCoords.yCoord = currCoords.yCoord;
                }

                currCoords.xCoord += siteWidth;
            }
        }

        else
        {
            while (currCoords.xCoord <= (windowBounds[rowInd * 2 + 1] - elements[cellInd].width))
            {
                elements[cellInd].XCoord = currCoords.xCoord;
                double newFuncValue = GetFuncValue(circuit);

                if (optimalFunc > newFuncValue)
                {
                    optimalFunc = newFuncValue;
                    optimalCoords.xCoord = currCoords.xCoord;
                    optimalCoords.yCoord = currCoords.yCoord;
                }

                currCoords.xCoord += siteWidth;
            }

            currCoords.xCoord = windowBounds[(rowInd - 1) * 2];
            currCoords.yCoord = circuit.rows[rowInd - 1].coordinate;
            elements[cellInd].YCoord = currCoords.yCoord;

            while (currCoords.xCoord <= (rightCellX - elements[cellInd].width))
            {
                elements[cellInd].XCoord = currCoords.xCoord;
                double newFuncValue = GetFuncValue(circuit);

                if (optimalFunc > newFuncValue)
                {
                    optimalFunc = newFuncValue;
                    optimalCoords.xCoord = currCoords.xCoord;
                    optimalCoords.yCoord = currCoords.yCoord;
                }

                currCoords.xCoord += siteWidth;        
            }
        }
    }

    double delta = optimalFunc - startFunc;

    if ((delta < 0)) // || (rand() / RAND_MAX < exp(-delta / T)))
    {
        elements[cellInd].XCoord = optimalCoords.xCoord;
        elements[cellInd].YCoord = optimalCoords.yCoord;
    }

    else
    {
        elements[cellInd].XCoord = startCoords.xCoord;
        elements[cellInd].YCoord = startCoords.yCoord;
    }
}

void MakeWindows(Circuit &circuit, int nSteps, int nCells, int nRows)
{    
    double *x_array = NULL;    
    int *siteIndexes = NULL;
    int *cellIndexes = NULL;    
    int firstRowIdx;    

    for (int i = 0; i < nSteps; i++)
    {
        bool isWindowCreated = false;
        int nFoundCells = nCells;
        firstRowIdx = rand() % (circuit.nRows - nRows);        
        nRows = min(circuit.nRows - firstRowIdx, nRows);
        GetStartPoint(siteIndexes, nRows, circuit);
        isWindowCreated = GetWindowParams(firstRowIdx, nRows, nFoundCells, siteIndexes, cellIndexes, x_array, circuit);

        if (isWindowCreated == false)
            continue;

        bool *nodeFromThisNet = new bool [circuit.nNets];

        for (int l = 0; l < circuit.nNets; l++)
            nodeFromThisNet[l] = false;

        int changedNetsCount = 0;

        for (int netInd = 0; netInd < circuit.nNets; netInd++)        
            for (int j = 0; j < circuit.nets[netInd].numOfPins; j++)
            {
                for (int k = 0; k < nFoundCells; k++) //improve it
                {
                    if (cellIndexes[k] == circuit.nets[netInd].arrPins[j].cellIdx)
                    {
                        nodeFromThisNet[netInd] = true;                
                        break;
                    }
                }

                if (nodeFromThisNet[netInd] == true)
                {
                    changedNetsCount++;
                    break; 
                }
            }

            int *changedNetsIdx = new int [changedNetsCount];
            int currSize = 0;

            for (int k = 0; k < circuit.nNets; k++)
                if (nodeFromThisNet[k] == true)
                    changedNetsIdx[currSize++] = k;

            double currWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);

            //GeneralWindow *slidingWindow = new GeneralWindow(nRows, firstRowIdx, x_array, nFoundCells, circuit, cellIndexes);
            SAWindow *slidingWindow = new SAWindow(nRows, firstRowIdx, x_array, nFoundCells, circuit, cellIndexes,
                                                   currWL, changedNetsIdx, changedNetsCount);
            slidingWindow->SearchOnSortedCells(false, circuit);
            slidingWindow->PlaceCells(circuit);        
            printf("bounds penalty: %f\n", slidingWindow->g1WindowBounds(circuit));
            printf("overlaps: %f\n", slidingWindow->g2Overlaps(circuit));
            printf("current WL: %f\n", slidingWindow->GetFuncValue(circuit));
            delete slidingWindow;
            delete [] x_array;
            delete [] siteIndexes;
            delete [] cellIndexes;
    }
}